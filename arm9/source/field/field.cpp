#include "field.h"
#include "../players/offense/quarterback/quarterback.h"
#include "../players/offense/wide_receiver/wide_receiver.h"
#include "../players/defense/linebacker/linebacker.h"
#include "../game_context.h"
#include <cmath>
#include "../renderer/renderer.h"
#include <stdio.h>
#include "button_a.h"
#include "button_b.h"
#include "button_x.h"
#include "button_y.h"
#include "button_rb.h"

static int spriteIndexForButton(uint32_t button) {
    if (button == KEY_A) return 0;
    if (button == KEY_B) return 1;
    if (button == KEY_X) return 2;
    if (button == KEY_Y) return 3;
    if (button == KEY_R) return 4;
    return -1;
}

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

    offensePlay = {
        {
            { offense[1], KEY_A },
            { offense[2], KEY_B },
            { offense[3], KEY_X }
        }, 3
    };

    // Load button sprite tile data
    buttonGfxPtrs[0] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    buttonGfxPtrs[1] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    buttonGfxPtrs[2] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    buttonGfxPtrs[3] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    buttonGfxPtrs[4] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);

    dmaCopy(button_aTiles,  buttonGfxPtrs[0], button_aTilesLen);
    dmaCopy(button_bTiles,  buttonGfxPtrs[1], button_bTilesLen);
    dmaCopy(button_xTiles,  buttonGfxPtrs[2], button_xTilesLen);
    dmaCopy(button_yTiles,  buttonGfxPtrs[3], button_yTilesLen);
    dmaCopy(button_rbTiles, buttonGfxPtrs[4], button_rbTilesLen);

    // Load palettes into sub-palette slots (16 colors each = 32 bytes)
    dmaCopy(button_aPal,  SPRITE_PALETTE + 0 * 16, button_aPalLen);
    dmaCopy(button_bPal,  SPRITE_PALETTE + 1 * 16, button_bPalLen);
    dmaCopy(button_xPal,  SPRITE_PALETTE + 2 * 16, button_xPalLen);
    dmaCopy(button_yPal,  SPRITE_PALETTE + 3 * 16, button_yPalLen);
    dmaCopy(button_rbPal, SPRITE_PALETTE + 4 * 16, button_rbPalLen);
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
        GameContext ctx { football, ballCarrier, PLAYER_COUNT, lineOfScrimmage, firstDown, offensePlay, {} };
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

    // Button label sprites above each receiver
    int scrollOffset = drawPosition - VIEWPORT_WIDTH / 4;
    for (int i = 0; i < offensePlay.passCatcherCount; i++) {
        PassCatcher& pc = offensePlay.passCatchers[i];
        int sprIdx = spriteIndexForButton(pc.button);
        if (sprIdx < 0) continue;
        int screenX = (int)pc.player->pos.x - scrollOffset - 8;
        int screenY = (int)pc.player->pos.y - 20;
        bool hide = screenX < -16 || screenX > VIEWPORT_WIDTH || screenY < -16 || screenY > VIEWPORT_HEIGHT;
        oamSet(&oamMain, i, screenX, screenY, 0, sprIdx,
               SpriteSize_16x16, SpriteColorFormat_16Color,
               buttonGfxPtrs[sprIdx], -1, false, hide, false, false, false);
    }
    oamUpdate(&oamMain);
}
