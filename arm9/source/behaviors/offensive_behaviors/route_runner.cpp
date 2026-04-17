#include "route_runner.h"

#include "../../players/player.h"
#include "../../game_context.h"
#include "../../football/football.h"
#include "ball_carrier/running_ball_carrier.h"

void RouteRunner::update(Player *self, const GameContext &ctx)
{
    if (!self->hasStatus(Player::Status::BALL_CARRIER)) {
        if (ctx.football->hasStatus(Football::Status::HIDDEN)) {
            self->goTo({ (float)convertToPixelYards(100), self->pos.y });
        } else if (ctx.football->hasStatus(Football::Status::FLYING)) {
            if (distanceTo(self->pos, ctx.football->pos) <= self->stats.catchRadius) {
                ctx.football->resetStatus();
                ctx.football->setStatus(Football::Status::HIDDEN);
                self->setStatus(Player::Status::BALL_CARRIER);
                self->behavior = &runningBallCarrierBehavior;
            } else {
                Vector2 intercept = interceptPoint(
                    ctx.football->pos,
                    ctx.football->destination,
                    ctx.football->speed,
                    self->pos,
                    self->stats.topSpeed
                );
                if (intercept.x == -1.0f && intercept.y == -1.0f) {
                    char buf[64];
                    snprintf(buf, sizeof(buf), "WR: interceptPoint no solution (pos=%.1f,%.1f)\n", self->pos.x, self->pos.y);
                    nocashWrite(buf, strlen(buf));
                }
                if (false && intercept != Vector2({ -1.0f, -1.0f })) {
                    self->goTo(intercept);
                } else self->goTo({ (float)convertToPixelYards(100), self->pos.y });
            }
        } else if (ctx.football->hasStatus(Football::Status::FUMBLED)) {
            nocashMessage("ROUTE RUNNER: football FUMBLED, no handler\n");
        }
    }
}