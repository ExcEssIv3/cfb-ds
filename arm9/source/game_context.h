#pragma once

#include <stdint.h>

class OffensivePlayer;
class Player;
struct Football;

struct PassCatcher {
    OffensivePlayer* player;
    uint32_t button;
};

struct OffensivePlayContext {
    PassCatcher passCatchers[5];
    int passCatcherCount;
};

struct DefensivePlayContext {
    // stub
};

struct GameContext {
    Football* football;
    Player* ballCarrier;
    int playerCount;
    int lineOfScrimmage;
    int firstDown;
    OffensivePlayContext offensePlay;
    DefensivePlayContext defensePlay;
};
