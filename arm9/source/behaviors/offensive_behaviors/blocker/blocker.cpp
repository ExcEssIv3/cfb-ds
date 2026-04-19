#include "blocker.h"

#include "../../generic_behaviors/blocking/blocking.h"
#include "../../../players/player.h"
#include "../../../game_context.h"

void Blocker::update(Player *self, const GameContext &ctx)
{
    Player* rusher = nullptr;
    for (int i = 0; i <  ctx.offensePlay.blockerCount; i++) {
        if (ctx.offensePlay.blockers[i].blocker == self) {
            rusher = ctx.offensePlay.blockers[i].rusher;
            break;
        }
    }

    if (rusher == nullptr) {
        printf("BLOCKING: blocker has no assignment — check PlayContext snap()\n");
        while(true) swiWaitForVBlank();
    } else {
        if (rusher->hasStatus(Player::Status::PANCAKED) || rusher->hasStatus(Player::Status::STUMBLED)) {
            rusher = Blocking::findTarget(self, ctx);
        }

        // TODO: Update to position blocker between defender and ball carrier
        if (rusher != nullptr) self->goTo(rusher->pos);
    }
}