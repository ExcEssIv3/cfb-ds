#pragma once

#include"../offensive_player.h"

class Quarterback : public OffensivePlayer {
public:

    Quarterback(Vector2 pos, int size, float speed)
        : OffensivePlayer(pos, size, speed, Position::QUARTERBACK) {};

    void runAI(Football* football, Player* ballCarrier) override;
};