#pragma once

#include <nds.h>

class Player {
public:
    float x;
    float y;
    int size;
    float speed;

    Player(float x, float y, int size, float speed);
    void move(float direction);
    void goTo(float gx, float gy);
};


