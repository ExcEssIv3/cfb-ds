#include <nds.h>
#include "quarterback.h"
#include "../../../utils.h"

void Quarterback::runAI(Football* football) {
    OffensivePlayer::runAI(football);
    if (hasBall) {
        // if (football->state == FootballState::HIDDEN) {

            uint32_t keys = keysHeld();
            if (keys & KEY_A) {
                football->startX = x;
                football->startY = y;
                football->x = x;
                football->y = y;
                football->destinationX = x + convertToPixelYards(50);
                football->destinationY = y;
                football->speed = 2;
                football->t = 0.0f;
                football->state = FootballState::FLYING;
                hasBall = false;
                return;
            }
        // }
    }
}