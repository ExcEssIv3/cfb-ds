#pragma once

#include "../player.h"

class DefensivePlayer : public Player {
public:
    bool hasBall;

    DefensivePlayer(float x, float y, uint16_t color, int size, float speed, bool hasBall = false)
        : Player(x, y, color, size, speed), hasBall(hasBall) {};

    void runAI();
};
