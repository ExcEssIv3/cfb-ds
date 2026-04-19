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
        STUMBLED     = 1 << 1,
        PANCAKED     = 1 << 2,
        BLOCKING     = 1 << 3,
    };

    Vector2 pos;
    bool isOffense;
    Position position;
    PlayerStats stats;
    Vector2 velocity = { 0.0f, 0.0f };
    Behavior* behavior = nullptr;
    int stumbleFrames = 0;

    Player(Vector2 pos, bool isOffense, Position position, PlayerStats stats = {}, uint16_t statusFlags = 0);
    virtual void runAI(const GameContext& ctx);
    void accelerate(float direction);
    void decelerate();
    void move();
    void goTo(Vector2 target);
    bool detectCollision(const Vector2& colliderPos, int width, int height);
};
