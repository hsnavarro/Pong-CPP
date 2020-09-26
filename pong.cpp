#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <unistd.h> 
#include <stdlib.h> 

const int screenWidth = 800;
const int screenHeight = 400;
const int rectangleWidth = 0.01 * screenWidth;
const int rectangleHeight = 0.2 * screenHeight;
const int ballSize = 5;

class Game {
public:
  sf::RenderWindow window;
  sf::RectangleShape player1;
  sf::RectangleShape player2;
  sf::CircleShape ball;
  sf::Vector2f ballVelocity;
  sf::Vector2f playerVelocity;
  bool player1Up;
  bool player1Down;
  bool player2Up;
  bool player2Down;

  Game() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    window.create(sf::VideoMode(screenWidth, screenHeight), "Pong Game", sf::Style::Default, settings);
    //window.setFramerateLimit(100);

    player1.setSize({ rectangleWidth, rectangleHeight });
    player2.setSize({ rectangleWidth, rectangleHeight });
    ball.setRadius(ballSize);

    ballVelocity = { 200, 0 };
    playerVelocity = { 0, 300 };

    player1.setPosition(0.1 * screenWidth, 0.5 * screenHeight - 0.5 * rectangleHeight);
    player2.setPosition(0.9 * screenWidth, 0.5 * screenHeight - 0.5 * rectangleHeight);
    ball.setPosition(0.5 * screenWidth - ballSize, 0.5 * screenHeight - ballSize);

    player1Up = false;
    player1Down = false;
    player2Up = false;
    player2Down = false;
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

  bool DetectRectangleCollisionBall(sf::Vector2f& previousBallPosition, sf::CircleShape& ball, sf::Vector2f& previousRectanglePosition, sf::RectangleShape& rectangle, float timeElapsed) {

    sf::Vector2f rectanglePosition = rectangle.getPosition();

    float xLeft = rectanglePosition.x;
    float xRight = rectanglePosition.x + rectangleWidth;

    float yTop = rectanglePosition.y;
    float yBottom = rectanglePosition.y + rectangleHeight;

    sf::RectangleShape ballCollisionBox({ 2 * ball.getRadius(), 2 * ball.getRadius() });
    ballCollisionBox.setPosition(ball.getPosition());

    float rectangleAceleration = (rectanglePosition.y - previousRectanglePosition.y) / timeElapsed;

    if (rectangleIntersection(rectangle, ballCollisionBox)) {
      bool horizontalApproach = previousBallPosition.x < xLeft or previousBallPosition.x > xRight;
      bool verticalApproach = previousBallPosition.y < yTop or previousBallPosition.y > yBottom;

      if (horizontalApproach) ballVelocity.x *= -1;
      if (verticalApproach) ballVelocity.y *= -1;
      if (horizontalApproach and !verticalApproach) ballVelocity.y += 0.3 * rectangleAceleration;

      return true;
    }

    return false;
  }

  void DetectWallCollisionPlayer(sf::RectangleShape& player) {

    float xLeft = player.getPosition().x;
    float yTop = player.getPosition().y;

    if (yTop <= 0) yTop = 0;

    if (yTop + rectangleHeight >= screenHeight)
      yTop = screenHeight - rectangleHeight;

    player.setPosition({ xLeft,  yTop });
  }

  bool DetectWallCollisionBall(sf::CircleShape& ball) {
    // wall collision

    sf::Vector2f ballPosition = ball.getPosition();
    float ballRadius = ball.getRadius();

    if (ballPosition.y + 2 * ballRadius >= screenHeight or
      ballPosition.y <= 0) {

      ballVelocity.y *= -1;
      return true;
    }

    if (ballPosition.x + 2 * ballRadius >= screenWidth or
      ballPosition.x <= 0) {

      ballVelocity.x *= -1;
      return true;
    }
    return false;
  }

  void Update(float timeElapsed) {
    sf::Vector2f previousBallPosition = ball.getPosition();
    ball.move(ballVelocity * timeElapsed);

    sf::Vector2f previousPlayer1Position = player1.getPosition();
    sf::Vector2f previousPlayer2Position = player2.getPosition();

    if (player1Up) player1.move(-playerVelocity * timeElapsed);
    if (player1Down) player1.move(playerVelocity * timeElapsed);
    if (player2Up) player2.move(-playerVelocity * timeElapsed);
    if (player2Down) player2.move(playerVelocity * timeElapsed);

    bool checkBallCollision = false;
    checkBallCollision |= DetectWallCollisionBall(ball);
    checkBallCollision |= DetectRectangleCollisionBall(previousBallPosition, ball, previousPlayer1Position, player1, timeElapsed);
    checkBallCollision |= DetectRectangleCollisionBall(previousBallPosition, ball, previousPlayer2Position, player2, timeElapsed);

    if (checkBallCollision) ball.move(ballVelocity * timeElapsed);

    DetectWallCollisionPlayer(player1);
    DetectWallCollisionPlayer(player2);
  }

  void Render() {
    window.clear(sf::Color::Black);
    window.draw(player1);
    window.draw(player2);
    window.draw(ball);
    window.display();
  }

  void handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::W) {
        player1Up = true;
      }

      if (event.key.code == sf::Keyboard::S) {
        player1Down = true;
      }

      if (event.key.code == sf::Keyboard::Up) {
        player2Up = true;
      }

      if (event.key.code == sf::Keyboard::Down) {
        player2Down = true;
      }
    }

    if (event.type == sf::Event::KeyReleased) {
      if (event.key.code == sf::Keyboard::W) {
        player1Up = false;
      }

      if (event.key.code == sf::Keyboard::S) {
        player1Down = false;
      }

      if (event.key.code == sf::Keyboard::Up) {
        player2Up = false;
      }

      if (event.key.code == sf::Keyboard::Down) {
        player2Down = false;
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
    std::cerr << 1 / elapsed << std::endl;

    game.Update(elapsed);
    game.Render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}
