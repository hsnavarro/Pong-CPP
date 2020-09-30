#pragma once

#include "sfml.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "GameWindow.hpp"

namespace physics {
  bool detectRectangleIntersection(sf::RectangleShape&, sf::RectangleShape&);
  bool detectAndFixPlayerCollisionBall(sf::Vector2f&, Ball&, sf::Vector2f&, Player&, float);
  bool detectAndFixWallCollisionPlayer(Player&, GameWindow&);
  bool detectAndFixWallCollisionBall(Ball&, GameWindow&);
}