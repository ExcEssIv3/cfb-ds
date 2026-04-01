#pragma once

#include <nds.h>

class Player {
public:
    float x;
    float y;
    uint16_t color;
    int size;

    Player(float x, float y, uint16_t color, int size);
};


