#pragma once

#include <nds.h>
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

    void print() const { printf("(%d, %d)\n", (int)x, (int)y); }
};

inline float angleTo(const Vector2& a, const Vector2& b) {
    return atan2f(b.y - a.y, b.x - a.x);
}

inline float distanceTo(const Vector2& a, const Vector2& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

inline float dotProduct(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

inline float magnitude(const Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

inline Vector2 interceptPoint(const Vector2& targetPos, const Vector2& targetDest, float targetSpeed, const Vector2& pursuerPos, float pursuerSpeed) {
    Vector2 targetTravel = { targetDest.x - targetPos.x, targetDest.y - targetPos.y };
    float len = distanceTo(targetPos, targetDest);

    Vector2 separation = { targetPos.x - pursuerPos.x, targetPos.y - pursuerPos.y };

    float dot = dotProduct(targetTravel, separation);
    float separationDistSquared = dotProduct(separation, separation);

    float relativeSpeedFactor = (pursuerSpeed / targetSpeed) * (pursuerSpeed / targetSpeed) - 1.0f;

    float time;
    if (fabsf(relativeSpeedFactor) < 0.0001f) {
        // speeds are equal, linear solution
        if (fabsf(dot) < 0.0001f) return { -1.0f, -1.0f };
        time = -separationDistSquared / (2.0f * dot);
    } else {
        // solve quadratic
        float a = len * len * relativeSpeedFactor;
        float b = -2.0f * dot;
        float c = -separationDistSquared;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) return { -1.0f, -1.0f };
        time = (-b + sqrtf(discriminant)) / (2.0f * a);
    }

    if (time < 0.0f) time = 0.0f;
    if (time > 1.0f) time = 1.0f;

    return { targetPos.x + time * targetTravel.x, targetPos.y + time * targetTravel.y };
}
