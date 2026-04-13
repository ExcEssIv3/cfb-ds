#pragma once

#include "../defensive_player.h"

class Cornerback : public DefensivePlayer {
public:
    Cornerback(Vector2 pos, int size, float speed)
        : DefensivePlayer(pos, size, speed, Position::CORNERBACK) {};
};
