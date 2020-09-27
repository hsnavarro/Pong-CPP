#include "GameSound.hpp"
#include "setupInfo.hpp"

GameSound::GameSound(std::string fileName) {
  if (soundBuffer.loadFromFile(fileName))
    sound.setBuffer(soundBuffer);
}

void GameSound::play() {
  sound.play();
}