#include <cmath>

#include "ai.hpp"

void ai::followCoordinate(Player& player, Ball& ball, float y) {
  float yPlayerTop = player.shape.getPosition().y;
  float yPlayerBottom = yPlayerTop + player.shape.getSize().y;
  float ballRadius = ball.shape.getRadius();

  if (y < yPlayerTop) {
    player.goingUp = true;
    player.goingDown = false;
  } else if (y + 2 * ballRadius > yPlayerBottom) {
    player.goingUp = false;
    player.goingDown = true;
  } else {
    player.goingUp = false;
    player.goingDown = false;
  }
}

void ai::AIPlayHard(Player& player, Ball& ball) {
  float ballRadius = ball.shape.getRadius();
  float pathLength = setupInfo::SCREEN_HEIGHT - 2 * ballRadius;

  float leftImpact = setupInfo::player1InitialPosition.x + player.shape.getSize().x;
  float rightImpact = setupInfo::player2InitialPosition.x - 2 * ballRadius;

  float xOfImpact = ball.velocity.x > 0 ? rightImpact : leftImpact;
  sf::Vector2f ballPosition = ball.shape.getPosition(); 
  float timeUntilImpact = (xOfImpact - ballPosition.x) / ball.velocity.x;

  float newY = ballPosition.y + ball.velocity.y * timeUntilImpact;

  int numberOfReflections = floor(newY / pathLength);

  float fixedY = newY - numberOfReflections * pathLength;
  float yOfImpact = (numberOfReflections % 2) ? pathLength - fixedY : fixedY;

  followCoordinate(player, ball, yOfImpact);
}

void ai::AIPlayMedium(Player& player, Ball& ball) {
  float yBall = ball.shape.getPosition().y;
  followCoordinate(player, ball, yBall);
}

void ai::AIPlayEasy(Player& player, Ball& ball) {
  const int probabilityOfMovement = 2;
  int option = rand() % probabilityOfMovement;

  if (!option) AIPlayMedium(player, ball);
  else player.goingUp = player.goingDown = false;
}

void ai::AIPlay(Player& player, Ball& ball, int difficulty) {
  if (player.isAI == false) return;

  switch (difficulty) {
  case EASY:
    AIPlayEasy(player, ball);
    break;

  case MEDIUM:
    AIPlayMedium(player, ball);
    break;

  case HARD:
    AIPlayHard(player, ball);
    break;
  }
}