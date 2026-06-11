#include "cstring.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "utils.h"

// FIX: only include the test that is actually used. The other three headers
// (mixed.h, prime.h, text.h) were included but their functions were never
// called, adding dead weight to every build.
#include "./test/color.h"

#define FPS 30

void panic(const char *msg) {
    clear_buffer();
    print_string(msg, point_new(0, 0), RED);
    swap_buffers();
    for (;;) {}
}

void render(void) {
    clear_buffer();

    color_screen();

    char buf[32];
    utoa(timer_get(), buf);
    print_string(buf, point_new(0, 0), WHITE);

    swap_buffers();
}

void _main_c(void) {
    idt_init();
    isr_init();
    irq_init();
    screen_init();
    // FIX: timer was commented out, leaving timer_get() always returning 0 and
    // the render loop running at full CPU speed with no frame limiting.
    // Timer is now initialised and the render loop is gated to FPS.
    timer_init();
    keyboard_init();

    uint32_t last_frame = 0;

    while (1) {
        const uint32_t now = timer_get();
        if ((now - last_frame) >= (TIMER_FREQ / FPS)) {
            last_frame = now;
            render();
        }
    }
}
