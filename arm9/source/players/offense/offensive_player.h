#pragma once

#include "../player.h"
#include "../../football/football.h"

class OffensivePlayer : public Player {
public:
    bool hasBall;

    OffensivePlayer(float x, float y, int size, float speed, bool hasBall)
        : Player(x, y, size, speed), hasBall(hasBall) {};

    virtual void runAI(Football* football);
};
