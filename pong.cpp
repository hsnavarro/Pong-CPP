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

  void Update(sf::Time& timeElapsed) {
    sf::Vector2f previousBallPosition = ball.getPosition();
    //std::cerr << ballPosition.x << " " << ballPosition.y << std::endl;
    sf::Vector2f currentBallPosition = previousBallPosition + ballVelocity * timeElapsed.asSeconds();

    bool checkCollision = false;
    checkCollision |= DetectWallCollisionBall(currentBallPosition);
    checkCollision |= DetectRectangleCollisionBall(previousBallPosition, currentBallPosition, player1);
    checkCollision |= DetectRectangleCollisionBall(previousBallPosition, currentBallPosition, player2);

    if (checkCollision) currentBallPosition += ballVelocity * timeElapsed.asSeconds();

    ball.setPosition(currentBallPosition);

    sf::Vector2f player1Position = player1.getPosition();
    sf::Vector2f player2Position = player2.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      player1Position -= playerVelocity * timeElapsed.asSeconds();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      player1Position += playerVelocity * timeElapsed.asSeconds();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      player2Position -= playerVelocity * timeElapsed.asSeconds();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      player2Position += playerVelocity * timeElapsed.asSeconds();
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
};

int main() {
  Game game;

  sf::Clock clock;
  while (game.window.isOpen()) {

    sf::Event event;
    while (game.window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) {
        game.window.close();
      }
    }

    game.Render();

    sf::Time elapsed = clock.restart();
    game.Update(elapsed);
  }
}
