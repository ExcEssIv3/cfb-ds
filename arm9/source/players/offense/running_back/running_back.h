#pragma once

#include "../offensive_player.h"

class RunningBack : public OffensivePlayer {
public:
    RunningBack(Vector2 pos, int size, float speed)
        : OffensivePlayer(pos, size, speed, Position::RUNNING_BACK) {};
};
