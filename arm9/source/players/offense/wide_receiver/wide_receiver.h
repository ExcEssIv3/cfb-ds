#pragma once

#include "../offensive_player.h"

class WideReceiver : public OffensivePlayer {
public:
    WideReceiver(Vector2 pos, int size, float speed)
        : OffensivePlayer(pos, size, speed, Position::WIDE_RECEIVER) {};
};
