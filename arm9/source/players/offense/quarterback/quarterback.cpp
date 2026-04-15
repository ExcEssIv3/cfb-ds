#include <nds.h>
#include "quarterback.h"
#include "../../../utils.h"

void Quarterback::runAI(const GameContext& ctx) {
    OffensivePlayer::runAI(ctx);
    if (hasStatus(Status::BALL_CARRIER)) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            uint32_t keys = keysHeld();

            uint32_t availableKeys = 0;
            for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
                availableKeys |= ctx.offensePlay.passCatchers[i].button;
            }

            if (keys & availableKeys) {
                ctx.football->start = pos;
                ctx.football->pos = pos;
                ctx.football->speed = 2.8;
                ctx.football->t = 0.0f;
                ctx.football->resetStatus();
                ctx.football->setStatus(Football::Status::FLYING);
                clearStatus(Status::BALL_CARRIER);
                for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
                    if (ctx.offensePlay.passCatchers[i].button & keys) ctx.football->destination = interceptPoint(
                        ctx.offensePlay.passCatchers[i].player->pos,
                        { (float)convertToPixelYards(100), ctx.offensePlay.passCatchers[i].player->pos.y },
                        ctx.offensePlay.passCatchers[i].player->speed,
                        pos,
                        ctx.football->speed
                    );
                }
                return;
            }
        }
    }
}
