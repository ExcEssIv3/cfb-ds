#pragma once

#include <stdint.h>

class Player;
struct Football;

enum class FieldStatus : uint16_t {
    PRESNAP  = 1 << 0,
    IN_PLAY  = 1 << 1,
    PASSABLE = 1 << 2,
};

struct PassCatcher {
    Player* player;
    uint32_t button;
};

struct OffensivePlayContext {
    PassCatcher passCatchers[5];
    int passCatcherCount;
};

struct ManDefender {
    Player* defender;
    Player* receiver;
};

struct ZoneDefender {
    Player* defender;
    // TODO: Define zones
};

struct DefensivePlayContext {
    PassCatcher passCatchers[5];
    int passCatcherCount;

    ManDefender manDefenders[5];
    int manDefenderCount;

    ZoneDefender zoneDefenders[5];
    int zoneDefenderCount;
};

struct GameContext {
    Football* football;
    Player* ballCarrier;
    int playerCount;
    int lineOfScrimmage;
    int firstDown;
    uint16_t fieldStatus;
    OffensivePlayContext offensePlay;
    DefensivePlayContext defensePlay;
};
