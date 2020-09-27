#include "Player.hpp"
#include "setupInfo.hpp"

Player::Player(float positionX, float positionY, bool isAI) {
    shape.setSize({ 
      setupInfo::RECTANGLE_WIDTH, 
      setupInfo::RECTANGLE_HEIGHT 
    });
    shape.setPosition(positionX, positionY);
    this->isAI = isAI;
  }

void Player::move(float timeElapsed) {
    if (goingUp) shape.move(-velocity * timeElapsed);
    if (goingDown) shape.move(velocity * timeElapsed);
}