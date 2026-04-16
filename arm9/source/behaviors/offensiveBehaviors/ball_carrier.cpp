#include "../../players/player.h"
#include "../../game_context.h"
#include "ball_carrier.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void BallCarrier::update(Player* self, const GameContext& ctx) {
    uint32_t keys = keysHeld();
    if(keys & KEY_UP) {
        if (keys & KEY_RIGHT) {
            self->move(-M_PI / 4);
            return;
        }
        if (keys & KEY_LEFT) {
            self->move(-3 * M_PI / 4);
            return;
        }
        self->move(-M_PI / 2);
        return;
    }
    if(keys & KEY_DOWN) {
        if (keys & KEY_RIGHT) {
            self->move(M_PI / 4);
            return;
        }
        if (keys & KEY_LEFT) {
            self->move(3 * M_PI / 4);
            return;
        }
        self->move(M_PI / 2);
        return;
    }
    if(keys & KEY_LEFT) {
        self->move(M_PI);
        return;
    }
    if(keys & KEY_RIGHT) {
        self->move(0);
        return;
    }
};