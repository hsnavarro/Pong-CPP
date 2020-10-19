#pragma once

#include "sfml.hpp"

#include "Ball.hpp"
#include "GameWindow.hpp"
#include "Player.hpp"

namespace physics {
  bool ballToPlayerCollision(Ball&, Player&);
  bool playerToWallCollision(Player&, GameWindow&);
  bool ballToWallCollision(Ball&, GameWindow&);
}