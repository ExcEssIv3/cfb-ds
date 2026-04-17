#pragma once

#include "../../behavior.h"

struct RouteRunner : Behavior {
    void update(Player* self, const GameContext& ctx) override;
};
