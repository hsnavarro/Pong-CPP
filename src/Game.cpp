#include "sfml.hpp"
#include "setupInfo.hpp"
#include "physics.hpp"
#include "ai.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "GameSound.hpp"
#include "GameWindow.hpp"
#include "GameText.hpp"
#include <iostream>

const std::string BEEP_SOUND_FILE = "./audio/beep-sound-enhanced.wav";
const std::string WALL_SOUND_FILE = "./audio/wall-sound.ogg";
const std::string POINT_SOUND_FILE = "./audio/point-sound.wav";
const std::string WIN_SOUND_FILE = "./audio/win-sound.wav";
const std::string FONT_FILE = "./fonts/main-font.ttf";

enum Direction { UP, DOWN };

const int NUM_PLAYERS = 2;
const int NUM_DIRECTIONS = 2;

const int MOVE[NUM_PLAYERS][NUM_DIRECTIONS] = {
  { sf::Keyboard::W, sf::Keyboard::S },
  { sf::Keyboard::Up, sf::Keyboard::Down }
};

class Game {
public:
  std::vector<Player>players;
  Ball ball;
  bool isMatchEnd = false;

  GameWindow window{ setupInfo::SCREEN_WIDTH, setupInfo::SCREEN_HEIGHT };

  GameSound beepSound{ BEEP_SOUND_FILE };
  GameSound wallSound{ WALL_SOUND_FILE };
  GameSound pointSound{ POINT_SOUND_FILE };
  GameSound winSound{ WIN_SOUND_FILE };

  GameText scoreText{ FONT_FILE };
  GameText winText{ FONT_FILE };

  Game() {
    scoreText.setString(scoreText.stringifyScore(0, 0));
    players.push_back({ setupInfo::player1InitialPosition });
    players.push_back({ setupInfo::player2InitialPosition, true });
  }

  void resetPosition() {
    ball.resetPosition();
    for (auto& player : players) player.resetPosition();
  }

  void updateScoreBoard() {
    scoreText.setString(scoreText.stringifyScore(players[0].points, players[1].points));
  }

  void checkMatchEnd() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
      if(players[i].points == setupInfo::MAX_PTS) {
        
        winText.setString("Player " + std::to_string(i + 1) + " wins!");
        winText.text.setPosition(winText.text.getPosition().x, 0.5 * setupInfo::SCREEN_HEIGHT);
        
        ball.shape.setFillColor(sf::Color::Black);
        isMatchEnd = true;

        winSound.play();
        return;
      } 
    }
  }

  void processPoint() {
    pointSound.play();
    updateScoreBoard();
    resetPosition();
    checkMatchEnd();
  }

  void detectAndHandlePoint() {
    sf::Vector2f ballPosition = ball.shape.getPosition();
    float ballRadius = ball.shape.getRadius();

    sf::Vector2u screenSize = window.getSize();
    float screenWidth = screenSize.x;

    if (ballPosition.x + 2 * ballRadius >= screenWidth) {
      players[0].points++;
      processPoint();
    }

    if (ballPosition.x <= 0) {
      players[1].points++;
      processPoint();
    }
  }

  void update(float timeElapsed) {
    if(isMatchEnd) return;

    sf::Vector2f previousBallPosition = ball.shape.getPosition();
    ball.move(timeElapsed);

    std::vector<sf::Vector2f> previousPlayerPosition;

    for (auto& player : players) {
      previousPlayerPosition.push_back(player.shape.getPosition());
      player.move(timeElapsed);
    }

    bool checkBallCollision = false;

    if (physics::detectAndFixWallCollisionBall(ball, window)) {
      wallSound.play();
      checkBallCollision = true;
    }

    for (auto& player : players)
      physics::detectAndFixWallCollisionPlayer(player, window);

    for (int i = 0; i < NUM_PLAYERS; i++) {
      if (physics::detectAndFixPlayerCollisionBall(
        previousBallPosition,
        ball,
        previousPlayerPosition[i],
        players[i],
        timeElapsed
      )) {

        checkBallCollision = true;
        beepSound.play();
      }
    }

    if (checkBallCollision) ball.move(timeElapsed);

    for (auto& player : players) ai::AIPlay(player, ball);

    detectAndHandlePoint();
  }

  void render() {
    window.clear();
    for (auto& player : players) window.draw(player.shape);
    window.draw(ball.shape);

    window.draw(scoreText.text);

    if(isMatchEnd) window.draw(winText.text);

    window.display();
  }

  void handleEvent(sf::Event& event) {
    if (event.type == sf::Event::Closed) window.close();

    if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {

      bool updateMovement = (event.type == sf::Event::KeyPressed);

      for (int i = 0; i < NUM_PLAYERS; i++) {
        Player& player = players[i];
        if (!player.isAI) {
          if (event.key.code == MOVE[i][UP]) player.goingUp = updateMovement;
          if (event.key.code == MOVE[i][DOWN]) player.goingDown = updateMovement;
        }
      }
    }
  }
};

int main() {
  Game game;

  sf::Clock clock;
  sf::Event event;

  float dt = (1 / 4000.0);

  while (game.window.isOpen()) {
    float elapsed = clock.restart().asSeconds();
    //std::cerr << 1 / elapsed << std::endl;

    while (elapsed > 0.0) {
      float deltaTime = std::min(elapsed, dt);
      game.update(deltaTime);
      elapsed -= deltaTime;
    }

    game.render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}
