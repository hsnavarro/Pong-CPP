#include "setupInfo.hpp"

#include "GameSound.hpp"

GameSound::GameSound(std::string fileName) {
  if (soundBuffer.loadFromFile(fileName))
    sound.setBuffer(soundBuffer);
}

void GameSound::play(const double scale) {
  sound.setPitch(scale);
  sound.play();
}