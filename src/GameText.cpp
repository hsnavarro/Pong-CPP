#include "setupInfo.hpp"

#include "GameText.hpp"

const int FONT_SIZE = 14;

const float TEXT_HEIGHT = 0.1 * setupInfo::SCREEN_HEIGHT;
const float TEXT_LEFT = 0.2 * setupInfo::SCREEN_WIDTH;
const float TEXT_RIGHT = 0.8 * setupInfo::SCREEN_WIDTH;

GameText::GameText(std::string fileName) {
  if (font.loadFromFile(fileName)) {
    text.setFont(font);
    text.setCharacterSize(FONT_SIZE);
  }
}

void GameText::center() {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(0.5f * textRect.width, 0.5f * textRect.height);
  text.setPosition(0.5f * setupInfo::SCREEN_WIDTH, TEXT_HEIGHT);
}

void GameText::left() {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(0.5f * textRect.width, 0.5f * textRect.height);
  text.setPosition(TEXT_LEFT, TEXT_HEIGHT);
}

void GameText::right() {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(0.5f * textRect.width, 0.5f * textRect.height);
  text.setPosition(TEXT_RIGHT, TEXT_HEIGHT);
}

void GameText::verticalCenter() {
  text.setPosition(text.getPosition().x, 0.5f * setupInfo::SCREEN_HEIGHT);
}

void GameText::setString(std::string newText, DisplayMode display) {
  text.setString(newText);
  
  switch (display) {
    case DisplayMode::LEFT:
      left();
      break;

    case DisplayMode::HORIZONTAL_CENTER:
      center();
      break;

    case DisplayMode::RIGHT:
      right();
      break;

    case DisplayMode::VERTICAL_CENTER:
      verticalCenter();
      break;

    case DisplayMode::ALL_CENTER:
      center();
      verticalCenter();
      break;
  }

}

std::string GameText::stringifyScore(int pointsPlayer1, int pointsPlayer2) {
  return std::to_string(pointsPlayer1) + " - " + std::to_string(pointsPlayer2);
}