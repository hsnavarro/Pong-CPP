#pragma once

#include "sfml.hpp"

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity = { 200, 0 };

    Ball(float = 0, float = 0);
    void move(float);
};