#pragma once

#include "sfml.hpp"

class Player {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity = { 0, 400 };
    bool goingUp = false;
    bool goingDown = false;
    bool isAI;

    Player(float = 0, float = 0, bool = false);
    void move(float);
};