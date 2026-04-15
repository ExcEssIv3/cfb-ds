#pragma once

#include <nds.h>
#include "../utils.h"
#include "../status_mixin.h"

class Football : public StatusMixin {
    public:
        // Mutually exclusive states — set one at a time
        enum class Status : uint16_t {
            HIDDEN  = 1 << 0,
            FLYING  = 1 << 1,
            FUMBLED = 1 << 2,
        };

        Vector2 pos;
        Vector2 start;
        Vector2 destination;
        float speed = 10.0f;
        float t = 0.0f; // tracks travel
        uint16_t color = RGB15(17, 8, 2) | BIT(15);
        int size = 4;
        int drawSize = size;

        Football(Vector2 pos)
            : pos(pos), start(pos), destination(pos) {
            setStatus(Status::HIDDEN);
        };

        void update();
};
