#pragma once

#include <nds.h>
#include "players/player.h"

namespace Renderer {
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void drawPlayer(Player* player);
}
