#include "ai.hpp"

void ai::AIPlay(Player& player, Ball& ball) {
    if (player.isAI == false) return;

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