#pragma once

#include <nds.h>
#include "../field_geometry.h"
#include "../status_mixin.h"
#include "../game_context.h"
#include "../roster/roster.h"
#include "../play_context/play_context.h"
#include "../players/player.h"
#include "../football/football.h"

struct BlockEngagement {
    Player* blocker;
    Player* rusher;
};

struct BlockCooldown {
    Player* blocker;
    Player* rusher;
    int framesRemaining;
};

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

    static constexpr int MAX_BLOCK_ENGAGEMENTS = 10;
    BlockEngagement blockEngagements[MAX_BLOCK_ENGAGEMENTS];
    int blockEngagementCount = 0;

    static constexpr int MAX_BLOCK_COOLDOWNS = 10;
    static constexpr int BEAT_BLOCK_COOLDOWN_FRAMES = 90;
    BlockCooldown blockCooldowns[MAX_BLOCK_COOLDOWNS];
    int blockCooldownCount = 0;

    Field();
    void draw();
    void update();
    void endPlay(int lineOfScrimmage, bool firstDown = false, bool touchdown = false, bool safety = false);
};