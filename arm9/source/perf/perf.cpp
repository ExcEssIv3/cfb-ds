#include "perf.h"
#include <stdio.h>

float Perf::fps = 0;
float Perf::ms = 0;

void Perf::startFrame() {
    cpuStartTiming(0);
}

void Perf::endFrame() {
    u32 cycles = cpuEndTiming();
    fps = 33513982.0f / cycles;
    ms = cycles / 33513.982f;
}

void Perf::draw() {
    struct mallinfo mi = mallinfo();
    printf("\x1b[2J\x1b[0;0H");
    printf("FPS: %d MS: %d\n", (int)fps, (int)ms);
    printf("Alloc: %d Free: %d\n", (int)mi.uordblks, (int)mi.fordblks);
}