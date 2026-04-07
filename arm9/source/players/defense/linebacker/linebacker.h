#pragma once

#include "../defensive_player.h"

class Linebacker : public DefensivePlayer {
public:
    Linebacker(float x, float y, uint16_t color, int size, float speed)
        : DefensivePlayer(x, y, color, size, speed) {};
};
