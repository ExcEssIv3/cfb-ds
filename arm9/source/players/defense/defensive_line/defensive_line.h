#pragma once

#include "../defensive_player.h"

class DefensiveLine : public DefensivePlayer {
public:
    DefensiveLine(float x, float y, uint16_t color, int size, float speed)
        : DefensivePlayer(x, y, color, size, speed) {};
};
