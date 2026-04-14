#include "field.h"
#include "../players/offense/quarterback/quarterback.h"
#include "../players/offense/wide_receiver/wide_receiver.h"
#include "../players/defense/linebacker/linebacker.h"
#include "../game_context.h"
#include <cmath>
#include "../renderer/renderer.h"
#include <stdio.h>

Field::Field() {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        offense[i] = nullptr;
        defense[i] = nullptr;
    }

    setStatus(Field::Status::PRESNAP);

    lineOfScrimmage = convertToPixelYards(25);
    drawPosition = lineOfScrimmage;
    firstDown = lineOfScrimmage + convertToPixelYards(10);
    ballCarrier = nullptr;

    // OFFENSE

    // QB

    offense[0] = new Quarterback(
        {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)}, 8, 2.0f
    );
    offense[0]->setStatus(Player::Status::BALL_CARRIER);

    // WR

    offense[1] = new WideReceiver(
        {(float)lineOfScrimmage - 4, (float)(TOP + 20)}, 8, 2.0f, KEY_A
    );
    offense[2] = new WideReceiver(
        {(float)lineOfScrimmage - 4, (float)(TOP + 40)}, 8, 2.0f, KEY_B
    );
    offense[3] = new WideReceiver(
        {(float)lineOfScrimmage - 4, (float)(BOTTOM - 20)}, 8, 2.0f, KEY_X
    );
    // DEFENSE

    defense[0] = new Linebacker({(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)}, 8, 1.0f);
    defense[0]->setStatus(Linebacker::Status::BLITZ);

    football = new Football(offense[0]->pos);
}

void Field::update() {
    scanKeys();
    uint32_t keys = keysHeld();

    ballCarrier = nullptr;
    if (football->hasStatus(Football::Status::HIDDEN)) {

        for (int i = 0; i < PLAYER_COUNT; i++) {
            if (offense[i] != nullptr && offense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
                ballCarrier = offense[i];
                drawPosition = (int)roundf(offense[i]->pos.x);
                break;
            } else if (defense[i] != nullptr && defense[i]->hasStatus(Player::Status::BALL_CARRIER)) {
                ballCarrier = defense[i];
                drawPosition = (int)roundf(defense[i]->pos.x);
                break;
            }
        }
    } else {
        drawPosition = (int)roundf(football->pos.x);
    }
    if (hasStatus(Field::Status::PRESNAP)) {
        // TODO: implement play system
        if (keys & KEY_B) {
            clearStatus(Field::Status::PRESNAP);
            setStatus(Field::Status::IN_PLAY);
        }
    } else if (hasStatus(Field::Status::IN_PLAY)) {
        OffensivePlayContext opctx {
            {
                { offense[1], KEY_A },
                { offense[2], KEY_B },
                { offense[3], KEY_X }
            }, 3
        };
        GameContext ctx { football, ballCarrier, PLAYER_COUNT, lineOfScrimmage, firstDown, opctx, {} };
        for (int i = 0; i < PLAYER_COUNT; i++) {
            if (offense[i] != nullptr) {
                offense[i]->runAI(ctx);
            }
        }
        for (int i = 0; i < PLAYER_COUNT; i++) {
            if (defense[i] != nullptr) {
                defense[i]->runAI(ctx);
                if (ballCarrier != nullptr && defense[i]->pos == ballCarrier->pos) {
                    // TACKLE
                    // TODO: CALCULATE BREAK TACKLE
                    endPlay(ballCarrier->pos.x, (ballCarrier->pos.x > firstDown));
                }
            }
        }
        football->update();

        iprintf("PRESNAP:%d IN_PLAY:%d\n", hasStatus(Field::Status::PRESNAP), hasStatus(Field::Status::IN_PLAY));

        // PLAY END STATES
        if (football->hasStatus(Football::Status::FLYING) && football->pos == football->destination) endPlay(lineOfScrimmage);
        if (ballCarrier != nullptr) {
            ballCarrier->pos.print();
            if (ballCarrier->pos.y > BOTTOM - ballCarrier->size || ballCarrier->pos.y < TOP)
                endPlay(ballCarrier->pos.x, (ballCarrier->pos.x > firstDown));
            if (ballCarrier->pos.x + ballCarrier->size / 2 > convertToPixelYards(100)) endPlay(convertToPixelYards(25), false, true);
            if (ballCarrier->pos.x < convertToPixelYards(0)) endPlay(convertToPixelYards(25), false, false, true);
        }
        
    }
}

void Field::endPlay(int lineOfScrimmage, bool firstDown, bool touchdown, bool safety) {
    resetStatus();
    setStatus(Field::Status::PRESNAP);
    for (int i = 0; i < 120; i++) {
        consoleClear();
        iprintf("Play End Time: %d.%d\n", (120 - i) / 60, ((120 - i) % 60) / 6);
        iprintf("First Down: %d Touchdown: %d\n", firstDown, touchdown);
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

    offense[0]->setStatus(Player::Status::BALL_CARRIER);
    football->resetStatus();
    football->setStatus(Football::Status::HIDDEN);

    offense[0]->pos = {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    defense[0]->pos = {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};

    for (int i = 1; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            offense[i]->pos.x = (float)(this->lineOfScrimmage - 4);
        }
    }
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
    if (!football->hasStatus(Football::Status::HIDDEN)) {
        Renderer::drawRect(football->pos.x - (drawPosition - VIEWPORT_WIDTH / 4), football->pos.y, football->drawSize, football->drawSize / 2, football->color);
    }

    // Sidelines drawn last so nothing else can overflow-bleed into them
    Renderer::drawRect(0, 0, VIEWPORT_WIDTH, TOP, SIDELINE_COLOR);
    Renderer::drawRect(0, BOTTOM, VIEWPORT_WIDTH, VIEWPORT_HEIGHT - BOTTOM, SIDELINE_COLOR);

    Renderer::flush();
}
