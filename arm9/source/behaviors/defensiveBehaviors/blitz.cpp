#include "../../players/player.h"
#include "../../game_context.h"
#include "blitz.h"

void Blitz::update(Player* self, const GameContext& ctx) {
    if (ctx.ballCarrier != nullptr) {
        self->goTo(ctx.ballCarrier->pos);
    } else {
        nocashMessage("BLITZ: ctx.ballCarrier is null\n");
        return;
    }
};