#pragma once

#include <nds.h>
#include "../utils.h"

enum class FootballState { HIDDEN, FLYING, FUMBLED };

class Football {
    public:
        Vector2 pos;
        Vector2 start;
        Vector2 destination;
        FootballState state = FootballState::HIDDEN;
        float speed = 10.0f;
        float t = 0.0f; // tracks travel
        uint16_t color = RGB15(17, 8, 2);
        int size = 4;
        int drawSize = size;

        Football(Vector2 pos)
            : pos(pos), start(pos), destination(pos) {};

        void update();
};
