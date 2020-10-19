#pragma once

#include "sfml.hpp"

#include "GameText.hpp"

const std::string FONT_FILE = "./fonts/main-font.ttf";

class Game;

class UI {
public:
  GameText scoreText{ FONT_FILE };
  GameText winText{ FONT_FILE };
  GameText pauseText{ FONT_FILE };
  GameText slowMotionText{ FONT_FILE };
  GameText fastMotionText{ FONT_FILE };

  Game& game;

public:
  UI(Game&);
  void render();

private:
  void draw(const GameText&);

};