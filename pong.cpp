#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int RECTANGLE_WIDTH = 0.01 * SCREEN_WIDTH;
const int RECTANGLE_HEIGHT = 0.2 * SCREEN_HEIGHT;
const int BALL_RADIUS = 5;
const int ANTI_ALIASING_LEVEL = 8;
const int FRAME_RATE = 100;
const std::string BEEP_SOUND_FILE = "music/beep-sound-enhanced.wav";
const std::string WALL_SOUND_FILE = "music/wall-sound.ogg";

enum Direction { UP, DOWN };

const int NUM_PLAYERS = 2;
const int NUM_DIRECTIONS = 2;

const int MOVE[NUM_PLAYERS][NUM_DIRECTIONS] = {
  { sf::Keyboard::W, sf::Keyboard::S },
  { sf::Keyboard::Up, sf::Keyboard::Down }
};

class Player {
public:
  sf::RectangleShape shape;
  sf::Vector2f velocity = { 0, 300 };
  bool goingUp = false;
  bool goingDown = false;
  bool isAI;

  Player(float positionX = 0, float positionY = 0, bool isAI = false) {
    shape.setSize({ RECTANGLE_WIDTH, RECTANGLE_HEIGHT });
    shape.setPosition(positionX, positionY);
    this->isAI = isAI;
  }

  void move(float timeElapsed) {
    if (goingUp) shape.move(-velocity * timeElapsed);
    if (goingDown) shape.move(velocity * timeElapsed);
  }
};

class Ball {
public:
  sf::CircleShape shape;
  sf::Vector2f velocity = { 200, 0 };

  Ball(float positionX = 0, float positionY = 0) {
    shape.setRadius(BALL_RADIUS);
    shape.setPosition(positionX, positionY);
  }

  void move(float timeElapsed) {
    shape.move(velocity * timeElapsed);
  }
};

class GameSound {
private:
  sf::SoundBuffer soundBuffer;
  sf::Sound sound;

public:
  GameSound(std::string fileName) {
    if (soundBuffer.loadFromFile(fileName))
      sound.setBuffer(soundBuffer);
  }

  void play() {
    sound.play();
  }
};

class GameWindow {
private:
  sf::RenderWindow window;

public:
  GameWindow(float width, float height) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
    window.create(sf::VideoMode(width, height), "Pong Game", sf::Style::Default, settings);
    window.setFramerateLimit(FRAME_RATE);
  }

  void clear() {
    window.clear(sf::Color::Black);
  }

  void draw(sf::Shape& shape) {
    window.draw(shape);
  }

  void display() {
    window.display();
  }

  bool pollEvent(sf::Event& event) {
    return window.pollEvent(event);
  }

  bool isOpen() {
    return window.isOpen();
  }

  void close() {
    window.close();
  }
};

class Game {
public:
  GameWindow window{ SCREEN_WIDTH, SCREEN_HEIGHT };
  std::vector<Player>players;
  Ball ball;

  GameSound beepSound{ BEEP_SOUND_FILE };
  GameSound wallSound{ WALL_SOUND_FILE };

  Game() {
    players.push_back({ 0.1 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT });
    players.push_back({ 0.9 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT });
    ball = { 0.5 * SCREEN_WIDTH - BALL_RADIUS, 0.5 * SCREEN_HEIGHT - BALL_RADIUS };
  }

  bool rectangleIntersection(sf::RectangleShape& rectangleA, sf::RectangleShape& rectangleB) {
    float xLeftA = rectangleA.getPosition().x;
    float xRightA = xLeftA + rectangleA.getSize().x;

    float yTopA = rectangleA.getPosition().y;
    float yBottomA = yTopA + rectangleA.getSize().y;

    float xLeftB = rectangleB.getPosition().x;
    float xRightB = xLeftB + rectangleB.getSize().x;

    float yTopB = rectangleB.getPosition().y;
    float yBottomB = yTopB + rectangleB.getSize().y;

    bool firstCondition = !(xRightA < xLeftB or xRightB < xLeftA);
    bool secondCondition = !(yBottomB < yTopA or yBottomA < yTopB);

    return firstCondition and secondCondition;
  }

