#pragma once

#include "../offensive_player.h"

class WideReceiver : public OffensivePlayer {
public:
    WideReceiver(float x, float y, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, size, speed, hasBall) {};
};
