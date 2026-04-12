#pragma once

#include <nds.h>
#include "../screen.h"
#include "../players/offense/offensive_player.h"
#include "../players/defense/defensive_player.h"

// Field / line colors
#define FIELD_COLOR             RGB15(14, 21, 7)
#define SIDELINE_COLOR          RGB15(31, 31, 31)
#define LINE_COLOR              RGB15(31, 31, 31)
#define LINE_OF_SCRIMMAGE_COLOR RGB15(0, 0, 31)
#define FIRST_DOWN_COLOR        RGB15(31, 31, 0)

// Player colors
#define OFFENSE_COLOR           RGB15(0, 0, 31)
#define DEFENSE_COLOR           RGB15(31, 0, 0)

namespace Renderer {
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void drawOffensePlayer(OffensivePlayer* player, int xOffset);
    void drawDefensePlayer(DefensivePlayer* player, int xOffset);
    void drawField(int drawPosition, int lineOfScrimmage, int firstDown);
    void flush();
}