#pragma once

#include "../player.h"

class DefensivePlayer : public Player {
public:
    DefensivePlayer(float x, float y, uint16_t color, int size, float speed, bool userControlled)
        : Player(x, y, color, size, speed) {};

    void runAI();
};
