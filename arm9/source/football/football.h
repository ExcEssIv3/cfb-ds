#pragma once

#include <nds.h>

enum class FootballState { HIDDEN, FLYING, FUMBLED };

class Football {
    public:
        float x;
        float y;
        int startX;
        int startY;
        int destinationX;
        int destinationY;
        FootballState state = FootballState::HIDDEN;
        float speed = 10.0f;
        float t = 0.0f; // tracks travel
        uint16_t color = RGB15(17, 8, 2);
        int size = 4;

        Football(float x, float y)
            : x(x), y(y), startX(x), startY(y), destinationX(x), destinationY(y) {};

        void update();
};
