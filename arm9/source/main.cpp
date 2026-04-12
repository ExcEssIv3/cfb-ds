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
    vramSetBankA(VRAM_A_LCD);
    videoSetMode(MODE_FB0);
    videoSetModeSub(MODE_5_2D);
    consoleDemoInit();
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
