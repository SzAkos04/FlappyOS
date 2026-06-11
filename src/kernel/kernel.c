#include "cstring.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "utils.h"

#include "./test/color.h"

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

typedef struct {
    int x, y;
} Player;

static Player player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

void update(void) {
    if (key_down(KEY_W)) {
        player.y--;
    }
    if (key_down(KEY_S)) {
        player.y++;
    }
    if (key_down(KEY_A)) {
        player.x--;
    }
    if (key_down(KEY_D)) {
        player.x++;
    }
}

void render(void) {
    clear_buffer();

#if 0
    color_screen();
#endif

    fill_rect(point_new(player.x - 2, player.y - 2),
              point_new(player.x + 2, player.y + 2), WHITE);

    char buf[32];
    print_string("X:", point_new(0, 0), WHITE);
    utoa(player.x, buf);
    print_string(buf, point_new(16, 0), WHITE);

    print_string("Y:", point_new(0, 10), WHITE);
    utoa(player.y, buf);
    print_string(buf, point_new(16, 10), WHITE);

    swap_buffers();
}

void _main_c(void) {
    idt_init();
    isr_init();
    irq_init();
    screen_init();
    timer_init();
    keyboard_init();

    uint32_t last_frame = 0;

    while (1) {
        const uint32_t now = timer_get();
        if ((now - last_frame) >= (TIMER_FREQ / FPS)) {
            last_frame = now;
            update();
            render();
        }
    }
}
