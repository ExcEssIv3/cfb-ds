#include "linebacker.h"
#include "../../../football/football.h"

void Linebacker::runAI(Football* football, Player* ballCarrier) {
    DefensivePlayer::runAI(football, ballCarrier);
    if (football->state != FootballState::FUMBLED) {
        if (football->state == FootballState::FLYING) {
            // TODO: Try to intercept if possible
            goTo(football->destination);
        } else {
            if (ballCarrier->position == Player::Position::QUARTERBACK) {
                if (hasStatus(Linebacker::Status::BLITZ)) {
                    goTo(ballCarrier->pos);
                } else if (hasStatus(Linebacker::Status::MAN_COVERAGE)) {
                    // TODO: implement coverages
                    return;
                } else if (hasStatus(Linebacker::Status::ZONE_COVERAGE)) {
                    // TODO: implement coverages
                    return;
                } else {
                    // THIS SHOULD NEVER HAPPEN
                    // TODO: implement error handling
                    return;
                }
            } else if (ballCarrier->isOffense) {
                goTo(ballCarrier->pos);
                // TODO: implement tackling
                return;
            } else {
                // TODO: implement blocking
                return;
            }
        }
    }

    return;
}