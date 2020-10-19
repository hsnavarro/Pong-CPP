#pragma once

#include "setupInfo.hpp"
#include "sfml.hpp"

#include "Ball.hpp"
#include "Player.hpp"

namespace ai {
  void AIPlay(Player&, Ball&, int = setupInfo::AI_DIFFICULTY);
  void followCoordinate(Player&, Ball&, float);
  void AIPlayHard(Player&, Ball&);
  void AIPlayMedium(Player&, Ball&);
  void AIPlayEasy(Player&, Ball&);
}