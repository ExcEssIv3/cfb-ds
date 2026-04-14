#include <nds.h>
#include "quarterback.h"
#include "../../../utils.h"

void Quarterback::runAI(const GameContext& ctx) {
    OffensivePlayer::runAI(ctx);
    if (hasStatus(Status::BALL_CARRIER)) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            uint32_t keys = keysHeld();
            if (keys & KEY_A) {
                ctx.football->start = pos;
                ctx.football->pos = pos;
                ctx.football->destination = pos + Vector2{ (float)convertToPixelYards(50), 0 };
                ctx.football->speed = 2;
                ctx.football->t = 0.0f;
                ctx.football->resetStatus();
                ctx.football->setStatus(Football::Status::FLYING);
                clearStatus(Status::BALL_CARRIER);
                return;
            }
        }
    }
}
