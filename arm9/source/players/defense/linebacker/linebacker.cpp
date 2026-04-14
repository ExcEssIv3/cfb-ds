#include "linebacker.h"
#include "../../../football/football.h"

void Linebacker::runAI(const GameContext& ctx) {
    DefensivePlayer::runAI(ctx);
    if (!ctx.football->hasStatus(Football::Status::FUMBLED)) {
        if (ctx.football->hasStatus(Football::Status::FLYING)) {
            // TODO: Try to intercept if possible
            goTo(ctx.football->destination);
        } else {
            if (ctx.ballCarrier->position == Player::Position::QUARTERBACK) {
                if (hasStatus(Linebacker::Status::BLITZ)) {
                    goTo(ctx.ballCarrier->pos);
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
            } else if (ctx.ballCarrier->isOffense) {
                goTo(ctx.ballCarrier->pos);
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