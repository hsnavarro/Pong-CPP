#pragma once

#include "sfml.hpp"

enum Difficulty { EASY, MEDIUM, HARD };

namespace setupInfo {
  const int BALL_RADIUS = 5;
  
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 400;
  
  const int RECTANGLE_WIDTH = 0.01 * SCREEN_WIDTH;
  const int RECTANGLE_HEIGHT = 0.2 * SCREEN_HEIGHT;
  
  const int RENDER_FRAME_RATE = 100;
  const int SIMULATION_FRAME_RATE = 2 * RENDER_FRAME_RATE;
  
  const sf::Vector2f ballInitialPosition = { 0.5 * SCREEN_WIDTH - BALL_RADIUS, 0.5 * SCREEN_HEIGHT - BALL_RADIUS };
  const sf::Vector2f ballInitialVelocity = { 300, 150 };

  const sf::Vector2f player1InitialPosition = { 0.1 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT };
  const sf::Vector2f player2InitialPosition = { 0.9 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT - 0.5 * RECTANGLE_HEIGHT,};
  const sf::Vector2f playerInitialVelocity = { 0, 400 };

  const int MAX_PTS = 5;

  const int AI_DIFFICULTY = HARD;
}