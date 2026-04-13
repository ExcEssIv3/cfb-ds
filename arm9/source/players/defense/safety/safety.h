#pragma once

#include "../defensive_player.h"

class Safety : public DefensivePlayer {
public:
    Safety(Vector2 pos, int size, float speed)
        : DefensivePlayer(pos, size, speed, Position::SAFETY) {};
};
