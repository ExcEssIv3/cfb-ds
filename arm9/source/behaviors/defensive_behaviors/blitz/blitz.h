#pragma once

#include "../../behavior.h"

struct Blitz : Behavior {
    void update(Player* self, const GameContext& ctx) override;
};
