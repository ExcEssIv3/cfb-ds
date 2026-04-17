#pragma once

#include "../../behavior.h"

struct BallCarrier : Behavior {
    void update(Player* self, const GameContext& ctx) override;
};
