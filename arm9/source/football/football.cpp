#include "football.h"
#include "../renderer/renderer.h"
#include <cmath>

void Football::update() {
    switch(this->state) {
        case FootballState::FLYING: {
            float xDistance = destinationX - startX; // distance to be traveled from start in x direction
            float yDistance = destinationY - startY; // distance to be traveled from start in y direction
            float dx = x - startX; // change in x
            float dy = y - startY; // change in y
            float totalDistance = hypotf(xDistance, yDistance);

            t += speed / totalDistance;

            if (t >= 1.0f) {
                t = 1.0f;
                x = destinationX;
                y = destinationY;
            } else {
                x = t * xDistance + startX;
                y = t * yDistance + startY;
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
