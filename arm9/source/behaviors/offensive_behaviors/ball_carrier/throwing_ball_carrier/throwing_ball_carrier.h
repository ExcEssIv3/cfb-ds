#pragma once

#include "../ball_carrier.h"

struct ThrowingBallCarrier : BallCarrier {
    void update(Player* self, const GameContext& ctx) override;
};
