#pragma once

#include "../player.h"
#include "../../football/football.h"

class OffensivePlayer : public Player {
public:
    OffensivePlayer(Vector2 pos, int size, float speed, Position position)
        : Player(pos, size, speed, true, position) {};

    void runAI(const GameContext& ctx) override;
};
