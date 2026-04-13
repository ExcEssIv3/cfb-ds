#pragma once

#include <nds.h>
#include "../utils.h"

class Football;

class Player {
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
    int size;
    float speed;
    bool isOffense;
    Position position;
    uint16_t statusFlags;

    Player(Vector2 pos, int size, float speed, bool isOffense, Position position, uint16_t statusFlags = 0);
    virtual void runAI(Football* football, Player* ballCarrier);
    void move(float direction);
    void goTo(Vector2 target);
    template<typename T> void setStatus(T flag)   { statusFlags |=  static_cast<uint16_t>(flag); }
    template<typename T> void clearStatus(T flag) { statusFlags &= ~static_cast<uint16_t>(flag); }
    template<typename T> bool hasStatus(T flag)   { return statusFlags & static_cast<uint16_t>(flag); }
    void resetStatus();
};
