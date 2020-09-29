#pragma once

#include "sfml.hpp"
#include "setupInfo.hpp"

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity = setupInfo::ballInitialVelocity;

    Ball(float = 0, float = 0);
    void move(float);
};