#pragma once

#include "../player.h"

class Football;

class DefensivePlayer : public Player {
public:
    DefensivePlayer(Vector2 pos, int size, float speed, Position position)
        : Player(pos, size, speed, false, position) {};

    void runAI(const GameContext& ctx) override;
};
