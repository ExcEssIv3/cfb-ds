#pragma once

#include "../player.h"

class OffensivePlayer : public Player {
public:
    bool hasBall;

    OffensivePlayer(float x, float y, uint16_t color, int size, float speed, bool hasBall)
        : Player(x, y, color, size, speed), hasBall(hasBall) {};

    void runAI();
};
