#pragma once

#include <nds.h>
#include "../ball_carrier.h"

struct ThrowingBallCarrier : BallCarrier {
    float minThrowSpeed = 1.68f;
    float maxHoldTimeSeconds = 0.5f;
    uint32_t buttonHeld = 0;
    int heldDuration = 0;

    void update(Player* self, const GameContext& ctx) override;
    float throwChargeT() const override;

private:
    void throwBall(Player* self, const GameContext& ctx);
};
