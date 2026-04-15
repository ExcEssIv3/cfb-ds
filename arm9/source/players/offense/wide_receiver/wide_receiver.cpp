#include "wide_receiver.h"
#include "../../../utils.h"

void WideReceiver::runAI(const GameContext& ctx) {
    OffensivePlayer::runAI(ctx);
    if (!hasStatus(Status::BALL_CARRIER)) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            goTo({ (float)convertToPixelYards(100), pos.y });
        } else if (ctx.football->hasStatus(Football::Status::FLYING)) {
            if (ctx.football->pos == pos) {
                ctx.football->resetStatus();
                ctx.football->setStatus(Football::Status::HIDDEN);
                setStatus(Status::BALL_CARRIER);
            } else {
                Vector2 intercept = interceptPoint(
                    ctx.football->pos,
                    ctx.football->destination,
                    ctx.football->speed,
                    pos,
                    speed
                );
                if (false && intercept != Vector2({ -1.0f, -1.0f })) {
                    goTo(intercept);
                } else goTo({ (float)convertToPixelYards(100), pos.y });
            }
        }
    }

    return;
}