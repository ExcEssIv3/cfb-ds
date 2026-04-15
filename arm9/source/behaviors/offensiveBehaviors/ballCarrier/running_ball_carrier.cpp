#include "../../../players/player.h"
#include "../../../game_context.h"
#include "running_ball_carrier.h"

RunningBallCarrier runningBallCarrierBehavior;

void RunningBallCarrier::update(Player* self, const GameContext& ctx) {
    BallCarrier::update(self, ctx);
}
