#pragma once

#include "../game_context.h"
#include "../roster/roster.h"

class PlayContext {
public:
    OffensivePlayContext offensePlay;
    DefensivePlayContext defensePlay;
    u16* buttonGfxPtrs[5];

    PlayContext(Roster& roster);
    void snap(Roster& roster);
    void draw(int scrollOffset, bool passable);
    void reset(Roster& roster, int lineOfScrimmage);
};