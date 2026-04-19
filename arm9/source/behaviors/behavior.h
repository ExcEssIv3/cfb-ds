#pragma once

class Player;
struct GameContext;

struct Behavior {
    virtual void update(Player* self, const GameContext& ctx) = 0;
    virtual float throwChargeT() const { return -1.0f; }
    virtual ~Behavior() = default;
};