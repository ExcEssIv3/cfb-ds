#pragma once

#include"../offensive_player.h"

class Quarterback : public OffensivePlayer {
public:

    Quarterback(float x, float y, int size, float speed, bool hasBall)
        : OffensivePlayer(x, y, size, speed, hasBall) {};
};