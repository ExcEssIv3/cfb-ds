#include <nds.h>
#include "quarterback.h"
#include "../../../utils.h"

void Quarterback::runAI(Football* football, Player* ballCarrier) {
    OffensivePlayer::runAI(football, ballCarrier);
    if (hasStatus(Status::BALL_CARRIER)) {
        if (football->hasStatus(Football::Status::HIDDEN)) {
            uint32_t keys = keysHeld();
            if (keys & KEY_A) {
                football->start = pos;
                football->pos = pos;
                football->destination = { pos.x + convertToPixelYards(50), pos.y };
                football->speed = 2;
                football->t = 0.0f;
                football->resetStatus();
                football->setStatus(Football::Status::FLYING);
                clearStatus(Status::BALL_CARRIER);
                return;
            }
        }
    }
}
