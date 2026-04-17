#pragma once

#include "../behavior.h"

struct ManDefense : Behavior {
    void update(Player* self, const GameContext& ctx) override;
};
