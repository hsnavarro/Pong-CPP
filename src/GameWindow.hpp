#pragma once

#include "sfml.hpp"

class GameWindow {
private:
  sf::RenderWindow window;

public:
  GameWindow(float, float);

  sf::Vector2u getSize();
  
  void clear();
  void draw(sf::Drawable&);
  void display();
  void close();
  bool isOpen();

  bool pollEvent(sf::Event&);
};