#include "renderer.h"
#include "../field/field.h"
#include <cmath>

void Renderer::drawRect(int x, int y, int w, int h, uint16_t color) {
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            VRAM_A[row * VIEWPORT_WIDTH + col] = color;
}

void Renderer::drawField(int drawPosition, int lineOfScrimmage, int firstDown) {
    // Sideline strips (white out-of-bounds areas above and below the field)
    drawRect(0, 0, VIEWPORT_WIDTH, Field::TOP, SIDELINE_COLOR);
    drawRect(0, Field::BOTTOM, VIEWPORT_WIDTH, VIEWPORT_HEIGHT - Field::BOTTOM, SIDELINE_COLOR);

    int firstLine = (int)ceil(drawPosition / (5.0 * PIXELS_PER_YARD));
    int lastLine  = (int)floor((drawPosition + VIEWPORT_WIDTH - 1) / (5.0 * PIXELS_PER_YARD));

    for (int i = firstLine; i <= lastLine; i++) {
        int fieldX  = i * 5 * PIXELS_PER_YARD;
        int screenX = fieldX - drawPosition;
        drawRect(screenX, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_COLOR);
    }

    drawRect(lineOfScrimmage - drawPosition, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_OF_SCRIMMAGE_COLOR);
    drawRect(firstDown - drawPosition, Field::TOP, 1, Field::DRAW_HEIGHT, FIRST_DOWN_COLOR);
}

void Renderer::drawOffensePlayer(OffensivePlayer* player, int xOffset = 0) {
    int drawX = (int)roundf(player->x )- xOffset;
    int drawY = (int)roundf(player->y);
    Renderer::drawRect(drawX, drawY, player->size / 2, player->size, OFFENSE_COLOR);
}

void Renderer::drawDefensePlayer(DefensivePlayer* player, int xOffset = 0) {
    int drawX = (int)roundf(player->x) - xOffset;
    int drawY = (int)roundf(player->y);
    Renderer::drawRect(drawX, drawY, player->size / 2, player->size, DEFENSE_COLOR);
}