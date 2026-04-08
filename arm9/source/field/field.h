#pragma once

#include <nds.h>
#include "../screen.h"
#include "../players/offense/offensive_player.h"
#include "../players/defense/defensive_player.h"
#include "../football/football.h"

class Field {
public:
    // Geometry (static — same for every game)
    static const int PIXELS_PER_YARD    = 6;
    static const int PLAYER_COUNT       = 11;
    static const int DRAW_WIDTH         = 120 * PIXELS_PER_YARD;   // 720px (120 yards)
    static const int DRAW_HEIGHT        = 128;
    static const int TOP                = (VIEWPORT_HEIGHT - DRAW_HEIGHT) / 2;
    static const int BOTTOM             = TOP + DRAW_HEIGHT;
    static const int END_ZONE_PX        = 10 * PIXELS_PER_YARD;    // 60px
    static const int PLAYER_SCREEN_X    = VIEWPORT_WIDTH / 4;      // player anchor at 64px

    // Game state
    int drawPosition;
    int lineOfScrimmage;
    int firstDown;

    OffensivePlayer* offense[PLAYER_COUNT];
    DefensivePlayer* defense[PLAYER_COUNT];
    Football* football;

    Field();
    void draw();
    void update();

    static int convertToPixelYards(float x);
};