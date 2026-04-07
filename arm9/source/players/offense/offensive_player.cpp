#include <nds.h>
#include <cmath>
#include "offensive_player.h"

void OffensivePlayer::runAI() {
    if (this->userControlled) {
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
    } else {
        // TODO: implement ai logic
    }
}