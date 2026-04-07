#include "field.h"

Field::Field() : drawPosition(0), lineOfScrimmage(0), firstDown(0) {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        offense[i] = nullptr;
        defense[i] = nullptr;
    }
}