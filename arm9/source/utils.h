#pragma once

#include <cmath>

static const int PIXELS_PER_YARD = 6;

inline int convertToPixelYards(float yards) {
    return floor(yards * PIXELS_PER_YARD);
}

struct Vector2 {
    float x;
    float y;
};

inline float angleTo(Vector2 a, Vector2 b) {
    return atan2f(b.y - a.y, b.x - a.x);
}

inline float distanceTo(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}


