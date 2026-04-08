#pragma once

#include "../defensive_player.h"

class Linebacker : public DefensivePlayer {
public:
    Linebacker(float x, float y, int size, float speed)
        : DefensivePlayer(x, y, size, speed) {};
};
