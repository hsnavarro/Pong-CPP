#include "sfml.hpp"
#include "setupInfo.hpp"
#include "physics.hpp"
#include "ai.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "GameSound.hpp"
#include "GameWindow.hpp"
#include <iostream>

const std::string BEEP_SOUND_FILE = "./music/beep-sound-enhanced.wav";
const std::string WALL_SOUND_FILE = "./music/wall-sound.ogg";

enum Direction { UP, DOWN };

const int NUM_PLAYERS = 2;
const int NUM_DIRECTIONS = 2;

const int MOVE[NUM_PLAYERS][NUM_DIRECTIONS] = {
  { sf::Keyboard::W, sf::Keyboard::S },
  { sf::Keyboard::Up, sf::Keyboard::Down }
};

class Game {
public:
  GameWindow window{ setupInfo::SCREEN_WIDTH, setupInfo::SCREEN_HEIGHT };
  std::vector<Player>players;
  Ball ball;

  GameSound beepSound{ BEEP_SOUND_FILE };
  GameSound wallSound{ WALL_SOUND_FILE };

  Game() {
    players.push_back({
     setupInfo::player1InitialPosition.x,
     setupInfo::player1InitialPosition.y
    });

    players.push_back({
      setupInfo::player2InitialPosition.x,
      setupInfo::player2InitialPosition.y,
      true
    });

    ball = {
      setupInfo::ballInitialPosition.x,
      setupInfo::ballInitialPosition.y
    };
  }

  void Update(float timeElapsed) {
    sf::Vector2f previousBallPosition = ball.shape.getPosition();
    ball.move(timeElapsed);

    std::vector<sf::Vector2f> previousPlayerPosition;

    for (auto& player : players) {
      previousPlayerPosition.push_back(player.shape.getPosition());
      player.move(timeElapsed);
    }

    bool checkBallCollision = false;

    if (physics::DetectAndFixWallCollisionBall(ball, window)) {
      wallSound.play();
      checkBallCollision = true;
    }

    for (auto& player : players) {
      if(physics::DetectAndFixWallCollisionPlayer(player, window))
        checkBallCollision = true;
    }

    for (int i = 0; i < NUM_PLAYERS; i++) {
      if (physics::DetectAndFixPlayerCollisionBall(
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

    for(auto& player : players) ai::AIPlay(player, ball);
  }

  void Render() {
    window.clear();
    for (auto& player : players) window.draw(player.shape);
    window.draw(ball.shape);
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
      game.Update(deltaTime);
      elapsed -= deltaTime;
    }

    game.Render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}
