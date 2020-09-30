#include "Ball.hpp"

Ball::Ball(sf::Vector2f initialPosition) {
  this->initialPosition = initialPosition;
  shape.setRadius(setupInfo::BALL_RADIUS);
  shape.setPosition(initialPosition);
}

void Ball::move(float timeElapsed) {
  shape.move(velocity * timeElapsed);
}

void Ball::resetPosition() {
  shape.setPosition(initialPosition);
  velocity = setupInfo::ballInitialVelocity;
}