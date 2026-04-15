#pragma once

#include "../ball_carrier.h"

struct RunningBallCarrier : BallCarrier {
    void update(Player* self, const GameContext& ctx) override;
};

extern RunningBallCarrier runningBallCarrierBehavior;
