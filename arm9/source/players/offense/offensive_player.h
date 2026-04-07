#pragma once

#include "../player.h"

class OffensivePlayer : public Player {
public:
    bool userControlled;
    bool hasBall;

    OffensivePlayer(float x, float y, uint16_t color, int size, float speed, bool userControlled)
        : Player(x, y, color, size, speed), userControlled(userControlled), hasBall(false) {};

    void runAI();
};
