#pragma once

#include "../offensive_player.h"

class WideReceiver : public OffensivePlayer {
public:
    uint32_t button;
    WideReceiver(Vector2 pos, int size, float speed, uint32_t button, float catchRadius)
        : OffensivePlayer(pos, size, speed, Position::WIDE_RECEIVER, catchRadius), button(button) {};

};
