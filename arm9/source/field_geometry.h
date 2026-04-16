#pragma once

#include "screen.h"
#include "utils.h"

static const int DRAW_WIDTH      = 120 * PIXELS_PER_YARD;              // 720px (120 yards)
static const int DRAW_HEIGHT     = 128;
static const int TOP             = (VIEWPORT_HEIGHT - DRAW_HEIGHT) / 2; // 32px
static const int BOTTOM          = TOP + DRAW_HEIGHT;                   // 160px
static const int END_ZONE_PX     = 10 * PIXELS_PER_YARD;               // 60px
static const int PLAYER_SCREEN_X = VIEWPORT_WIDTH / 4;                 // 64px
