#include "UI.hpp"
#include "Game.hpp"

UI::UI(Game& game) : game{ game } {}

void UI::render() {
  draw(scoreText);
  if (game.isMatchEnd) draw(winText);
  else {
    if (game.isFastMotion) draw(fastMotionText);
    if (game.isSlowMotion) draw(slowMotionText);
    if (game.isPaused) draw(pauseText);
  }
}

void UI::draw(const GameText& gameText) {
  game.window.draw(gameText.text);
}