#include <cmath>
#include <stdio.h>
#include "field.h"
#include "../game_context.h"
#include "../renderer/renderer.h"
#include "../play_context/play_context.h"
#include "../roster/roster.h"

Field::Field() {
    setStatus(Field::Status::PRESNAP);

    roster = new Roster();
    playContext = new PlayContext(*roster);

    lineOfScrimmage = convertToPixelYards(25);
    drawPosition = lineOfScrimmage;
    firstDown = lineOfScrimmage + convertToPixelYards(10);
    ballCarrier = nullptr;

    football = new Football(roster->offense[0]->pos);

}

void Field::update() {
    scanKeys();
    uint32_t keys = keysDown();

    ballCarrier = nullptr;
    if (football->hasStatus(Football::Status::HIDDEN)) {

        for (int i = 0; i < roster->PLAYER_COUNT; i++) {
            if (roster->offense[i] != nullptr && roster->offense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
                ballCarrier = roster->offense[i];
                drawPosition = (int)roundf(roster->offense[i]->pos.x);
                break;
            } else if (roster->defense[i] != nullptr && roster->defense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
                ballCarrier = roster->defense[i];
                drawPosition = (int)roundf(roster->defense[i]->pos.x);
                break;
            }
        }
    } else {
        drawPosition = (int)roundf(football->pos.x);
    }
    if (hasStatus(Field::Status::PRESNAP)) {
        // TODO: implement play system
        if (keys & KEY_L) {
            clearStatus(Field::Status::PRESNAP);
            setStatus(Field::Status::IN_PLAY);

            playContext->snap(*roster);
        }
    } else if (hasStatus(Field::Status::IN_PLAY)) {
        football->update();
        GameContext ctx { football, ballCarrier, roster->PLAYER_COUNT, lineOfScrimmage, firstDown, playContext->offensePlay, {} };
        for (int i = 0; i < roster->PLAYER_COUNT; i++) {
            if (roster->offense[i] != nullptr) {
                roster->offense[i]->runAI(ctx);
            }
        }
        for (int i = 0; i < roster->PLAYER_COUNT; i++) {
            if (roster->defense[i] != nullptr) {
                roster->defense[i]->runAI(ctx);
                if (ballCarrier != nullptr && roster->defense[i]->pos == ballCarrier->pos) { // TODO: better bounds detection
                    // TACKLE
                    // TODO: CALCULATE BREAK TACKLE
                    endPlay(ballCarrier->pos.x, (ballCarrier->pos.x > firstDown));
                }
            }
        }

        printf("PRESNAP:%d IN_PLAY:%d\n", hasStatus(Field::Status::PRESNAP), hasStatus(Field::Status::IN_PLAY));

        // PLAY END STATES
        if (football->hasStatus(Football::Status::FLYING) && football->pos == football->destination) endPlay(lineOfScrimmage);
        if (ballCarrier != nullptr) {
            ballCarrier->pos.print();
            if (ballCarrier->pos.y > BOTTOM - ballCarrier->stats.height || ballCarrier->pos.y < TOP)
                endPlay(ballCarrier->pos.x, (ballCarrier->pos.x > firstDown));
            if (ballCarrier->pos.x + ballCarrier->stats.width > convertToPixelYards(100)) endPlay(convertToPixelYards(25), false, true);
            if (ballCarrier->pos.x < convertToPixelYards(0)) endPlay(convertToPixelYards(25), false, false, true);
        }
        
    }
}

void Field::endPlay(int lineOfScrimmage, bool firstDown, bool touchdown, bool safety) {
    resetStatus();
    setStatus(Field::Status::PRESNAP);
    for (int i = 0; i < 120; i++) {
        consoleClear();
        printf("Play End Time: %d.%d\n", (120 - i) / 60, ((120 - i) % 60) / 6);
        printf("First Down: %d Touchdown: %d\n", firstDown, touchdown);
        swiWaitForVBlank();
    }

    if (touchdown) {
        this->lineOfScrimmage = convertToPixelYards(25);
        this->firstDown = convertToPixelYards(35);
    } else if (safety) {
        this->lineOfScrimmage = convertToPixelYards(25);
        this->firstDown = convertToPixelYards(35);
     } else {
        this->lineOfScrimmage = lineOfScrimmage;
        if (firstDown) this->firstDown = lineOfScrimmage + convertToPixelYards(10);
    }

    football->resetStatus();
    football->setStatus(Football::Status::HIDDEN);

    playContext->reset(*roster, this->lineOfScrimmage);
}

void Field::draw() {
    Renderer::drawField(drawPosition - VIEWPORT_WIDTH / 4, lineOfScrimmage, firstDown);
    for (int i = 0; i < roster->PLAYER_COUNT; i++) {
        if (roster->offense[i] != nullptr) {
            Renderer::drawOffensePlayer(roster->offense[i], drawPosition - VIEWPORT_WIDTH / 4);
        }
        if (roster->defense[i] != nullptr) {
            Renderer::drawDefensePlayer(roster->defense[i], drawPosition - VIEWPORT_WIDTH / 4);
        }
    }
    if (!football->hasStatus(Football::Status::HIDDEN)) {
        Renderer::drawRect(football->pos.x - (drawPosition - VIEWPORT_WIDTH / 4), football->pos.y, football->drawSize, football->drawSize / 2, football->color);
    }

    // Sidelines drawn last so nothing else can overflow-bleed into them
    Renderer::drawRect(0, 0, VIEWPORT_WIDTH, TOP, SIDELINE_COLOR);
    Renderer::drawRect(0, BOTTOM, VIEWPORT_WIDTH, VIEWPORT_HEIGHT - BOTTOM, SIDELINE_COLOR);

    // Button label sprites above each receiver
    int scrollOffset = drawPosition - VIEWPORT_WIDTH / 4;
    playContext->draw(scrollOffset);
}
