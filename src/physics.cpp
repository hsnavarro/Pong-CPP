#include <cmath>

#include "sfml.hpp"

#include "Ball.hpp"
#include "physics.hpp"
#include "Player.hpp"

/* Debug
#include <iostream>

static std::ostream& operator<<(std::ostream& os, const sf::Vector2f& point) {
  os << "(" << point.x << ", " << point.y << ")";
  return os;
}

namespace sf {
  using Line = std::array<sf::Vector2f, 2>;
}

static void drawLine(const sf::Line& line, const sf::Color& color, GameWindow& window) {
  sf::Vertex verts[] = { { line[0], color }, { line[1], color } };
  window.window.draw(verts, 2, sf::Lines);
}
*/

const float EPS = 1e-9;

static sf::Vector2f rotate(sf::Vector2f& vector, const float angle) {

  float newX = cos(angle) * vector.x - sin(angle) * vector.y;
  float newY = sin(angle) * vector.x + cos(angle) * vector.y;

  vector = { newX, newY };

  return vector;
}

static sf::Vector2f rotate90ACW(sf::Vector2f& vector) {

  float newX = -vector.y;
  float newY = vector.x;

  vector = { newX, newY };

  return vector;
}

static sf::Vector2f rotate90CW(sf::Vector2f& vector) {

  float newX = vector.y;
  float newY = -vector.x;

  vector = { newX, newY };

  return vector;
}

static sf::Vector2f getPerpendicular(const sf::Vector2f& vector) {

  sf::Vector2f newVector = vector;
  rotate90CW(newVector);

  return newVector;
}

static float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b) {
  return a.x * b.x + a.y * b.y;
}

static float crossProduct(const sf::Vector2f& a, const sf::Vector2f& b) {
  return a.x * b.y - a.y * b.x;
}

static float getMagnitude(const sf::Vector2f& vector) {
  return sqrt(dotProduct(vector, vector));
}

static sf::Vector2f getUnitVector(const sf::Vector2f& vector) {
  float magnitude = getMagnitude(vector);
  return magnitude > EPS ? vector/magnitude : vector;
}


static std::vector<sf::Vector2f> getShapePoints(const sf::Shape& shape) {
  const auto transform = shape.getTransform();
  const int numOfPoints = shape.getPointCount();

  std::vector<sf::Vector2f> points(numOfPoints);

  for (int i = 0; i < numOfPoints; i++) {
    const auto point = transform.transformPoint(shape.getPoint(i));
    points[i] = point;
  }

  return points;
}

static std::vector<sf::Vector2f> getShapeNormals(const sf::Shape& shape) {

  const auto points = getShapePoints(shape);
  const int numOfPoints = static_cast<int>(points.size());

  std::vector<sf::Vector2f> normals(numOfPoints);

  for (int i = 1; i <= numOfPoints; i++) {
    auto sideVector = points[i % numOfPoints] - points[i - 1];
    auto normalVector = rotate90ACW(sideVector) / getMagnitude(sideVector);
    normals[i - 1] = normalVector;
  }

  return normals;
}

static sf::Vector2f getCircleCenter(const sf::CircleShape& circle) {
  const sf::Vector2f position = circle.getPosition();
  const float circleRadius = circle.getRadius();

  return { position.x + circleRadius, position.y + circleRadius };
}

static sf::Vector2f getRectangleCenter(const sf::RectangleShape& rectangle) {
  const auto position = rectangle.getPosition();
  const auto localBounds = rectangle.getLocalBounds();

  const sf::Vector2f localCenter = { localBounds.left + localBounds.width * 0.5f, localBounds.top + localBounds.height * 0.5f };

  auto transform = rectangle.getTransform();

  return transform.transformPoint(localCenter);
}

struct Manifold {
  sf::RectangleShape& a;
  sf::CircleShape& b;
  sf::Vector2f normal;
  float penetration;
};

static bool isInsideConvexPolygon(const sf::Shape& shape, const sf::Vector2f& point) {
  std::vector<sf::Vector2f> polygonPoints = getShapePoints(shape);

  const int numOfPoints = static_cast<int>(polygonPoints.size());

  for (int i = 1; i <= numOfPoints; i++) {
    sf::Vector2f side = polygonPoints[i % numOfPoints] - polygonPoints[i - 1];
    if (crossProduct(side, point - polygonPoints[i - 1]) < 0) return false;
  }

  return true;
}

