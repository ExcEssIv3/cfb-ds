#include "play_context.h"
#include "../behaviors/offensive_behaviors/ball_carrier/throwing_ball_carrier/throwing_ball_carrier.h"
#include "../behaviors/offensive_behaviors/route_runner/route_runner.h"
#include "../behaviors/defensive_behaviors/blitz/blitz.h"
#include "../behaviors/defensive_behaviors/man_defense/man_defense.h"

static ThrowingBallCarrier throwingBallCarrierBehavior;
static RouteRunner routeRunnerBehavior;
static Blitz blitzBehavior;
static ManDefense manDefenseBehavior;
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

PlayContext::PlayContext(Roster& roster)
{
    offensePlay = {
        {
            { roster.offense[1], KEY_A },
            { roster.offense[2], KEY_B },
            { roster.offense[3], KEY_X }
        }, 3
    };

    defensePlay = {
        .manDefenders = {
            { roster.defense[1], roster.offense[1] },
            { roster.defense[2], roster.offense[2] },
            { roster.defense[3], roster.offense[3] },
        },
        .manDefenderCount = 3,
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

void PlayContext::snap(Roster &roster)
{
    roster.offense[0]->behavior = &throwingBallCarrierBehavior;
    for (int i = 1; i < roster.PLAYER_COUNT; i++) {
        if (roster.offense[i] != nullptr) roster.offense[i]->behavior = &routeRunnerBehavior;
    }
    for (int i = 0; i < roster.PLAYER_COUNT; i++) {
        if (roster.defense[i] != nullptr) roster.defense[i]->behavior = &blitzBehavior;
    }
    for (int i = 0; i < defensePlay.manDefenderCount; i++) {
        defensePlay.manDefenders[i].defender->behavior = &manDefenseBehavior;
    }
}

void PlayContext::draw(int scrollOffset, bool passable)
{
    // TODO: fix the draw position to center of middle of pass catcher, not left edge
    for (int i = 0; i < offensePlay.passCatcherCount; i++) {
        if (passable) {
            PassCatcher& pc = offensePlay.passCatchers[i];
            int sprIdx = spriteIndexForButton(pc.button);
            if (sprIdx < 0) continue;
            int screenX = (int)pc.player->pos.x - scrollOffset - 8 + (pc.player->stats.width / 2);
            int screenY = (int)pc.player->pos.y - 20;
            bool hide = screenX < -16 || screenX > VIEWPORT_WIDTH || screenY < -16 || screenY > VIEWPORT_HEIGHT;
            oamSet(&oamMain, i, screenX, screenY, 0, sprIdx,
                SpriteSize_16x16, SpriteColorFormat_16Color,
                buttonGfxPtrs[sprIdx], -1, false, hide, false, false, false);
        } else {
            oamSetHidden(&oamMain, i, true);
        }
    }
    oamUpdate(&oamMain);
}

void PlayContext::reset(Roster &roster, int lineOfScrimmage)
{
    roster.endPlay(lineOfScrimmage, offensePlay, defensePlay);
}
