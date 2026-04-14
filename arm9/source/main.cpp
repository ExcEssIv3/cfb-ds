#include <nds.h>
#include <cmath>
#include "perf/perf.h"
#include "field/field.h"
#include "renderer/renderer.h"
#include "physics/physics.h"
#include "football/football.h"
#include "players/offense/offensive_player.h"

#define PLAYER_SIZE 10
#define PLAYER_SPEED 1.0f

#define INVERSE_SQRT_2 0.7071f

int main(void) {
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    videoSetMode(MODE_5_2D);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    int bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSetRotateScale(bg, 0, 1 << 8, 1 << 8);
    bgUpdate();
    Renderer::init((u16*)bgGetGfxPtr(bg));
    videoSetModeSub(MODE_5_2D);
    consoleDemoInit();
    REG_DISPCNT |= DISPLAY_BG2_ACTIVE;

    Field* field = new Field();

    while (1) {
        Perf::startFrame();
        field->update();
        field->draw();
        Perf::endFrame();
        Perf::draw();
        swiWaitForVBlank();
    }

    return 0;
}
