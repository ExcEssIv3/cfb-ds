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

inline Vector2 interceptPoint(const Vector2& footballPos, const Vector2& destination, float footballSpeed, const Vector2& receiverPos, float receiverSpeed) {
    Vector2 footballTravelDistance = { destination.x - footballPos.x, destination.y - footballPos.y};
    float len = distanceTo(footballPos, destination);

    Vector2 receiverFootballDistance = { footballPos.x - receiverPos.x, footballPos.y - receiverPos.y };

    float dot = dotProduct(footballTravelDistance, receiverFootballDistance);
    float receiverDistSquared = dotProduct(receiverFootballDistance, receiverFootballDistance);

    float relativeSpeedFactor = (receiverSpeed / footballSpeed) * (receiverSpeed / footballSpeed) - 1.0f;

    float time;
    if (fabsf(relativeSpeedFactor) <0.0001f) {
        // speeds are equal, linear solution
        if (fabsf(dot) < 0.0001f) return { -1.0f, -1.0f };
        time = -receiverDistSquared / (2.0f * dot);
    } else {
        // solve quadratic
        float a = len * len * relativeSpeedFactor;
        float b = -2.0f * dot;
        float c = -receiverDistSquared;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) return { -1.0f, -1.0f };
        time = (-b + sqrtf(discriminant)) / (2.0f * a);
    }

    if (time < 0.0f) time = 0.0f;
    if (time > 1.0f) time = 1.0f;

    return { footballPos.x + time * footballTravelDistance.x, footballPos.y + time * footballTravelDistance.y };
}
