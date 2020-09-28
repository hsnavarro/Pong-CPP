#include "GameWindow.hpp"

const int ANTI_ALIASING_LEVEL = 8;
const int FRAME_RATE = 300;

GameWindow::GameWindow(float width, float height) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(width, height), "Pong Game", sf::Style::Default, settings);
  window.setFramerateLimit(FRAME_RATE);
}

sf::Vector2u GameWindow::getSize() { return window.getSize(); }

void GameWindow::clear() { window.clear(sf::Color::Black); };

void GameWindow::draw(sf::Shape& shape) { window.draw(shape); };

void GameWindow::display() { window.display(); }

bool GameWindow::pollEvent(sf::Event& event) { 
  return window.pollEvent(event);
}

bool GameWindow::isOpen() { return window.isOpen(); }

void GameWindow::close() { window.close(); }