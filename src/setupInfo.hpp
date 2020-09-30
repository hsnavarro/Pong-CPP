#pragma once

#include "sfml.hpp"

namespace setupInfo {
  const int BALL_RADIUS = 5;
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 400;
  const int RECTANGLE_WIDTH = 0.01 * SCREEN_WIDTH;
  const int RECTANGLE_HEIGHT = 0.2 * SCREEN_HEIGHT;
  const int FRAME_RATE = 100;
  const sf::Vector2f ballInitialPosition = { 0.5 * SCREEN_WIDTH - BALL_RADIUS, 0.5 * SCREEN_HEIGHT - BALL_RADIUS };
  const sf::Vector2f player1InitialPosition = { 0.1 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT };
  const sf::Vector2f player2InitialPosition = { 0.9 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT,};
  const sf::Vector2f ballInitialVelocity = { 300, 0 };
  const sf::Vector2f playerInitialVelocity = { 0, 400 };
  const int MAX_PTS = 10;
}