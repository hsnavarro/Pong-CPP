#include "sfml.hpp"
#include "setupInfo.hpp"
#include "Player.hpp"
#include "Ball.hpp"

namespace ai {
  void AIPlay(Player&, Ball&, int = setupInfo::AI_DIFFICULTY);
  void followCoordinate(Player&, Ball&, float);
  void AIPlayHard(Player&, Ball&);
  void AIPlayMedium(Player&, Ball&);
  void AIPlayEasy(Player&, Ball&);
}