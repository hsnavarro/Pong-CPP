#include "Player.hpp"

Player::Player(sf::Vector2f initialPosition, bool isAI) {
    shape.setSize({ 
      setupInfo::RECTANGLE_WIDTH, 
      setupInfo::RECTANGLE_HEIGHT 
    });
    shape.setPosition(initialPosition);
    this->isAI = isAI;
    this->initialPosition = initialPosition;
  }

void Player::move(float timeElapsed) {
  if (goingUp) shape.move(-movementVelocity * timeElapsed);
  if (goingDown) shape.move(movementVelocity * timeElapsed);
}

void Player::resetPosition() {
  shape.setPosition(initialPosition);
}