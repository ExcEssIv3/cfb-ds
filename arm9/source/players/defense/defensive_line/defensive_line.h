#pragma once

#include "../defensive_player.h"

class DefensiveLine : public DefensivePlayer {
public:
    DefensiveLine(Vector2 pos, int size, float speed)
        : DefensivePlayer(pos, size, speed, Position::DEFENSIVE_LINE) {};
};
