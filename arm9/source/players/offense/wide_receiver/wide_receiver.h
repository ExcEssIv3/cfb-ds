#pragma once

#include "../offensive_player.h"

class WideReceiver : public OffensivePlayer {
public:
    uint32_t button;
    WideReceiver(Vector2 pos, int size, float speed, uint32_t button)
        : OffensivePlayer(pos, size, speed, Position::WIDE_RECEIVER), button(button) {};

    void runAI(const GameContext& ctx) override;
};
