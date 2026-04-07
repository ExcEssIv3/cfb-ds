#pragma once

#include "../offensive_player.h"

class WideReceiver : public OffensivePlayer {
public:
    WideReceiver(float x, float y, uint16_t color, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, color, size, speed, hasBall) {};
};
