#include "physics.hpp"

bool physics::detectRectangleIntersection(sf::RectangleShape& rectangleA, sf::RectangleShape& rectangleB) {
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

bool physics::detectAndFixPlayerCollisionBall(
  sf::Vector2f& previousBallPosition,
  Ball& ball,
  sf::Vector2f& previousRectanglePosition,
  Player& player,
  float timeElapsed) {

  sf::Vector2f rectanglePosition = player.shape.getPosition();

  sf::Vector2f rectangleSize = player.shape.getSize();
  float rectangleWidth = rectangleSize.x;
  float rectangleHeight = rectangleSize.y;

  float xLeft = rectanglePosition.x;
  float xRight = rectanglePosition.x + rectangleWidth;

  float yTop = rectanglePosition.y;
  float yBottom = rectanglePosition.y + rectangleHeight;

  float ballRadius = ball.shape.getRadius();
  sf::RectangleShape ballCollisionBox({ 2 * ballRadius, 2 * ballRadius });
  ballCollisionBox.setPosition(ball.shape.getPosition());

  float rectangleAceleration = (rectanglePosition.y - previousRectanglePosition.y) / timeElapsed;

  sf::Vector2f previousBallCenter = previousBallPosition + sf::Vector2f(ballRadius, ballRadius);

  if (detectRectangleIntersection(player.shape, ballCollisionBox)) {
    //ball.velocity.x *= -1;

    bool horizontalApproach = previousBallCenter.x <= xLeft or previousBallCenter.x >= xRight;
    bool verticalApproach = previousBallCenter.y <= yTop or previousBallCenter.y >= yBottom;

    if (horizontalApproach) ball.velocity.x *= -1;
    if (verticalApproach) ball.velocity.y *= -1;
    if (horizontalApproach and !verticalApproach) ball.velocity.y += 0.3 * rectangleAceleration;


    return true;
  }

  return false;
}

bool physics::detectAndFixWallCollisionPlayer(Player& player, GameWindow& window) {

  float xLeft = player.shape.getPosition().x;
  float yTop = player.shape.getPosition().y;
  float rectangleHeight = player.shape.getSize().y;

  float screenHeight = window.getSize().y;

  if (yTop <= 0.0 or yTop + rectangleHeight >= screenHeight) {
    yTop = std::max(yTop, 0.0f);
    yTop = std::min(yTop, screenHeight - rectangleHeight);
    player.shape.setPosition({ xLeft, yTop });
    return true;
  }

  return false;
}

bool physics::detectAndFixWallCollisionBall(Ball& ball, GameWindow& window) {

  sf::Vector2f ballPosition = ball.shape.getPosition();
  float ballRadius = ball.shape.getRadius();

  sf::Vector2u screenSize = window.getSize();
  float screenHeight = screenSize.y;
  float screenWidth = screenSize.x;

  if (ballPosition.y + 2 * ballRadius >= screenHeight or
    ballPosition.y <= 0) {
    ball.velocity.y *= -1;
    return true;
  }

  return false;
}