#include "catching.h"
#include "../../../players/player.h"
#include "../../../game_context.h"
#include "../../../football/football.h"
#include <cstdlib>

constexpr float baseCatchOdds = 0.5f;
constexpr float catchSkillWeight = 0.45f; // 0..100 stat → up to ±45% odds

bool Catching::catchBall(Player *self, const GameContext &ctx)
{

    if (self->hasStatus(Player::Status::FAILED_CATCH)) return false;

    Vector2 catchLocation = self->pos + Vector2({self->stats.width / 2.0f, self->stats.height / 4.0f});
    if (distanceTo(catchLocation, ctx.football->pos) > self->stats.catchRadius) return false;
    if (ctx.football->height > self->stats.jump + self->stats.catchRadius) return false;

    float odds = baseCatchOdds + catchSkillWeight * (self->stats.catching / 100.0f - 0.5f);
    bool caught = (rand() % 100) < (int)(odds * 100);
    char buf[64];
    snprintf(buf, sizeof(buf), "CATCH: odds=%.0f%% height=%.1f jump=%.1f %s\n",
        odds * 100, ctx.football->height, self->stats.jump + self->stats.catchRadius,
        caught ? "CAUGHT" : "DROPPED");
    nocashWrite(buf, strlen(buf));
    if (!caught) self->setStatus(Player::Status::FAILED_CATCH);
    return caught;
}