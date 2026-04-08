#pragma once

#include "../offensive_player.h"

class OffensiveLine : public OffensivePlayer {
public:
    OffensiveLine(float x, float y, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, size, speed, hasBall) {};
};
