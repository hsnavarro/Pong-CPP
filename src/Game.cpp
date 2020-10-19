#include "Game.hpp"

void Game::setup() {
  ui.scoreText.setString(ui.scoreText.stringifyScore(0, 0), DisplayMode::HORIZONTAL_CENTER);
  players.push_back({ setupInfo::player1InitialPosition });
  players.push_back({ setupInfo::player2InitialPosition, true });

  ui.pauseText.setString("Pause", DisplayMode::RIGHT);
  ui.slowMotionText.setString("Slow", DisplayMode::LEFT);
  ui.fastMotionText.setString("Fast", DisplayMode::LEFT);
}

void Game::resetPosition() {
  ball.resetPosition();
  for (auto& player : players) player.resetPosition();
}

void Game::updateScoreBoard() {
  ui.scoreText.setString(ui.scoreText.stringifyScore(players[0].points, players[1].points), DisplayMode::HORIZONTAL_CENTER);
}

void Game::checkMatchEnd() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (players[i].points == setupInfo::MAX_PTS) {

      ui.winText.setString("Player " + std::to_string(i + 1) + " wins!", DisplayMode::ALL_CENTER);

      ball.shape.setFillColor(sf::Color::Black);
      isMatchEnd = true;

      audio.winSound.play(timeScale);
      return;
    }
  }
}

void Game::processPoint() {
  audio.pointSound.play(timeScale);
  updateScoreBoard();
  resetPosition();
  checkMatchEnd();
}

void Game::detectAndHandlePoint() {
  sf::Vector2f ballPosition = ball.shape.getPosition();
  float ballRadius = ball.shape.getRadius();

  sf::Vector2u screenSize = window.getSize();
  float screenWidth = screenSize.x;

  if (ballPosition.x + 2 * ballRadius >= screenWidth) {
    players[0].points++;
    processPoint();
  }

  if (ballPosition.x <= 0) {
    players[1].points++;
    processPoint();
  }
}

void Game::simulateMotion(const float timeElapsed) {
  ball.move(timeElapsed);

  for (auto& player : players) {
    const sf::Vector2f previousPosition = player.shape.getPosition();
    player.move(timeElapsed);
    player.velocity = (player.shape.getPosition() - previousPosition) / timeElapsed;
  }
}

void Game::handleCollisions(const float timeScale) {
  if (physics::ballToWallCollision(ball, window)) {
    audio.wallSound.play(timeScale);
  }

  for (auto& player : players) {
    physics::playerToWallCollision(player, window);

    if (physics::ballToPlayerCollision(ball, player)) {
      audio.beepSound.play(timeScale);
      //isPaused = true;
    }
  }
}

void Game::updateSimulation(float timeElapsed) {
  if (isMatchEnd) return;

  assert(timeScale != 0.0);

  if (isPaused) return;

  timeElapsed *= timeScale;

  simulateMotion(timeElapsed);
  handleCollisions(timeScale);

  for (auto& player : players) ai::AIPlay(player, ball);

  detectAndHandlePoint();
}

void Game::render() {
  window.clear();

  for (auto& player : players) window.draw(player.shape);
  window.draw(ball.shape);

  ui.render();

  window.display();
}

void Game::handleEvents() {
  sf::Event event;

  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window.close();

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::P) isPaused ^= 1;

      if (event.key.code == sf::Keyboard::N) {
        if (isFastMotion) {
          timeScale = 1.0;
          isFastMotion = false;
        } else {
          isFastMotion = true;
          isSlowMotion = false;
          timeScale = 1.5;
        }
      }

      if (event.key.code == sf::Keyboard::M) {
        if (isSlowMotion) {
          timeScale = 1.0;
          isSlowMotion = false;
        } else {
          isSlowMotion = true;
          isFastMotion = false;
          timeScale = 0.5;
        }
      }
    }

    if (event.type == sf::Event::KeyPressed or event.type == sf::Event::KeyReleased) {

      bool updateMovement = (event.type == sf::Event::KeyPressed);

      for (int i = 0; i < NUM_PLAYERS; i++) {
        Player& player = players[i];
        if (!player.isAI) {
          if (event.key.code == MOVE[i][UP]) player.goingUp = updateMovement;
          if (event.key.code == MOVE[i][DOWN]) player.goingDown = updateMovement;
        }
      }

    }

  }
}

void Game::update(const float frameTime) {
  float elapsedTime = frameTime;

  while (elapsedTime > 0.0f) {
    float simulationTime = std::min(elapsedTime, static_cast<float>(setupInfo::SIMULATION_FRAME_RATE));
    updateSimulation(simulationTime);
    elapsedTime -= simulationTime;
  }

  render();

  handleEvents();
}

Game::Game() {
  setup();
}

void Game::run() {
  while(window.isOpen()) {
    const float frameTime = clock.restart().asSeconds();
    update(frameTime);
  }
}