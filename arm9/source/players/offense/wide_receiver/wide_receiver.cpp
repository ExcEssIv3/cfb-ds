#include "wide_receiver.h"
#include "../../../utils.h"

void WideReceiver::runAI(const GameContext& ctx) {
    OffensivePlayer::runAI(ctx);
    if (!hasStatus(Status::BALL_CARRIER)) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            goTo({ (float)convertToPixelYards(100), pos.y });
        } else if (ctx.football->hasStatus(Football::Status::FLYING)) {
            Vector2 intercept = interceptPoint(
                ctx.football->pos,
                ctx.football->destination,
                ctx.football->speed,
                pos,
                speed
            );
            if (intercept != Vector2({ -1.0f, -1.0f })) {
                goTo(intercept);
            } else goTo({ (float)convertToPixelYards(100), pos.y });
        }
    }

    return;
}