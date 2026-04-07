#include <nds.h>
#include <cmath>
#include "player.h"

Player::Player(
    float x,
    float y,
    uint16_t color,
    int size,
    float speed
) : x(x), y(y), color(color), size(size), speed(speed) {}

void Player::move(float direction) {
    x += cosf(direction) * speed;
    y += sinf(direction) * speed;
}

void Player::goTo(float gx, float gy) {
    float xDelta = gx - x;
    float yDelta = gy - y;
    float distance = sqrtf(xDelta * xDelta + yDelta * yDelta);

    if (distance < speed) {
        x = gx;
        y = gy;
        return;
    }
    
    float angle = atan2f(yDelta, xDelta);
    move(angle);
}