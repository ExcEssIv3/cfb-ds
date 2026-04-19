#include "throwing_ball_carrier.h"
#include "../ball_carrier.h"
#include "../running_ball_carrier/running_ball_carrier.h"
#include "../../../generic_behaviors/blocking/blocking.h"
#include "../../../../players/player.h"
#include "../../../../game_context.h"
#include "../../../../football/football.h"

float ThrowingBallCarrier::throwChargeT() const {
    if (heldDuration == 0) return -1.0f;
    int maxFrames = (int)(maxHoldTimeSeconds * 60);
    return std::min(heldDuration, maxFrames) / (float)maxFrames;
}

void ThrowingBallCarrier::update(Player *self, const GameContext& ctx)
{
    BallCarrier::update(self, ctx);
    if (self->pos.x < ctx.lineOfScrimmage) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN) && self->hasStatus(Player::Status::BALL_CARRIER)) {
            uint32_t keys = keysDown() | keysHeld() | keysUp();

            // TODO: Consolidate key calculation, ideally done at play init
            uint32_t availableKeys = 0;
            for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
                availableKeys |= ctx.offensePlay.passCatchers[i].button;
            }

            if (keys & availableKeys) {
                throwBall(self, ctx);
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

void ThrowingBallCarrier::throwBall(Player *self, const GameContext &ctx)
{
    int maxHoldTimeFrames = maxHoldTimeSeconds * 60;

    uint32_t availableKeys = 0;
    for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++)
        availableKeys |= ctx.offensePlay.passCatchers[i].button;

    if (buttonHeld == 0) {
        uint32_t pressed = keysDown() & availableKeys;
        if (pressed) {
            buttonHeld = pressed;
            heldDuration = 1;
        }
        return;
    } else if (buttonHeld & keysUp()) {

        // calculate throw speed:
        float t = std::min(heldDuration, maxHoldTimeFrames) / (float)maxHoldTimeFrames;                                                             
        float speed = minThrowSpeed + t * (self->stats.throwStrength - minThrowSpeed);         

        ctx.football->start = self->pos;
        ctx.football->pos = self->pos;
        ctx.football->speed = speed;
        ctx.football->t = 0.0f;
        ctx.football->resetStatus();
        ctx.football->setStatus(Football::Status::FLYING);
        self->clearStatus(Player::Status::BALL_CARRIER);
        for (int i = 0; i < ctx.offensePlay.passCatcherCount; i++) {
            if (ctx.offensePlay.passCatchers[i].button & buttonHeld) {
                Vector2 dest = interceptPoint(
                    ctx.offensePlay.passCatchers[i].player->pos,
                    { (float)convertToPixelYards(110), ctx.offensePlay.passCatchers[i].player->pos.y },
                    ctx.offensePlay.passCatchers[i].player->stats.topSpeed,
                    self->pos,
                    ctx.football->speed
                // );
                ) + Vector2({ 3.0f, 0 }); // prevent ball landing right at receiver
                if (dest.x == -1.0f && dest.y == -1.0f) {
                    char buf[64];
                    snprintf(buf, sizeof(buf), "THROWING BALL CARRIER: interceptPoint no solution (catcher %d)\n", i);
                    nocashWrite(buf, strlen(buf));
                }
                ctx.football->destination = dest;
            }
        }

        buttonHeld = 0;
        heldDuration = 0;
        return;
    } else if (buttonHeld & keysHeld()) {
        if (maxHoldTimeFrames < heldDuration) {
            //TODO: bad throw
            char buf[64];
            snprintf(buf, sizeof(buf), "THROW: held %d frames over max\n", heldDuration - maxHoldTimeFrames);
            nocashWrite(buf, strlen(buf));
        }
        heldDuration++;
    } else {
        buttonHeld = 0;
        heldDuration = 0;
    }
}