  bool DetectRectangleCollisionBall(
    sf::Vector2f& previousBallPosition,
    Ball& ball,
    sf::Vector2f& previousRectanglePosition,
    Player& player,
    float timeElapsed) {

    sf::Vector2f rectanglePosition = player.shape.getPosition();

    float xLeft = rectanglePosition.x;
    float xRight = rectanglePosition.x + RECTANGLE_WIDTH;

    float yTop = rectanglePosition.y;
    float yBottom = rectanglePosition.y + RECTANGLE_HEIGHT;

    float ballRadius = ball.shape.getRadius();
    sf::RectangleShape ballCollisionBox({ 2 * ballRadius, 2 * ballRadius });
    ballCollisionBox.setPosition(ball.shape.getPosition());

    float rectangleAceleration = (rectanglePosition.y - previousRectanglePosition.y) / timeElapsed;

    if (rectangleIntersection(player.shape, ballCollisionBox)) {
      beepSound.play();
      bool horizontalApproach = previousBallPosition.x <= xLeft or previousBallPosition.x >= xRight;
      bool verticalApproach = previousBallPosition.y <= yTop or previousBallPosition.y >= yBottom;

      if (horizontalApproach) ball.velocity.x *= -1;
      if (verticalApproach) ball.velocity.y *= -1;
      if (horizontalApproach and !verticalApproach) ball.velocity.y += 0.3 * rectangleAceleration;

      return true;
    }

    return false;
  }

  void DetectWallCollisionPlayer(Player& player) {

    float xLeft = player.shape.getPosition().x;
    float yTop = player.shape.getPosition().y;

    if (yTop <= 0) yTop = 0;

    if (yTop + RECTANGLE_HEIGHT >= SCREEN_HEIGHT)
      yTop = SCREEN_HEIGHT - RECTANGLE_HEIGHT;

    player.shape.setPosition({ xLeft,  yTop });
  }

  bool DetectWallCollisionBall(Ball& ball) {

    sf::Vector2f ballPosition = ball.shape.getPosition();
    float ballRadius = ball.shape.getRadius();

    if (ballPosition.y + 2 * ballRadius >= SCREEN_HEIGHT or
      ballPosition.y <= 0) {

      wallSound.play();
      ball.velocity.y *= -1;
      return true;
    }

    if (ballPosition.x + 2 * ballRadius >= SCREEN_WIDTH or
      ballPosition.x <= 0) {

      wallSound.play();
      ball.velocity.x *= -1;
      return true;
    }
    return false;
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

    checkBallCollision |= DetectWallCollisionBall(ball);
    for (int i = 0; i < NUM_PLAYERS; i++) {
      checkBallCollision |= DetectRectangleCollisionBall(
        previousBallPosition,
        ball,
        previousPlayerPosition[i],
        players[i],
        timeElapsed
      );
    }

    if (checkBallCollision) ball.move(timeElapsed);

    for (auto& player : players)
      DetectWallCollisionPlayer(player);

    for (int i = 0; i < NUM_PLAYERS; i++) 
      if(players[i].isAI)AIMove(i);
  }

  void Render() {
    window.clear();
    for (auto& player : players) window.draw(player.shape);
    window.draw(ball.shape);
    window.display();
  }

  void AIMove(int playerID) {
    /*
    float yHitPosition;
    sf::Vector2f ballPosition = ball.shape.getPosition();

        if(ballVelocity.x > 0) {
          float timeToHit = (player.getPosition().x - ballPosition.x) / ballVelocity.x

          float yHitPosition = ballPosition.y + ballVelocity.y * timeToHit;


        }
    */
    Player& player = players[playerID];
    if(!player.isAI) return;

    float yBall = ball.shape.getPosition().y;

    float yPlayerTop = player.shape.getPosition().y;
    float yPlayerBottom = yPlayerTop + player.shape.getSize().y;

    if (yBall < yPlayerTop) {
      player.goingUp = true;
      player.goingDown = false;
    } else if (yBall > yPlayerBottom) {
      player.goingUp = false;
      player.goingDown = true;
    } else {
      player.goingUp = false;
      player.goingDown = false;
    }
  }

  void handleEvent(sf::Event& event) {
    if (event.type == sf::Event::Closed) window.close();

    if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {

       bool updateMovement = (event.type == sf::Event::KeyPressed);

      for(int i = 0; i < NUM_PLAYERS; i++) {
        Player& player = players[i];
        if(!player.isAI) {
          if(event.key.code == MOVE[i][UP]) player.goingUp = updateMovement;
          if(event.key.code == MOVE[i][DOWN]) player.goingDown = updateMovement;
        }
      }
    }
  }
};

int main() {
  Game game;

  sf::Clock clock;
  sf::Event event;

  while (game.window.isOpen()) {
    float elapsed = clock.restart().asSeconds();
    //std::cerr << 1 / elapsed << std::endl;

    game.Update(elapsed);
    game.Render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}
