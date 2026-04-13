#pragma once

#include "../offensive_player.h"

class OffensiveLine : public OffensivePlayer {
public:
    OffensiveLine(Vector2 pos, int size, float speed)
        : OffensivePlayer(pos, size, speed, Position::OFFENSIVE_LINE) {};
};
