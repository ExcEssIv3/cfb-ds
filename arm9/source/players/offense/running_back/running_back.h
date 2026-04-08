#pragma once

#include "../offensive_player.h"

class RunningBack : public OffensivePlayer {
public:
    RunningBack(float x, float y, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, size, speed, hasBall) {};
};
