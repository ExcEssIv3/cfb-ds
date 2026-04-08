#pragma once

#include "../defensive_player.h"

class DefensiveLine : public DefensivePlayer {
public:
    DefensiveLine(float x, float y, int size, float speed)
        : DefensivePlayer(x, y, size, speed) {};
};
