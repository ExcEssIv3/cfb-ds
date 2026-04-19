#pragma once

#include "../../behavior.h"

struct Blocker : Behavior {
    void update(Player* self, const GameContext& ctx) override;
};
