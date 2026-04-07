#pragma once

#include "../offensive_player.h"

class OffensiveLine : public OffensivePlayer {
public:
    OffensiveLine(float x, float y, uint16_t color, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, color, size, speed, hasBall) {};
};
