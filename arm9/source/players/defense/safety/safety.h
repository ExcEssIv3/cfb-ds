#pragma once

#include "../defensive_player.h"

class Safety : public DefensivePlayer {
public:
    Safety(float x, float y, int size, float speed)
        : DefensivePlayer(x, y, size, speed) {};
};
