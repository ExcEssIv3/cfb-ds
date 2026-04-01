#include <nds.h>
#include <cmath>
#include "field.h"
#include "renderer.h"
#include "physics.h"
#include "players/player.h"

#define OFFENSE_COLOR RGB15(0, 0, 31)
#define DEFENSE_COLOR RGB15(31, 0, 0)

#define PLAYER_SIZE 10  // Size of the player in pixels
#define PLAYER_SPEED 1

#define INVERSE_SQRT_2 0.7071f

int main(void) {
    vramSetBankA(VRAM_A_LCD);
    videoSetMode(MODE_FB0);
    Player* player = new Player(FIELD_WIDTH / 2 - (PLAYER_SIZE / 2), FIELD_HEIGHT / 2 + 20, OFFENSE_COLOR, PLAYER_SIZE);
    Player* defender = new Player(FIELD_WIDTH / 2 - (PLAYER_SIZE / 2), FIELD_HEIGHT / 2 - 20, DEFENSE_COLOR, PLAYER_SIZE);

    while (1) {
        swiWaitForVBlank();
        dmaFillHalfWords(FIELD_COLOR, VRAM_A, FIELD_WIDTH * FIELD_HEIGHT * 2);

        // Player logic
        scanKeys();                                                                                                                                                                                                        
        uint32_t keys = keysHeld();
        float vx = 0;
        float vy = 0;
        if(keys & KEY_UP) {
            vy = -PLAYER_SPEED;
        }
        if(keys & KEY_DOWN) {
            vy = PLAYER_SPEED;
        }
        if(keys & KEY_LEFT) {
            vx = -PLAYER_SPEED;
        }
        if(keys & KEY_RIGHT) {
            vx = PLAYER_SPEED;
        }

        if (vx != 0 || vy != 0) {
            vx = vx * INVERSE_SQRT_2;
            vy = vy * INVERSE_SQRT_2;
        }
        Physics::move(player, vx, vy);
        Renderer::drawPlayer(player);

        // Defender logic
        float dx = player->x - defender->x;
        float dy = player->y - defender->y;
        float dist = Physics::distance(player, defender);
        vx = dx / dist;
        vy = dy / dist;
        Physics::move(defender, vx, vy);
        Renderer::drawPlayer(defender);
    }

    return 0;
}