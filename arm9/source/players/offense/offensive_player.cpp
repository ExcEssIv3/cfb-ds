#include <nds.h>
#include <cmath>
#include "../../football/football.h"
#include "offensive_player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void OffensivePlayer::runAI(const GameContext& ctx) {
    Player::runAI(ctx);
    if (hasStatus(Status::BALL_CARRIER)) {
        uint32_t keys = keysHeld();
        if(keys & KEY_UP) {
            if (keys & KEY_RIGHT) {
                Player::move(-M_PI / 4);
                return;
            }
            if (keys & KEY_LEFT) {
                Player::move(-3 * M_PI / 4);
                return;
            }
            Player::move(-M_PI / 2);
            return;
        }
        if(keys & KEY_DOWN) {
            if (keys & KEY_RIGHT) {
                Player::move(M_PI / 4);
                return;
            }
            if (keys & KEY_LEFT) {
                Player::move(3 * M_PI / 4);
                return;
            }
            Player::move(M_PI / 2);
            return;
        }
        if(keys & KEY_LEFT) {
            Player::move(M_PI);
            return;
        }
        if(keys & KEY_RIGHT) {
            Player::move(0);
            return;
        }
    }
}