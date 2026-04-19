#pragma once

#include "../../../players/player.h"
#include "../../../game_context.h"

namespace Blocking {

    Player* findTarget(Player* self, const GameContext& ctx);
}