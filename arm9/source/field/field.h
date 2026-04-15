#pragma once

#include <nds.h>
#include "../screen.h"
#include "../utils.h"
#include "../status_mixin.h"
#include "../game_context.h"
#include "../players/player.h"
#include "../football/football.h"

class Field : public StatusMixin {
public:
    enum class Status : uint16_t {
        PRESNAP  = 1 << 0,
        IN_PLAY  = 1 << 1,
    };
    // Geometry (static — same for every game)
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

    OffensivePlayContext offensePlay;
    u16* buttonGfxPtrs[5];

    Player* offense[PLAYER_COUNT];
    Player* defense[PLAYER_COUNT];
    Football* football;
    Player* ballCarrier;

    Field();
    void draw();
    void update();
    void endPlay(int lineOfScrimmage, bool firstDown = false, bool touchdown = false, bool safety = false);
};