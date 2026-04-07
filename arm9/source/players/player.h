#pragma once

#include <nds.h>

class Player {
public:
    float x;
    float y;
    uint16_t color;
    int size;
    float speed;

    Player(float x, float y, uint16_t color, int size, float speed);
    void move(float direction);
    void goTo(float gx, float gy);
};


