#include "GameText.hpp"
#include "setupInfo.hpp"

const int FONT_SIZE = 14;
const int SCOREBOARD_HEIGHT = 0.1 * setupInfo::SCREEN_HEIGHT;


GameText::GameText(std::string fileName) {
  if (font.loadFromFile(fileName)) {
    text.setFont(font);
    text.setCharacterSize(FONT_SIZE);
  }
}

std::string GameText::stringifyScore(int pointsPlayer1, int pointsPlayer2) {
  return std::to_string(pointsPlayer1) + " - " + std::to_string(pointsPlayer2);
}

void GameText::centralizeText() {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.width / 2.0, textRect.height / 2.0);
  text.setPosition(setupInfo::SCREEN_WIDTH / 2.0, SCOREBOARD_HEIGHT);
}

void GameText::setString(std::string newText) {
  text.setString(newText);
  centralizeText();
}