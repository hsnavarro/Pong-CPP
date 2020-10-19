#pragma once

#include "sfml.hpp"

enum class DisplayMode { LEFT, RIGHT, HORIZONTAL_CENTER, VERTICAL_CENTER, ALL_CENTER };

class GameText {
private:
  sf::Font font;

public:
  sf::Text text;

  GameText(std::string);

  void center();
  void left();
  void right();
  void verticalCenter();

  void setString(std::string, DisplayMode);
  std::string stringifyScore(int, int);
};