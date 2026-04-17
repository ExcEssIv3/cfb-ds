#include <nds.h>
#include <stdio.h>
#include "man_defense.h"
#include "../../../game_context.h"
#include "../../../players/player.h"
#include "../../../football/football.h"

void ManDefense::update(Player *self, const GameContext &ctx)
{
    Player* receiver = nullptr;
    for (int i = 0; i < ctx.defensePlay.manDefenderCount; i++) {
        if (ctx.defensePlay.manDefenders[i].defender == self) {
            receiver = ctx.defensePlay.manDefenders[i].receiver;
            break;
        }
    }

    if (receiver != nullptr) {
        if (ctx.ballCarrier != nullptr) {
            if (!(ctx.fieldStatus & static_cast<uint16_t>(FieldStatus::PASSABLE))) {
                if (ctx.ballCarrier->isOffense) {
                    self->goTo(ctx.ballCarrier->pos);
                } else {
                    // TODO: blocking logic
                }
            } else {
                self->goTo(receiver->pos);
            }
        } else if (ctx.football->hasStatus(Football::Status::FLYING)) {
            Vector2 catchPoint = interceptPoint(
                ctx.football->pos,
                ctx.football->destination,
                ctx.football->speed,
                self->pos,
                self->stats.topSpeed
            );
            if (catchPoint != Vector2({-1, -1})) {
                self->goTo(catchPoint);
            } else {
                // TODO: Determine where to go
                self->goTo(receiver->pos);
            }
        } else if (ctx.football->hasStatus(Football::Status::FUMBLED)) {
            self->goTo(ctx.football->pos);
        } else {
            // ballCarrier lags one frame behind on the catch — shadow the receiver
            self->goTo(receiver->pos);
        }
    } else {
        printf("MAN DEFENSE: defender has no assignment — check PlayContext snap()\n");
        while(true) swiWaitForVBlank();
    }
}
