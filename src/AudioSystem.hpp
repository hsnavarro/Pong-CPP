#include "sfml.hpp"

#include "GameSound.hpp"

const std::string BEEP_SOUND_FILE = "./audio/beep-sound-enhanced.wav";
const std::string WALL_SOUND_FILE = "./audio/wall-sound.ogg";
const std::string POINT_SOUND_FILE = "./audio/point-sound.wav";
const std::string WIN_SOUND_FILE = "./audio/win-sound.wav";

class AudioSystem {
public:
  GameSound beepSound{ BEEP_SOUND_FILE };
  GameSound wallSound{ WALL_SOUND_FILE };
  GameSound pointSound{ POINT_SOUND_FILE };
  GameSound winSound{ WIN_SOUND_FILE };
};