#include "renderer.h"
#include "field.h"
#include <cmath>

void Renderer::drawRect(int x, int y, int w, int h, uint16_t color) {
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            VRAM_A[row * FIELD_WIDTH + col] = color;
}

void Renderer::drawPlayer(Player* player) {
    int drawX = (int)roundf(player->x);
    int drawY = (int)roundf(player->y);
    Renderer::drawRect(drawX, drawY, player->size, player->size, player->color);
}
