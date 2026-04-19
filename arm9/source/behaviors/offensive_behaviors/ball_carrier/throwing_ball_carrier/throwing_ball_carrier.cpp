#include "throwing_ball_carrier.h"
#include "../ball_carrier.h"
#include "../running_ball_carrier/running_ball_carrier.h"
#include "../../../generic_behaviors/blocking/blocking.h"
#include "../../../../players/player.h"
#include "../../../../game_context.h"
#include "../../../../football/football.h"

void ThrowingBallCarrier::update(Player *self, const GameContext& ctx)
{
    BallCarrier::update(self, ctx);
    if (self->pos.x < ctx.lineOfScrimmage) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            uint32_t keys = keysHeld();

            uint32_t availableKeys = 0;
            for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
                availableKeys |= ctx.offensePlay.passCatchers[i].button;
            }

            if (keys & availableKeys) {
                ctx.football->start = self->pos;
                ctx.football->pos = self->pos;
                ctx.football->speed = 2.8;
                ctx.football->t = 0.0f;
                ctx.football->resetStatus();
                ctx.football->setStatus(Football::Status::FLYING);
                self->clearStatus(Player::Status::BALL_CARRIER);
                for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
                    if (ctx.offensePlay.passCatchers[i].button & keys) {
                        Vector2 dest = interceptPoint(
                            ctx.offensePlay.passCatchers[i].player->pos,
                            { (float)convertToPixelYards(100), ctx.offensePlay.passCatchers[i].player->pos.y },
                            ctx.offensePlay.passCatchers[i].player->stats.topSpeed,
                            self->pos,
                            ctx.football->speed
                        );
                        // ) + Vector2({ 3.0f, 0 }); // prevent ball landing right at receiver
                        if (dest.x == -1.0f && dest.y == -1.0f) {
                            char buf[64];
                            snprintf(buf, sizeof(buf), "THROWING BALL CARRIER: interceptPoint no solution (catcher %d)\n", i);
                            nocashWrite(buf, strlen(buf));
                        }
                        ctx.football->destination = dest;
                    }
                }
                return;
            }
        } else if (self->hasStatus(Player::Status::BALL_CARRIER)) {
            printf("THROWING BALL CARRIER: BALL_CARRIER but football not HIDDEN (flags=%d)\n", (int)ctx.football->statusFlags);
            while(true) swiWaitForVBlank();
        } else {
            self->setStatus(Player::Status::BLOCKING);
            Player* rusher = Blocking::findTarget(self, ctx);

            if (rusher != nullptr) {
                self->goTo(rusher->pos);
            }
        }
    } else {
        self->behavior = &runningBallCarrierBehavior;
    }
}
