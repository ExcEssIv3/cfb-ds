#pragma once

#include "../player.h"

class Football;

class DefensivePlayer : public Player {
public:
    bool hasBall;

    DefensivePlayer(float x, float y, int size, float speed, bool hasBall = false)
        : Player(x, y, size, speed), hasBall(hasBall) {};

    void runAI(Football* football);
};
