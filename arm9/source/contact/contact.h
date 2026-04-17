#pragma once

#include "../players/player.h"

namespace Contact {
    constexpr float breakTackleWeight  = 200.0f;
    constexpr float weightDeltaWeight  = 500.0f;
    constexpr float speedWeight        = 200.0f;

    // returns true if tackle succeeds
    bool tackle(Player* tackler, Player* tacklee);
}