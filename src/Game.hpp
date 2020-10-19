#include <assert.h> 

#include "setupInfo.hpp"
#include "sfml.hpp"

#include "ai.hpp"
#include "AudioSystem.hpp"
#include "Ball.hpp"
#include "physics.hpp"
#include "Player.hpp"
#include "GameText.hpp"
#include "GameWindow.hpp"
#include "UI.hpp"

enum Direction { UP, DOWN };

const int NUM_PLAYERS = 2;
const int NUM_DIRECTIONS = 2;

const double normalTimeScale = 1.0;

const int MOVE[NUM_PLAYERS][NUM_DIRECTIONS] = {
  { sf::Keyboard::W, sf::Keyboard::S },
  { sf::Keyboard::Up, sf::Keyboard::Down }
};

class Game {
private:
  sf::Clock clock;

  std::vector<Player>players;
  Ball ball;

  double timeScale = normalTimeScale;

public:
  bool isMatchEnd = false;

  bool isFastMotion = false;
  bool isSlowMotion = false;

  bool isPaused = false;

  GameWindow window{ setupInfo::SCREEN_WIDTH, setupInfo::SCREEN_HEIGHT };

  AudioSystem audio;

  UI ui{ *this };

private:
  void setup();

  void resetPosition();

  void updateScoreBoard();

  void checkMatchEnd();

  void processPoint();

  void detectAndHandlePoint();

  void simulateMotion(const float timeElapsed);
  void handleCollisions(const float timeScale);
  void updateSimulation(float timeElapsed);

  void render();
  void handleEvents();

  void update(const float frameTime);

public:
  Game();
  void run();

};