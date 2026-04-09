#include "field.h"
#include "../players/offense/quarterback/quarterback.h"
#include <cmath>
#include "../renderer/renderer.h"

Field::Field() {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        offense[i] = nullptr;
        defense[i] = nullptr;
    }
    
    lineOfScrimmage = convertToPixelYards(25);
    drawPosition = lineOfScrimmage;
    firstDown = lineOfScrimmage + convertToPixelYards(10);

    offense[0] = new Quarterback(
        lineOfScrimmage - convertToPixelYards(5), DRAW_HEIGHT / 2, 8, 3.0f, true
    );
    football = new Football (offense[0]->x, offense[0]->y);
}

void Field::update() {
    scanKeys();

    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            offense[i]->runAI(football);
            if (offense[i]->hasBall) drawPosition = (int)roundf(offense[i]->x);
        }
        if (defense[i] != nullptr) {
            defense[i]->runAI(football);
        }
    }
    football->update();
}

void Field::draw() {
    dmaFillHalfWords(FIELD_COLOR, VRAM_A, VIEWPORT_WIDTH * VIEWPORT_HEIGHT * 2);

    Renderer::drawField(drawPosition - VIEWPORT_WIDTH / 4, lineOfScrimmage, firstDown);
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            Renderer::drawOffensePlayer(offense[i], drawPosition - VIEWPORT_WIDTH / 4);
        }
        if (defense[i] != nullptr) {
            Renderer::drawDefensePlayer(defense[i], drawPosition - VIEWPORT_WIDTH / 4);
        }
    }
    if (football->state != FootballState::HIDDEN) {
        Renderer::drawRect(football->x - (drawPosition - VIEWPORT_WIDTH / 4), football->y, football->drawSize, football->drawSize / 2, football->color);
    }
}