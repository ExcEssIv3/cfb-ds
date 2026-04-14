#pragma once

#include "../defensive_player.h"

class Linebacker : public DefensivePlayer {
public:
    // Bits 8-15: linebacker-specific
    enum class Status : uint16_t {
        ZONE_COVERAGE = 1 << 8,
        MAN_COVERAGE  = 1 << 9,
        BLITZ         = 1 << 10,
    };

    Linebacker(Vector2 pos, int size, float speed)
        : DefensivePlayer(pos, size, speed, Position::LINEBACKER) {};

    void runAI(const GameContext& ctx) override;
};
