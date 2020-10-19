#include "setupInfo.hpp"

#include "GameWindow.hpp"

const int ANTI_ALIASING_LEVEL = 8;

GameWindow::GameWindow(float width, float height) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
  window.create(sf::VideoMode(width, height), "Pong Game", sf::Style::Default, settings);
  window.setFramerateLimit(setupInfo::RENDER_FRAME_RATE);
}

sf::Vector2u GameWindow::getSize() { return window.getSize(); }

void GameWindow::clear() { window.clear(); };

void GameWindow::draw(const sf::Drawable& drawable) { window.draw(drawable); };

void GameWindow::display() { window.display(); }

bool GameWindow::pollEvent(sf::Event& event) { 
  return window.pollEvent(event);
}

bool GameWindow::isOpen() { return window.isOpen(); }

void GameWindow::close() { window.close(); }