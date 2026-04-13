#include "football.h"
#include "../renderer/renderer.h"
#include <cmath>

void Football::update() {
    switch(this->state) {
        case FootballState::FLYING: {
            float xDistance = destination.x - start.x;
            float yDistance = destination.y - start.y;
            float dx = pos.x - start.x;
            float dy = pos.y - start.y;
            float totalDistance = hypotf(xDistance, yDistance);

            t += speed / totalDistance;

            if (t >= 1.0f) {
                t = 1.0f;
                pos = destination;
            } else {
                pos.x = t * xDistance + start.x;
                pos.y = t * yDistance + start.y;
            }

            // Calculating height
            // height = [gravity * deltaX * (xFinal - deltaX)]/(2 * xVelocity ^ 2)

            float distanceTraveled = hypotf(dx, dy);
            float gravity = 5; // will have to change this value
            float heightFunc = (gravity * distanceTraveled * (totalDistance - distanceTraveled)) / (2 * speed * speed);
            float heightFuncNormalized = heightFunc / (totalDistance * totalDistance / 4);
            float maxHeight = 4.0f;

            drawSize = (int)floorf(heightFuncNormalized * maxHeight) + size;
            break;
        }
        case FootballState::FUMBLED:
            // TODO: add fumble logic
            break;
        default:
            break;
    }
}
