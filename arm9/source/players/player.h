#pragma once

#include <nds.h>
#include "../utils.h"
#include "../status_mixin.h"
#include "../game_context.h"
#include "player_stats.h"

struct Behavior;

class Player : public StatusMixin {
public:
    enum class Position : uint8_t {
        QUARTERBACK,
        RUNNING_BACK,
        WIDE_RECEIVER,
        TIGHT_END,
        OFFENSIVE_LINE,
        CORNERBACK,
        LINEBACKER,
        SAFETY,
        DEFENSIVE_LINE,
    };

    // Bits 0-7: shared across all players
    enum class Status : uint16_t {
        BALL_CARRIER = 1 << 0,
    };

    Vector2 pos;
    bool isOffense;
    Position position;
    PlayerStats stats;
    Behavior* behavior = nullptr;

    Player(Vector2 pos, bool isOffense, Position position, PlayerStats stats = {}, uint16_t statusFlags = 0);
    virtual void runAI(const GameContext& ctx);
    void move(float direction);
    void goTo(Vector2 target);
};
