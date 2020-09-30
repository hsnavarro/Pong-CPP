#pragma once

#include "sfml.hpp"
#include "setupInfo.hpp"

class Player {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity = setupInfo::playerInitialVelocity;
    bool goingUp = false;
    bool goingDown = false;
    bool isAI;
    sf::Vector2f initialPosition;
    int points = 0;

    Player(sf::Vector2f = {0, 0}, bool = false);
    void move(float);
    void resetPosition();
};