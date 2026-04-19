#include <cmath>
#include <stdio.h>
#include "field.h"
#include "../behaviors/behavior.h"
#include "../game_context.h"
#include "../renderer/renderer.h"
#include "../play_context/play_context.h"
#include "../roster/roster.h"
#include "../contact/contact.h"

Field::Field() {
    setStatus(FieldStatus::PRESNAP);
    setStatus(FieldStatus::PASSABLE);

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
    if (hasStatus(FieldStatus::PRESNAP)) {
        // TODO: implement play system
        if (keys & KEY_L) {
            clearStatus(FieldStatus::PRESNAP);
            setStatus(FieldStatus::IN_PLAY);

            playContext->snap(*roster);
        }
    } else if (hasStatus(FieldStatus::IN_PLAY)) {
        football->update();
        GameContext ctx { football, ballCarrier, roster->PLAYER_COUNT, lineOfScrimmage, firstDown, statusFlags, playContext->offensePlay, playContext->defensePlay, roster->offense, roster->defense };

        // Tick block cooldowns
        for (int i = blockCooldownCount - 1; i >= 0; i--) {
            if (--blockCooldowns[i].framesRemaining <= 0)
                blockCooldowns[i] = blockCooldowns[--blockCooldownCount];
        }
        for (int i = 0; i < roster->PLAYER_COUNT; i++) {
            if (roster->offense[i] != nullptr) {
                roster->offense[i]->runAI(ctx);

                // Register new block engagements for players with BLOCKING status
                if (roster->offense[i]->hasStatus(Player::Status::BLOCKING)) {
                    for (int j = 0; j < roster->PLAYER_COUNT; j++) {
                        if (roster->defense[j] == nullptr) continue;
                        if (!roster->offense[i]->detectCollision(
                            roster->defense[j]->pos,
                            roster->defense[j]->stats.width,
                            roster->defense[j]->stats.height
                        )) continue;

                        // Check if this pair is already engaged or cooling down
                        bool skip = false;
                        for (int k = 0; k < blockEngagementCount; k++) {
                            if (blockEngagements[k].blocker == roster->offense[i] &&
                                blockEngagements[k].rusher  == roster->defense[j]) {
                                skip = true; break;
                            }
                        }
                        if (!skip) {
                            for (int k = 0; k < blockCooldownCount; k++) {
                                if (blockCooldowns[k].blocker == roster->offense[i] &&
                                    blockCooldowns[k].rusher  == roster->defense[j]) {
                                    skip = true; break;
                                }
                            }
                        }
                        if (!skip && blockEngagementCount < MAX_BLOCK_ENGAGEMENTS) {
                            nocashMessage("Block: engagement registered\n");
                            blockEngagements[blockEngagementCount++] = { roster->offense[i], roster->defense[j] };
                        }
                    }
                }
            }
        }

        // Resolve block engagements
        for (int i = blockEngagementCount - 1; i >= 0; i--) {
            Player* blocker = blockEngagements[i].blocker;
            Player* rusher  = blockEngagements[i].rusher;

            // Drop stale engagements where players are no longer colliding
            if (!blocker->detectCollision(rusher->pos, rusher->stats.width, rusher->stats.height)) {
                blockEngagements[i] = blockEngagements[--blockEngagementCount];
                continue;
            }

            switch (Contact::block(blocker, rusher)) {
                case Contact::BlockResult::RUSHER_PANCAKE:
                    nocashMessage("Block: rusher pancaked\n");
                    rusher->stumbleFrames = 60;
                    rusher->setStatus(Player::Status::STUMBLED);
                    rusher->setStatus(Player::Status::PANCAKED);
                    rusher->velocity = {0, 0};
                    blockEngagements[i] = blockEngagements[--blockEngagementCount];
                    break;
                case Contact::BlockResult::BLOCKER_PANCAKE:
                    nocashMessage("Block: blocker pancaked\n");
                    blocker->stumbleFrames = 60;
                    blocker->setStatus(Player::Status::STUMBLED);
                    blocker->velocity = {0, 0};
                    blockEngagements[i] = blockEngagements[--blockEngagementCount];
                    break;
                case Contact::BlockResult::BEAT_BLOCK:
                    nocashMessage("Block: beat block\n");
                    if (blockCooldownCount < MAX_BLOCK_COOLDOWNS)
                        blockCooldowns[blockCooldownCount++] = { blocker, rusher, BEAT_BLOCK_COOLDOWN_FRAMES };
                    blockEngagements[i] = blockEngagements[--blockEngagementCount];
                    break;
                case Contact::BlockResult::ENGAGED:
                    blocker->velocity = {0, 0};
                    rusher->velocity  = {0, 0};
                    break;
            }
        }

        for (int i = 0; i < roster->PLAYER_COUNT; i++) {
            if (roster->defense[i] != nullptr) {
                roster->defense[i]->runAI(ctx);
                if (
                    ballCarrier != nullptr
                    && ballCarrier->detectCollision(
                        roster->defense[i]->pos,
                        roster->defense[i]->stats.width,
                        roster->defense[i]->stats.height
                    )
                ) {
                    // TACKLE
                    if (roster->defense[i]->hasStatus(Player::Status::STUMBLED)) {
                        continue;
                    } else if (Contact::tackle(roster->defense[i], ballCarrier)) {
                        nocashMessage("Tackle success\n");
                        endPlay(ballCarrier->pos.x, (ballCarrier->pos.x > firstDown));
                    } else {
                        nocashMessage("Tackle fail\n");
                        roster->defense[i]->stumbleFrames = 60;
                        roster->defense[i]->setStatus(Player::Status::STUMBLED);
                        roster->defense[i]->velocity = {0, 0};
                    }
                }
            }
        }

        printf("PRESNAP:%d IN_PLAY:%d\n", hasStatus(FieldStatus::PRESNAP), hasStatus(FieldStatus::IN_PLAY));

        // PLAY END STATES
        if (football->hasStatus(Football::Status::FLYING)) {
            if (football->pos.x > lineOfScrimmage) clearStatus(FieldStatus::PASSABLE);
            if (football->pos == football->destination) endPlay(lineOfScrimmage);
        }
        if (ballCarrier != nullptr) {
            if (ballCarrier->pos.x > lineOfScrimmage) clearStatus(FieldStatus::PASSABLE);
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
    setStatus(FieldStatus::PRESNAP);
    setStatus(FieldStatus::PASSABLE);
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

    blockEngagementCount = 0;
    blockCooldownCount = 0;

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
        Renderer::drawFootball(football->pos, drawPosition - VIEWPORT_WIDTH / 4, football->whitePosition);
    }

    // Sidelines drawn last so nothing else can overflow-bleed into them
    Renderer::drawRect(0, 0, VIEWPORT_WIDTH, TOP, SIDELINE_COLOR);
    Renderer::drawRect(0, BOTTOM, VIEWPORT_WIDTH, VIEWPORT_HEIGHT - BOTTOM, SIDELINE_COLOR);

    // Throw power bar above QB when a throw is being charged
    if (ballCarrier != nullptr && ballCarrier->behavior != nullptr) {
        float t = ballCarrier->behavior->throwChargeT();
        if (t >= 0.0f) {
            int barX = (int)roundf(ballCarrier->pos.x) - (drawPosition - VIEWPORT_WIDTH / 4) - 10;
            int barY = (int)roundf(ballCarrier->pos.y) - 6;
            Renderer::drawPowerBar(barX, barY, t);
        }
    }

    // Button label sprites above each receiver
    int scrollOffset = drawPosition - VIEWPORT_WIDTH / 4;
    playContext->draw(scrollOffset, hasStatus(FieldStatus::PASSABLE));
}
