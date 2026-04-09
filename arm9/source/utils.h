#pragma once

#include <cmath>

static const int PIXELS_PER_YARD = 6;

inline int convertToPixelYards(float yards) {
    return floor(yards * PIXELS_PER_YARD);
}
