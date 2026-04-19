#pragma once

#include <nds.h>
#include "../screen.h"
#include "../players/player.h"

// Basic colors
#define COLOR_BLACK   (RGB15(0,  0,  0)  | BIT(15))
#define COLOR_WHITE   (RGB15(31, 31, 31) | BIT(15))
#define COLOR_RED     (RGB15(31, 0,  0)  | BIT(15))
#define COLOR_GREEN   (RGB15(0,  31, 0)  | BIT(15))
#define COLOR_BLUE    (RGB15(0,  0,  31) | BIT(15))
#define COLOR_YELLOW  (RGB15(31, 31, 0)  | BIT(15))
#define COLOR_ORANGE  (RGB15(31, 15, 0)  | BIT(15))
#define COLOR_PURPLE  (RGB15(15, 0,  31) | BIT(15))
#define COLOR_CYAN    (RGB15(0,  31, 31) | BIT(15))
#define COLOR_GRAY    (RGB15(15, 15, 15) | BIT(15))
#define COLOR_BROWN   (RGB15(17, 8, 2)   | BIT(15))

#define DARKEN(color, shift) (BIT(15) | ((((color) >> 10) & 0x1F) >> (shift)) << 10 | ((((color) >> 5) & 0x1F) >> (shift)) << 5 | (((color) & 0x1F) >> (shift)))
#define LIGHTEN(color, shift) (BIT(15) | (((((color) >> 10) & 0x1F) + (31 >> (shift))) > 31 ? 31 : (((color) >> 10) & 0x1F) + (31 >> (shift))) << 10 | (((((color) >> 5) & 0x1F) + (31 >> (shift))) > 31 ? 31 : (((color) >> 5) & 0x1F) + (31 >> (shift))) << 5 | ((((color) & 0x1F) + (31 >> (shift))) > 31 ? 31 : ((color) & 0x1F) + (31 >> (shift))))

// Field / line colors
#define FIELD_COLOR             (RGB15(14, 21, 7) | BIT(15))
#define SIDELINE_COLOR          COLOR_WHITE
#define LINE_COLOR              COLOR_WHITE
#define LINE_OF_SCRIMMAGE_COLOR COLOR_BLUE
#define FIRST_DOWN_COLOR        COLOR_YELLOW

// Player colors
#define OFFENSE_COLOR  COLOR_BLUE
#define DEFENSE_COLOR  COLOR_RED

// Football color
#define FOOTBALL_COLOR COLOR_BROWN

namespace Renderer {
    void init(u16* gfxPtr);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void drawOffensePlayer(Player* player, int xOffset);
    void drawDefensePlayer(Player* player, int xOffset);
    void drawFootball(const Vector2& position, int xOffset, uint8_t whiteLocation = 0);
    void drawField(int drawPosition, int lineOfScrimmage, int firstDown);
    void drawPowerBar(int screenX, int screenY, float t);
    void flush();
}