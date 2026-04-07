#include <nds.h>
#include <cmath>
#include "field/field.h"
#include "renderer/renderer.h"
#include "physics/physics.h"
#include "football/football.h"
#include "players/offense/offensive_player.h"

#define OFFENSE_COLOR RGB15(0, 0, 31)
#define DEFENSE_COLOR RGB15(31, 0, 0)

#define PLAYER_SIZE 10
#define PLAYER_SPEED 1.0f

#define INVERSE_SQRT_2 0.7071f

int main(void) {
    vramSetBankA(VRAM_A_LCD);
    videoSetMode(MODE_FB0);
    videoSetModeSub(MODE_5_2D);
    OffensivePlayer* qb = new OffensivePlayer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, OFFENSE_COLOR, PLAYER_SIZE, PLAYER_SPEED, true /* hasBall */);
    Football* ball = new Football(qb->x, qb->y);

    while (1) {
        swiWaitForVBlank();
        dmaFillHalfWords(FIELD_COLOR, VRAM_A, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
        Renderer::drawField(0, Field::PIXELS_PER_YARD * 5, Field::PIXELS_PER_YARD * 15);

        qb->runAI();
        Renderer::drawPlayer(qb);

        scanKeys();
        if (keysDown() & KEY_A && ball->state == FootballState::HIDDEN) {
            ball->startX = qb->x;
            ball->startY = qb->y;
            ball->x = qb->x;
            ball->y = qb->y;
            ball->destinationX = qb->x + (50 * Field::PIXELS_PER_YARD);
            ball->destinationY = qb->y;
            ball->speed = 3;
            ball->t = 0.0f;
            ball->state = FootballState::FLYING;
        }

        ball->update();
    }

    return 0;
}
