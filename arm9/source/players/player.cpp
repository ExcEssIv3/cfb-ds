#include <nds.h>
#include <cmath>
#include "player.h"
#include "../football/football.h"
#include "../game_context.h"

Player::Player(
    Vector2 pos,
    int size,
    float speed,
    bool isOffense,
    Position position,
    uint16_t statusFlags
) : pos(pos), size(size), speed(speed), isOffense(isOffense), position(position) {
    this->statusFlags = statusFlags;
}

void Player::runAI(const GameContext& ctx) {
    if (ctx.football->hasStatus(Football::Status::FUMBLED)) {
        if (distanceTo(pos, ctx.football->pos) < speed) {
            pos = ctx.football->pos;
            ctx.football->resetStatus();
            ctx.football->setStatus(Football::Status::HIDDEN);
            setStatus(Status::BALL_CARRIER);
        } else {
            goTo(ctx.football->pos);
        }
    }
}

void Player::move(float direction) {
    pos.x += cosf(direction) * speed;
    pos.y += sinf(direction) * speed;
}

void Player::goTo(Vector2 target) {
    float distance = distanceTo(pos, target);

    if (distance < speed) {
        pos = target;
        return;
    }

    move(angleTo(pos, target));
}

