#pragma once

#include <nds.h>
#include "../field_geometry.h"
#include "../status_mixin.h"
#include "../game_context.h"
#include "../roster/roster.h"
#include "../play_context/play_context.h"
#include "../players/player.h"
#include "../football/football.h"

class Field : public StatusMixin {
public:
    
    // Game state
    int drawPosition;
    int lineOfScrimmage;
    int firstDown;

    Roster* roster;
    PlayContext* playContext;

    Football* football;
    Player* ballCarrier;

    Field();
    void draw();
    void update();
    void endPlay(int lineOfScrimmage, bool firstDown = false, bool touchdown = false, bool safety = false);
};