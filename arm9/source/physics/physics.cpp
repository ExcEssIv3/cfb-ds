#include "physics.h"
#include <cmath>

void Physics::move(Player* player, float vx, float vy) {
    player->x += vx;
    player->y += vy;
}

float Physics::distance(Player* a, Player* b) {
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    return sqrtf(dx * dx + dy * dy);
}