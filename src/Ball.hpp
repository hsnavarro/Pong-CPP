#pragma once

#include "sfml.hpp"
#include "setupInfo.hpp"

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity = setupInfo::ballInitialVelocity;
    sf::Vector2f initialPosition;

    Ball(sf::Vector2f = setupInfo::ballInitialPosition);
    void move(float);
    void resetPosition();
};