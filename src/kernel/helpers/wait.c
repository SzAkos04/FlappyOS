#include "wait.h"

#include "../timer.h"
#include "cstdint.h"

void wait_ticks(uint32_t ticks) {
    uint32_t start = timer_get();
    while ((uint32_t)(timer_get() - start) < ticks) {
        // busy-wait: do nothing
    }
}

void wait_ms(uint32_t ms) {
    // Compute ceil(ms * TIMER_FREQ / 1000) without using 64-bit division.
    // Let ms = a*1000 + b where a = ms/1000 and b = ms%1000.
    // Then ticks = a*TIMER_FREQ + ceil(b*TIMER_FREQ/1000)
    uint32_t a = ms / 1000;
    uint32_t b = ms % 1000;

    uint32_t ticks = a * TIMER_FREQ;
    // (b * TIMER_FREQ + 999) / 1000 implements the ceiling for the remainder
    // part
    ticks += (b * TIMER_FREQ + 999) / 1000;

    uint32_t start = timer_get();
    while ((uint32_t)(timer_get() - start) < ticks) {
        // halt until next interrupt (timer IRQ should wake us)
        asm volatile("hlt");
    }
}

void wait_s(uint32_t s) {
    for (uint32_t i = 0; i < s; ++i) {
        wait_ms(1000);
    }
}
