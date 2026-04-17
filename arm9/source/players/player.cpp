#include <nds.h>
#include <cmath>
#include "player.h"
#include "../football/football.h"
#include "../game_context.h"
#include "../behaviors/behavior.h"

Player::Player(
    Vector2 pos,
    bool isOffense,
    Position position,
    PlayerStats stats,
    uint16_t statusFlags
) : pos(pos), isOffense(isOffense), position(position), stats(stats) {
    this->statusFlags = statusFlags;
}

void Player::runAI(const GameContext& ctx) {
    if (hasStatus(Status::STUMBLED)) {
        if (stumbleFrames > 0) {
            stumbleFrames--;
            return;
        } else {
            clearStatus(Status::STUMBLED);
        }
    }
    if (ctx.football->hasStatus(Football::Status::FUMBLED)) {
        if (distanceTo(pos, ctx.football->pos) < stats.topSpeed) {
            pos = ctx.football->pos;
            ctx.football->resetStatus();
            ctx.football->setStatus(Football::Status::HIDDEN);
            setStatus(Status::BALL_CARRIER);
        } else {
            goTo(ctx.football->pos);
        }
        move();
        return;
    }

    if (behavior != nullptr) {
        behavior->update(this, ctx);
    }

    move();
}

void Player::move() {
    pos = pos + velocity;
}

void Player::accelerate(float direction) {
    Vector2 desired = { cosf(direction) * stats.topSpeed, sinf(direction) * stats.topSpeed };
    Vector2 delta = desired - velocity;
    float deltaMagnitude = magnitude(delta);

    if (deltaMagnitude <= stats.acceleration) {
        velocity = desired;
    } else {
        velocity.x += (delta.x / deltaMagnitude) * stats.acceleration;
        velocity.y += (delta.y / deltaMagnitude) * stats.acceleration;
    }
}

void Player::decelerate() {
    Vector2 desired = {0, 0};
    Vector2 delta = desired - velocity;
    float deltaMagnitude = magnitude(delta);

    if (deltaMagnitude <= stats.acceleration) {
        velocity = desired;
    } else {
        velocity.x += (delta.x / deltaMagnitude) * stats.acceleration;
        velocity.y += (delta.y / deltaMagnitude) * stats.acceleration;
    }
}

void Player::goTo(Vector2 target) {
    float speed = magnitude(velocity);
    float distance = distanceTo(pos, target); 
    if (distance < stats.acceleration && speed < stats.acceleration) {
        pos = target;
        velocity = {0, 0};
        return;
    }

    float stoppingDistance = (speed * speed) / (2 * stats.acceleration);
    if (distance <= stoppingDistance) {
        decelerate();
    } else accelerate(angleTo(pos, target));
}

bool Player::detectCollision(const Vector2& colliderPos, int width, int height)
{
    bool xCollision = pos.x < colliderPos.x + width && pos.x + stats.width > colliderPos.x;
    bool yCollision = pos.y < colliderPos.y + height && pos.y + stats.height > colliderPos.y;

    return xCollision && yCollision;
}
