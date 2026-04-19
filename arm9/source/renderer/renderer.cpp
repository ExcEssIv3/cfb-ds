#include "renderer.h"
#include "../field/field.h"
#include <stdio.h>
#include <cmath>

static u16* bgGfxPtr = nullptr;
static uint16_t backbuffer[VIEWPORT_WIDTH * VIEWPORT_HEIGHT];

void Renderer::init(u16* gfxPtr) {
    bgGfxPtr = gfxPtr;
}

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
    dmaFillHalfWords(FIELD_COLOR, backbuffer, VIEWPORT_WIDTH * VIEWPORT_HEIGHT * 2);

    int firstLine = (int)ceil(drawPosition / (5.0 * PIXELS_PER_YARD));
    if (firstLine < 1) {
        drawRect(-10 * PIXELS_PER_YARD - drawPosition, TOP, 10 * PIXELS_PER_YARD, DRAW_HEIGHT, OFFENSE_COLOR);
        if (firstLine < 0) firstLine = 0;
    }
    int lastLine  = (int)floor((drawPosition + VIEWPORT_WIDTH - 1) / (5.0 * PIXELS_PER_YARD));
    if (lastLine > 19) {
        drawRect(100 * PIXELS_PER_YARD - drawPosition, TOP, 10 * PIXELS_PER_YARD, DRAW_HEIGHT, DEFENSE_COLOR);
        if (lastLine > 20) lastLine = 20;
    }

    for (int i = firstLine; i <= lastLine; i++) {
        int fieldX  = i * 5 * PIXELS_PER_YARD;
        int screenX = fieldX - drawPosition;
        drawRect(screenX, TOP, 1, DRAW_HEIGHT, LINE_COLOR);
    }

    drawRect(lineOfScrimmage - drawPosition, TOP, 1, DRAW_HEIGHT, LINE_OF_SCRIMMAGE_COLOR);
    drawRect(firstDown - drawPosition, TOP, 1, DRAW_HEIGHT, FIRST_DOWN_COLOR);
}

static void drawPlayer(Player* player, int xOffset, uint16_t baseColor) {
    int drawX = (int)roundf(player->pos.x) - xOffset;
    int drawY = (int)roundf(player->pos.y - player->jumpHeight);
    uint16_t color = player->hasStatus(Player::Status::STUMBLED) ? DARKEN(baseColor, 1) : baseColor;
    Renderer::drawRect(drawX, drawY, player->stats.width, player->stats.height, COLOR_BLACK);
    Renderer::drawRect(drawX + 1, drawY + 1, player->stats.width - 2, player->stats.height - 2, color);
    if (player->hasStatus(Player::Status::BALL_CARRIER)) {
        Renderer::drawFootball(
            player->pos + Vector2({roundf(player->stats.width * 0.25f), roundf(player->stats.height * 0.25f) - player->jumpHeight}),
            xOffset
        );
    }
}

void Renderer::drawOffensePlayer(Player* player, int xOffset) {
    drawPlayer(player, xOffset, OFFENSE_COLOR);
}

void Renderer::drawDefensePlayer(Player* player, int xOffset) {
    drawPlayer(player, xOffset, DEFENSE_COLOR);
}

void Renderer::drawFootball(const Vector2& drawPosition, int xOffset, uint8_t whiteLocation) {
    // TODO: Update this to use the size
    int drawX = (int)roundf(drawPosition.x) - xOffset;
    int drawY = (int)roundf(drawPosition.y);

    Renderer::drawRect(drawX + 1, drawY, 2, 3, FOOTBALL_COLOR);
    Renderer::drawRect(drawX, drawY + 1, 1, 1, FOOTBALL_COLOR);
    Renderer::drawRect(drawX + 3, drawY + 1, 1, 1, FOOTBALL_COLOR);
    uint8_t whiteRow = whiteLocation / 4;
    if (whiteRow < 3) Renderer::drawRect(drawX + 1, drawY + whiteRow, 2, 1, COLOR_WHITE);
}

void Renderer::drawPowerBar(int screenX, int screenY, float t) {
    const int barW = 20;
    const int barH = 3;
    // full blue background
    drawRect(screenX, screenY, barW, barH, COLOR_BLUE);
    // depleting dark overlay grows from right as t increases
    int emptyW = (int)roundf((1.0f - t) * barW);
    if (emptyW > 0)
        drawRect(screenX + barW - emptyW, screenY, emptyW, barH, DARKEN(COLOR_BLUE, 2));
}

void Renderer::flush() {
    dmaCopy(backbuffer, bgGfxPtr, VIEWPORT_WIDTH * VIEWPORT_HEIGHT * 2);
}