#pragma once

#include "../offensive_player.h"

class TightEnd : public OffensivePlayer {
public:
    TightEnd(float x, float y, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, size, speed, hasBall) {};
};
