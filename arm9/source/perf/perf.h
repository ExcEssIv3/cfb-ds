#pragma once

#include <nds.h>
#include <malloc.h>

namespace Perf {
    extern float fps;
    extern float ms;

    void startFrame();
    void endFrame();
    void draw();
}