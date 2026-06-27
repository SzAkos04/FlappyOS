#include "cstring.h"
#include "helpers/rng.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#include "test/flappy.h"

#define FPS 30

void panic(const char *msg, struct Registers *regs) {
    clear_buffer();
    print_string("PANIC:", point_new(0, 0), RED);
    print_string(msg, point_new(0, 10), WHITE);

    char buf[32];
    print_string("EIP:", point_new(0, 30), WHITE);
    utoa(regs->eip, buf);
    print_string(buf, point_new(40, 30), WHITE);

    print_string("ERR:", point_new(0, 40), WHITE);
    utoa(regs->err_code, buf);
    print_string(buf, point_new(40, 40), WHITE);

    swap_buffers();
    for (;;) {
    }
}

void _main_c(void) {
    idt_init();
    isr_init();
    irq_init();
    screen_init();
    timer_init();
    keyboard_init();

    rand_seed(timer_get());

    State state;
    state_init(&state);

    uint32_t last_frame = 0;

    while (1) {
        const uint32_t now = timer_get();
        if ((now - last_frame) >= (TIMER_FREQ / FPS)) {
            last_frame = now;
            flappy_update(&state);
            flappy_render(&state);
        }
    }
}
