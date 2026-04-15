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
                    if (ctx.offensePlay.passCatchers[i].button & keys) {
                        Vector2 dest = interceptPoint(
                            ctx.offensePlay.passCatchers[i].player->pos,
                            { (float)convertToPixelYards(100), ctx.offensePlay.passCatchers[i].player->pos.y },
                            ctx.offensePlay.passCatchers[i].player->speed,
                            pos,
                            ctx.football->speed
                        );
                        // ) + Vector2({ 3.0f, 0 }); // prevent ball landing right at receiver
                        if (dest.x == -1.0f && dest.y == -1.0f) {
                            char buf[64];
                            snprintf(buf, sizeof(buf), "QB: interceptPoint no solution (catcher %d)\n", i);
                            nocashMessage(buf);
                        }
                        ctx.football->destination = dest;
                    }
                }
                return;
            }
        } else {
            char buf[64];
            snprintf(buf, sizeof(buf), "QB: BALL_CARRIER but football not HIDDEN (flags=%d)\n", (int)ctx.football->statusFlags);
            nocashMessage(buf);
        }
    }
}
