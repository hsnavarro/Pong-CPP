#pragma once

#include "setupInfo.hpp"
#include "sfml.hpp"

class Player {
public:
    sf::RectangleShape shape;
    sf::Vector2f movementVelocity = setupInfo::playerInitialVelocity;
    sf::Vector2f velocity;
    bool goingUp = false;
    bool goingDown = false;
    bool isAI;
    sf::Vector2f initialPosition;
    int points = 0;

    Player(sf::Vector2f = { 0, 0 }, bool = false);
    void move(float);
    void resetPosition();
};