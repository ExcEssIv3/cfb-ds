#include <stdio.h>
#include <nds.h>
#include "contact.h"

bool Contact::tackle(Player *tackler, Player *tacklee)
{
    if (tackler->hasStatus(Player::Status::STUMBLED)) return false;
    // Tackle considerations:
    // breakTackle
    // tackle
    // weight difference
    // speed

    float tackleOdds = baseOdds;

    tackleOdds += ((tackler->stats.tackle - tacklee->stats.breakTackle) / 100.0f) * breakTackleWeight;
    tackleOdds += ((tackler->stats.weight - tacklee->stats.weight) / 100.0f) * weightDeltaWeight;
    float normalizedSpeed = magnitude(tackler->velocity - tacklee->velocity) / tacklee->stats.topSpeed;

    tackleOdds += normalizedSpeed * speedWeight;
    
    char buf[64];
    sprintf(buf, "Tackle: %d\n", (int)(tackleOdds * 1000));
    nocashWrite(buf, strlen(buf));

    if (tackleOdds < minOdds) tackleOdds = minOdds;
    if (tackleOdds > maxOdds) tackleOdds = maxOdds;

    return (rand() % 100) < (int)(tackleOdds * 100);
}
