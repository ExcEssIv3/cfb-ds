#pragma once

#include "../players/player.h"

namespace Contact {
    constexpr float baseOdds            = 0.5f;
    constexpr float minOdds             = 0.3f;
    constexpr float maxOdds             = 0.95f;

    constexpr float breakTackleWeight   = 0.3f;   // normalized -1..+1 * this → max ±30% odds
    constexpr float weightDeltaWeight   = 0.15f;  // normalized -1..+1 * this → max ±15% odds
    constexpr float speedWeight         = 0.1f;   // normalized  0..1  * this → max +10% odds

    // returns true if tackle succeeds
    bool tackle(Player* tackler, Player* tacklee);
}