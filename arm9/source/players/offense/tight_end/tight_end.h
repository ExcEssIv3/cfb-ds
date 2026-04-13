#pragma once

#include "../offensive_player.h"

class TightEnd : public OffensivePlayer {
public:
    TightEnd(Vector2 pos, int size, float speed)
        : OffensivePlayer(pos, size, speed, Position::TIGHT_END) {};
};
