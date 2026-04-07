#include "renderer.h"
#include <cmath>

void Renderer::drawRect(int x, int y, int w, int h, uint16_t color) {
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            VRAM_A[row * SCREEN_WIDTH + col] = color;
}

void Renderer::drawField(int drawPosition, int lineOfScrimmage, int firstDown) {
    // Sideline strips (white out-of-bounds areas above and below the field)
    drawRect(0, 0, SCREEN_WIDTH, Field::TOP, SIDELINE_COLOR);
    drawRect(0, Field::BOTTOM, SCREEN_WIDTH, SCREEN_HEIGHT - Field::BOTTOM, SIDELINE_COLOR);

    int firstLine = (int)ceil(drawPosition / (5.0 * Field::PIXELS_PER_YARD));
    int lastLine  = (int)floor((drawPosition + SCREEN_WIDTH - 1) / (5.0 * Field::PIXELS_PER_YARD));

    for (int i = firstLine; i <= lastLine; i++) {
        int fieldX  = i * 5 * Field::PIXELS_PER_YARD;
        int screenX = fieldX - drawPosition;
        drawRect(screenX, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_COLOR);
    }

    drawRect(lineOfScrimmage, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_OF_SCRIMMAGE_COLOR);
    drawRect(firstDown, Field::TOP, 1, Field::DRAW_HEIGHT, FIRST_DOWN_COLOR);
}

void Renderer::drawPlayer(Player* player) {
    int drawX = (int)roundf(player->x);
    int drawY = (int)roundf(player->y);
    Renderer::drawRect(drawX, drawY, player->size / 2, player->size, player->color);
}