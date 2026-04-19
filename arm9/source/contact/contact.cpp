#include <stdio.h>
#include <nds.h>
#include "contact.h"

bool Contact::tackle(Player *tackler, Player *carrier)
{
    if (tackler->hasStatus(Player::Status::STUMBLED)) return false;
    // Tackle considerations:
    // breakTackle
    // tackle
    // weight difference
    // speed

    float tackleOdds = baseTackleOdds;

    tackleOdds += ((tackler->stats.tackle - carrier->stats.breakTackle) / 100.0f) * tackleBreakWeight;
    tackleOdds += ((tackler->stats.weight - carrier->stats.weight) / 100.0f) * tackleMassWeight;
    float normalizedSpeed = magnitude(tackler->velocity - carrier->velocity) / carrier->stats.topSpeed;

    tackleOdds += normalizedSpeed * tackleSpeedWeight;

    char buf[64];
    sprintf(buf, "Tackle: %d\n", (int)(tackleOdds * 1000));
    nocashWrite(buf, strlen(buf));

    if (tackleOdds < minTackleOdds) tackleOdds = minTackleOdds;
    if (tackleOdds > maxTackleOdds) tackleOdds = maxTackleOdds;

    return (rand() % 100) < (int)(tackleOdds * 100);
}

Contact::BlockResult Contact::block(Player *blocker, Player *rusher)
{
    if (blocker->hasStatus(Player::Status::STUMBLED)) return BlockResult::BEAT_BLOCK;

    // Pancake odds — driven by momentum differential (mass * speed toward each other)
    float blockerMomentum = blocker->stats.weight * magnitude(blocker->velocity);
    float rusherMomentum  = rusher->stats.weight  * magnitude(rusher->velocity);
    float maxMomentum     = (blocker->stats.weight + rusher->stats.weight) * rusher->stats.topSpeed;

    float rusherPancakeOdds  = 0.0f;
    float blockerPancakeOdds = 0.0f;
    if (maxMomentum > 0.0f) {
        float delta = (blockerMomentum - rusherMomentum) / maxMomentum;
        rusherPancakeOdds  =  delta * blockMassWeight + magnitude(blocker->velocity) / blocker->stats.topSpeed * blockSpeedWeight;
        blockerPancakeOdds = -delta * blockMassWeight + magnitude(rusher->velocity)  / rusher->stats.topSpeed  * blockSpeedWeight;
        if (rusherPancakeOdds  < 0.0f) rusherPancakeOdds  = 0.0f;
        if (blockerPancakeOdds < 0.0f) blockerPancakeOdds = 0.0f;
    }

    // Beat block odds — driven by blockShed vs block stat matchup
    float beatBlockOdds = baseBeatBlockOdds + ((rusher->stats.blockShed - blocker->stats.block) / 100.0f) * blockShedWeight;
    if (beatBlockOdds < 0.0f) beatBlockOdds = 0.0f;

    // Normalize if combined odds exceed 1
    float total = rusherPancakeOdds + blockerPancakeOdds + beatBlockOdds;
    if (total > 1.0f) {
        rusherPancakeOdds  /= total;
        blockerPancakeOdds /= total;
        beatBlockOdds      /= total;
    }

    // Single roll, compare against ranges
    float roll = (rand() % 1000) / 1000.0f;
    if (roll < rusherPancakeOdds)                                    return BlockResult::RUSHER_PANCAKE;
    if (roll < rusherPancakeOdds + blockerPancakeOdds)               return BlockResult::BLOCKER_PANCAKE;
    if (roll < rusherPancakeOdds + blockerPancakeOdds + beatBlockOdds) return BlockResult::BEAT_BLOCK;
    return BlockResult::ENGAGED;
}
