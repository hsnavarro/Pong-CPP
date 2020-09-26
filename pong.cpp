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
const long double EPS = 1e-9;

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
    window.create(sf::VideoMode(screenWidth, screenHeight), "Pong Game");
    //window.setVerticalSyncEnable(true);

    player1.setSize(sf::Vector2f(rectangleWidth, rectangleHeight));
    player2.setSize(sf::Vector2f(rectangleWidth, rectangleHeight));
    ball.setRadius(ballSize);
    ballVelocity = { 100, 0 };
    playerVelocity = { 0, 150 };

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
    float xRightA = xLeftA + rectangleWidth;

    float yTopA = rectangleA.getPosition().y;
    float yBottomA = yTopA + rectangleHeight;

    float xLeftB = rectangleB.getPosition().x;
    float xRightB = xLeftB + rectangleWidth;

    float yTopB = rectangleB.getPosition().y;
    float yBottomB = yTopB + rectangleHeight;

    bool firstCondition = !(xRightA < xLeftB or xRightB < xLeftA);
    bool secondCondition = !(yBottomB < yTopA or yBottomA < yTopB);

    return firstCondition and secondCondition;
  }

  bool DetectRectangleCollisionBall(sf::Vector2f& previousBallPosition, sf::Vector2f& currentBallPosition, sf::RectangleShape& rectangle) {

    sf::Vector2f rectanglePosition = rectangle.getPosition();

    float xLeft = rectanglePosition.x;
    float xRight = rectanglePosition.x + rectangleWidth;

    float yTop = rectanglePosition.y;
    float yBottom = rectanglePosition.y + rectangleHeight;

    sf::RectangleShape ballCollisionBox(sf::Vector2f(2 * ballSize, 2 * ballSize));
    ballCollisionBox.setPosition(currentBallPosition.x, currentBallPosition.y);

    if (rectangleIntersection(rectangle, ballCollisionBox)) {
      if (previousBallPosition.x < xLeft or previousBallPosition.x > xRight) {
        ballVelocity.x *= -1;
        return true;
      }

      if (previousBallPosition.y < yTop or previousBallPosition.y > yBottom) {
        ballVelocity.y *= -1;
        return true;
      }
    }

    return false;
  }

  bool DetectWallCollisionBall(sf::Vector2f& ballPosition) {
    // wall collision

    if (ballPosition.y + 2 * ballSize >= screenHeight or
      ballPosition.y <= 0) {

      ballVelocity.y *= -1;
      return true;
    }

    if (ballPosition.x + 2 * ballSize >= screenWidth or
      ballPosition.x <= 0) {

      ballVelocity.x *= -1;
      return true;
    }
    return false;
  }

  void Update(float timeElapsed) {
    sf::Vector2f previousBallPosition = ball.getPosition();
    //std::cerr << ballPosition.x << " " << ballPosition.y << std::endl;
    sf::Vector2f currentBallPosition = previousBallPosition + ballVelocity * timeElapsed;

    bool checkBallCollision = false;
    checkBallCollision |= DetectWallCollisionBall(currentBallPosition);
    checkBallCollision |= DetectRectangleCollisionBall(previousBallPosition, currentBallPosition, player1);
    checkBallCollision |= DetectRectangleCollisionBall(previousBallPosition, currentBallPosition, player2);

    if (checkBallCollision) currentBallPosition += ballVelocity * timeElapsed;

    ball.setPosition(currentBallPosition);

    sf::Vector2f player1Position = player1.getPosition();
    sf::Vector2f player2Position = player2.getPosition();

    std::cerr << "(player1Position) " << player1Position.x << " " << player1Position.y << std::endl;
    std::cerr << "(player2Position) " << player2Position.x << " " << player2Position.y << std::endl;

    if (player1Up) {
      player1Position -= playerVelocity * timeElapsed;
    }

    if (player1Down) {
      player1Position += playerVelocity * timeElapsed;
    }

    if (player2Up) {
      player2Position -= playerVelocity * timeElapsed;
    }

    if (player2Down) {
      player2Position += playerVelocity * timeElapsed;
    }

    player1.setPosition(player1Position);
    player2.setPosition(player2Position);
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

    game.Update(elapsed);
    game.Render();

    while (game.window.pollEvent(event)) {
      game.handleEvent(event);
    }
  }
}
