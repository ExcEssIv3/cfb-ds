#include "renderer.h"
#include "../field/field.h"
#include <stdio.h>
#include <cmath>

static uint16_t backbuffer[VIEWPORT_WIDTH * VIEWPORT_HEIGHT];

void Renderer::drawRect(int x, int y, int w, int h, uint16_t color) {
    int x0 = x < 0 ? 0 : x;
    int x1 = x + w > VIEWPORT_WIDTH ? VIEWPORT_WIDTH : x + w;
    int y0 = y < 0 ? 0 : y;
    int y1 = y + h > VIEWPORT_HEIGHT ? VIEWPORT_HEIGHT : y + h;
    for (int row = y0; row < y1; row++)
        for (int col = x0; col < x1; col++)
            backbuffer[row * VIEWPORT_WIDTH + col] = color;
}

void Renderer::drawField(int drawPosition, int lineOfScrimmage, int firstDown) {
    for(int i = 0; i < VIEWPORT_HEIGHT * VIEWPORT_WIDTH; i++) {
        backbuffer[i] = FIELD_COLOR;
    }

    int firstLine = (int)ceil(drawPosition / (5.0 * PIXELS_PER_YARD));
    if (firstLine < 1) {
        drawRect(-10 * PIXELS_PER_YARD - drawPosition, Field::TOP, 10 * PIXELS_PER_YARD, Field::DRAW_HEIGHT, OFFENSE_COLOR);
        if (firstLine < 0) firstLine = 0;
    }
    int lastLine  = (int)floor((drawPosition + VIEWPORT_WIDTH - 1) / (5.0 * PIXELS_PER_YARD));
    if (lastLine > 19) {
        drawRect(100 * PIXELS_PER_YARD - drawPosition, Field::TOP, 10 * PIXELS_PER_YARD, Field::DRAW_HEIGHT, DEFENSE_COLOR);
        if (lastLine > 20) lastLine = 20;
    }

    for (int i = firstLine; i <= lastLine; i++) {
        int fieldX  = i * 5 * PIXELS_PER_YARD;
        int screenX = fieldX - drawPosition;
        drawRect(screenX, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_COLOR);
    }

    drawRect(lineOfScrimmage - drawPosition, Field::TOP, 1, Field::DRAW_HEIGHT, LINE_OF_SCRIMMAGE_COLOR);
    drawRect(firstDown - drawPosition, Field::TOP, 1, Field::DRAW_HEIGHT, FIRST_DOWN_COLOR);
}

void Renderer::drawOffensePlayer(OffensivePlayer* player, int xOffset = 0) {
    int drawX = (int)roundf(player->pos.x) - xOffset;
    int drawY = (int)roundf(player->pos.y);
    Renderer::drawRect(drawX, drawY, player->size / 2, player->size, OFFENSE_COLOR);
}

void Renderer::drawDefensePlayer(DefensivePlayer* player, int xOffset = 0) {
    int drawX = (int)roundf(player->pos.x) - xOffset;
    int drawY = (int)roundf(player->pos.y);
    Renderer::drawRect(drawX, drawY, player->size / 2, player->size, DEFENSE_COLOR);
}

void Renderer::flush() {
    dmaCopy(backbuffer, VRAM_A, VIEWPORT_WIDTH * VIEWPORT_HEIGHT * 2);
}