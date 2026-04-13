#include "physics.h"
#include <cmath>

void Physics::move(Player* player, float vx, float vy) {
    player->pos.x += vx;
    player->pos.y += vy;
}

float Physics::distance(Player* a, Player* b) {
    return distanceTo(a->pos, b->pos);
}