static sf::Vector2f closestPointInConvexPolygonBorder(const sf::Shape& shape, const sf::Vector2f& point) {
  float minDistance = std::numeric_limits<float>::max();
  sf::Vector2f closest;

  const std::vector<sf::Vector2f> polygonPoints = getShapePoints(shape);
  const int numOfPoints = static_cast<int>(polygonPoints.size());

  //corners
  for (int i = 0; i < numOfPoints; i++) {

    const float distance = getMagnitude(polygonPoints[i] - point);

    if (distance < minDistance) {
      minDistance = distance;
      closest = polygonPoints[i];
    }

  }

  // sides
  for (int i = 1; i <= numOfPoints; i++) {
    sf::Vector2f sideDirection = polygonPoints[i % numOfPoints] - polygonPoints[i - 1];
    const float sideSize = getMagnitude(sideDirection);

    sideDirection /= sideSize;

    const sf::Vector2f vectorToPoint = point - polygonPoints[i - 1];

    const sf::Vector2f vectorToPointProjection = dotProduct(vectorToPoint, sideDirection) * sideDirection;

    if (getMagnitude(vectorToPointProjection) > sideSize or dotProduct(vectorToPointProjection, sideDirection) < 0.0f) continue;

    const sf::Vector2f pointProjectedOnSide = polygonPoints[i - 1] + vectorToPointProjection;

    const float distance = getMagnitude(pointProjectedOnSide - point);

    if (distance < minDistance) {
      minDistance = distance;
      closest = pointProjectedOnSide;
    }

  }

  return closest;
}

static bool rectangleCircleCollision(Manifold& m) {
  const sf::Vector2f circleCenter = getCircleCenter(m.b);
  const float circleRadius = m.b.getRadius();

  sf::Vector2f closest = closestPointInConvexPolygonBorder(m.a, circleCenter);

  m.normal = circleCenter - closest;
  const float normalMagnitude = getMagnitude(m.normal);
  const bool isInside = isInsideConvexPolygon(m.a, circleCenter);

  if (!isInside and normalMagnitude > circleRadius) return false;

  if (isInside) {
    m.normal *= -1.0f;
    m.penetration = circleRadius + normalMagnitude;
  } else {
    m.penetration = circleRadius - normalMagnitude;
  }

  m.normal = getUnitVector(m.normal);

  return true;
}

bool physics::ballToPlayerCollision(Ball& ball, Player& player) {

  Manifold m = { player.shape, ball.shape };

  if (rectangleCircleCollision(m)) {
    ball.shape.move(m.normal * m.penetration);

    const sf::Vector2f perpendicularDirection = getPerpendicular(m.normal);

    const sf::Vector2f ballPerpendicularVelocity = dotProduct(ball.velocity, perpendicularDirection) * perpendicularDirection;

    const sf::Vector2f ballParallelVelocity = static_cast<float>(fabs(dotProduct(ball.velocity, m.normal))) * m.normal;

    const float rectangleParallelVelocityMagnitude = std::max(dotProduct(player.velocity, m.normal), 0.0f);

    const sf::Vector2f rectangleParallelVelocity = rectangleParallelVelocityMagnitude * m.normal;

    ball.velocity = ballPerpendicularVelocity + ballParallelVelocity + rectangleParallelVelocity;

    const sf::Vector2f unitXDirection = { 1.0f, 0.0f };

    if (m.normal == unitXDirection or m.normal == -unitXDirection) {
      ball.velocity.y += 0.2f * player.velocity.y;
    }


    /* Debug
    const sf::Vector2f ballCenter = getCircleCenter(ball.shape);

    const sf::Vector2f unitVector = { 1.0f, 1.0f };

    drawLine({ ballCenter + 4.f * unitVector, ballCenter + 4.f * unitVector + 40.0f * m.normal }, sf::Color::Green, window);
    drawLine({ ballCenter + 2.f * unitVector, ballCenter + 2.f * unitVector + 40.0f * getUnitVector(ballPerpendicularVelocity) }, sf::Color::Blue, window);
    drawLine({ ballCenter - 2.f * unitVector, ballCenter - 2.f * unitVector + 40.0f * getUnitVector(ballParallelVelocity) }, sf::Color::Red, window);
    drawLine({ ballCenter - 4.f * unitVector, ballCenter - 4.f * unitVector + 40.0f * getUnitVector(rectangleParallelVelocity) }, sf::Color::Yellow, window);

    window.display();
    */

    return true;
  }

  return false;
}

bool physics::playerToWallCollision(Player& player, GameWindow& window) {

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

bool physics::ballToWallCollision(Ball& ball, GameWindow& window) {

  sf::Vector2f ballPosition = ball.shape.getPosition();
  float ballRadius = ball.shape.getRadius();

  sf::Vector2u screenSize = window.getSize();
  float screenHeight = screenSize.y;
  float screenWidth = screenSize.x;

  float xLeft = ball.shape.getPosition().x;
  float yTop = ball.shape.getPosition().y;

  if (ballPosition.y + 2 * ballRadius >= screenHeight or
    ballPosition.y <= 0) {

    yTop = std::max(yTop, 0.0f);
    yTop = std::min(yTop, screenHeight - 2 * ballRadius);
    ball.shape.setPosition({ xLeft, yTop });
    ball.velocity.y *= -1;
    return true;
  }

  return false;
}