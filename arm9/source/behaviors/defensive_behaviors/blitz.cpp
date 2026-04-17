#include "blitz.h"
#include "../../players/player.h"
#include "../../game_context.h"

void Blitz::update(Player* self, const GameContext& ctx) {
    if (ctx.ballCarrier != nullptr) {
        self->goTo(ctx.ballCarrier->pos);
    } else {
        // TODO: Fix this bug, this case happens a lot
        nocashMessage("BLITZ: ctx.ballCarrier is null\n");
    }
};