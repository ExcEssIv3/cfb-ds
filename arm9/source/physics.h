#pragma once

#include "players/player.h"

namespace Physics {
    void move(Player* player, float vx, float vy);
    float distance(Player* a, Player* b);
}
