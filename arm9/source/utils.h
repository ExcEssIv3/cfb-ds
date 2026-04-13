#pragma once

#include <cmath>
#include <stdio.h>

static const int PIXELS_PER_YARD = 6;

inline int convertToPixelYards(float yards) {
    return floor(yards * PIXELS_PER_YARD);
}

struct Vector2 {
    float x;
    float y;

    bool operator==(const Vector2& other) const {
        return fabsf(x - other.x) < 0.1f && fabsf(y - other.y) < 0.1f;
    }

    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
    Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }

    void print() const { iprintf("(%d, %d)\n", (int)x, (int)y); }
};

inline float angleTo(Vector2 a, Vector2 b) {
    return atan2f(b.y - a.y, b.x - a.x);
}

inline float distanceTo(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}


