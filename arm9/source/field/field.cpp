#include "field.h"
#include "../players/offense/quarterback/quarterback.h"
#include "../players/defense/linebacker/linebacker.h"
#include <cmath>
#include "../renderer/renderer.h"
#include <stdio.h>

Field::Field() {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        offense[i] = nullptr;
        defense[i] = nullptr;
    }

    lineOfScrimmage = convertToPixelYards(25);
    drawPosition = lineOfScrimmage;
    firstDown = lineOfScrimmage + convertToPixelYards(10);
    ballCarrier = nullptr;

    offense[0] = new Quarterback(
        {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)}, 8, 2.0f
    );
    offense[0]->setStatus(Player::Status::BALL_CARRIER);

    defense[0] = new Linebacker({(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)}, 8, 1.0f);
    defense[0]->setStatus(Linebacker::Status::BLITZ);

    football = new Football(offense[0]->pos);
}

void Field::update() {
    scanKeys();

    ballCarrier = nullptr;
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr && offense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
            ballCarrier = offense[i];
            break;
        }
        if (defense[i] != nullptr && defense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
            ballCarrier = defense[i];
            break;
        }
    }

    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            offense[i]->runAI(football, ballCarrier);
            if (offense[i]->hasStatus(Player::Status::BALL_CARRIER))
                drawPosition = (int)roundf(offense[i]->pos.x);
        }
        if (defense[i] != nullptr) {
            defense[i]->runAI(football, ballCarrier);
        }
    }
    football->update();
}

void Field::draw() {
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
        Renderer::drawRect(football->pos.x - (drawPosition - VIEWPORT_WIDTH / 4), football->pos.y, football->drawSize, football->drawSize / 2, football->color);
    }

    // Sidelines drawn last so nothing else can overflow-bleed into them
    Renderer::drawRect(0, 0, VIEWPORT_WIDTH, TOP, SIDELINE_COLOR);
    Renderer::drawRect(0, BOTTOM, VIEWPORT_WIDTH, VIEWPORT_HEIGHT - BOTTOM, SIDELINE_COLOR);

    Renderer::flush();
}
