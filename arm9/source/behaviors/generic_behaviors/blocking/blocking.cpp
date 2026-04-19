#include "blocking.h"
#include "../../../players/player.h"
#include "../../../game_context.h"

Player* Blocking::findTarget(Player* self, const GameContext &ctx)
{
    Player* currentTarget = nullptr;
    float distance = 99999;
    if (self->isOffense) {
        for (int i = 0; i < ctx.playerCount; i++) {
            if (ctx.defense[i] != nullptr) {
                if (
                    currentTarget == nullptr
                    || (
                        !ctx.defense[i]->hasStatus(Player::Status::STUMBLED)
                        && !ctx.defense[i]->hasStatus(Player::Status::BALL_CARRIER)
                        && distance > distanceTo(self->pos, ctx.defense[i]->pos)
                    )
                ) {
                    currentTarget = ctx.defense[i];
                    distance = distanceTo(self->pos, ctx.defense[i]->pos);
                }
            }
        }
    } else {
        for (int i = 0; i < ctx.playerCount; i++) {
            if (ctx.offense[i] != nullptr) {
                if (
                    currentTarget == nullptr
                    || (
                        !ctx.offense[i]->hasStatus(Player::Status::STUMBLED)
                        && !ctx.offense[i]->hasStatus(Player::Status::BALL_CARRIER)
                        && distance > distanceTo(self->pos, ctx.offense[i]->pos
                    ))
                ) {
                    currentTarget = ctx.offense[i];
                    distance = distanceTo(self->pos, ctx.offense[i]->pos);
                }
            }
        }
    }

    return currentTarget;
}