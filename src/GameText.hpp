#pragma once

#include "sfml.hpp"

class GameText {
public:
  sf::Font font;
  sf::Text text;

  GameText(std::string);

  void centralizeText();
  void setString(std::string);
  std::string stringifyScore(int, int);
};