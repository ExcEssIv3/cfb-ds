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

    float tackleOdds = 0.7f;

    tackleOdds += (tackler->stats.tackle - tacklee->stats.breakTackle) / breakTackleWeight;
    tackleOdds += (tackler->stats.weight - tacklee->stats.weight) / weightDeltaWeight;
    float normalizedSpeed = magnitude(tackler->velocity - tacklee->velocity) / tacklee->stats.topSpeed;

    tackleOdds += normalizedSpeed / speedWeight;
    
    char buf[64];
    sprintf(buf, "Tackle: %d\n", (int)(tackleOdds * 1000));
    nocashWrite(buf, strlen(buf));

    if (tackleOdds < 0.50f) tackleOdds = 0.50f;
    if (tackleOdds > 0.95f) tackleOdds = 0.95f;

    return (rand() % 100) < (int)(tackleOdds * 100);
}
