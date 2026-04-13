#include <nds.h>
#include <cmath>
#include "player.h"
#include "../football/football.h"

Player::Player(
    Vector2 pos,
    int size,
    float speed,
    bool isOffense,
    Position position,
    uint16_t statusFlags
) : pos(pos), size(size), speed(speed), isOffense(isOffense), position(position), statusFlags(statusFlags) {}

void Player::runAI(Football* football, Player* ballCarrier) {
    if (football->state == FootballState::FUMBLED) {
        if (distanceTo(pos, football->pos) < speed) {
            pos = football->pos;
            football->state = FootballState::HIDDEN;
            setStatus(Status::BALL_CARRIER);
        } else {
            goTo(football->pos);
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

void Player::resetStatus() { statusFlags = 0; }
