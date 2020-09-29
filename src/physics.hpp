#pragma once

#include "sfml.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "GameWindow.hpp"

namespace physics {
  bool DetectRectangleIntersection(sf::RectangleShape&, sf::RectangleShape&);
  bool DetectAndFixPlayerCollisionBall(sf::Vector2f&, Ball&, sf::Vector2f&, Player&, float);
  bool DetectAndFixWallCollisionPlayer(Player&, GameWindow&);
  bool DetectAndFixWallCollisionBall(Ball&, GameWindow&);
}