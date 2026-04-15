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
        
    }
}