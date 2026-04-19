#include "football.h"
#include "../renderer/renderer.h"
#include <cmath>

void Football::update() {
    if (hasStatus(Status::FLYING)) {
        if (++whitePosition / 4 >= 3) whitePosition = 0;
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

        float distanceTraveled = hypotf(dx, dy);
        constexpr float arcFactor = 0.40f;
        height = arcFactor * distanceTraveled * (totalDistance - distanceTraveled) / totalDistance;
    } else if (hasStatus(Status::FUMBLED)) {
        whitePosition = 0;
        // TODO: add fumble logic
    } else {
        whitePosition = 0;
    }
}
