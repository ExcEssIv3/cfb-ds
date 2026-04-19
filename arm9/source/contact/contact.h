#pragma once

#include "../players/player.h"

namespace Contact {
    constexpr float baseTackleOdds       = 0.5f;
    constexpr float minTackleOdds        = 0.3f;
    constexpr float maxTackleOdds        = 0.95f;
    constexpr float tackleBreakWeight    = 0.3f;   // normalized -1..+1 * this → max ±30% odds
    constexpr float tackleMassWeight     = 0.15f;  // normalized -1..+1 * this → max ±15% odds
    constexpr float tackleSpeedWeight    = 0.1f;   // normalized  0..1  * this → max +10% odds

    constexpr float baseBeatBlockOdds    = 0.010f; // ~100 frames (~1.7s) baseline to beat an even block
    constexpr float blockShedWeight      = 0.020f; // normalized -1..+1 * this → max ±2% beat block odds per frame
    constexpr float blockMassWeight      = 0.002f; // normalized -1..+1 * this → max ±0.2% pancake odds per frame
    constexpr float blockSpeedWeight     = 0.001f; // normalized  0..1  * this → max +0.1% pancake odds per frame

    // returns true if tackle succeeds
    bool tackle(Player* tackler, Player* carrier);

    enum class BlockResult { ENGAGED, BEAT_BLOCK, BLOCKER_PANCAKE, RUSHER_PANCAKE };
    BlockResult block(Player* blocker, Player* rusher);
}
