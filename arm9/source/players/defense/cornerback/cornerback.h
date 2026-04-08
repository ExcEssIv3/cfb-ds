#pragma once

#include "../defensive_player.h"

class Cornerback : public DefensivePlayer {
public:
    Cornerback(float x, float y, int size, float speed)
        : DefensivePlayer(x, y, size, speed) {};
};
