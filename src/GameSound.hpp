#pragma once

#include "sfml.hpp"

class GameSound {
private:
  sf::SoundBuffer soundBuffer;
  sf::Sound sound;

public:
  GameSound(std::string);
  void play();
};