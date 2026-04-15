#pragma once

#include "../defensive_player.h"

class Linebacker : public DefensivePlayer {
public:
    Linebacker(Vector2 pos, int size, float speed)
        : DefensivePlayer(pos, size, speed, Position::LINEBACKER) {};
};
