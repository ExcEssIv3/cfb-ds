#pragma once

#include "../players/player.h"
#include "../game_context.h"
#include "../field_geometry.h"

class Roster {
public:
    static const int PLAYER_COUNT = 11;
    Player* offense[PLAYER_COUNT];
    Player* defense[PLAYER_COUNT];

    Roster();

    void endPlay(int lineOfScrimmage, const OffensivePlayContext& octx, const DefensivePlayContext& dctx);
};