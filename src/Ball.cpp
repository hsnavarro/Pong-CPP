#include "Ball.hpp"
#include "setupInfo.hpp"

Ball::Ball(float positionX, float positionY) {
  shape.setRadius(setupInfo::BALL_RADIUS);
  shape.setPosition(positionX, positionY);
}

void Ball::move(float timeElapsed) {
  shape.move(velocity * timeElapsed);
}