#pragma once

#include "../player.h"
#include "../../football/football.h"

class OffensivePlayer : public Player {
public:
    OffensivePlayer(Vector2 pos, int size, float speed, Position position, float catchRadius = 0.0f)
        : Player(pos, size, speed, true, position, catchRadius) {};

    void runAI(const GameContext& ctx) override;
};
