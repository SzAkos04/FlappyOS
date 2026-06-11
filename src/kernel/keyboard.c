#include "keyboard.h"

#include "cstdbool.h"
#include "irq.h"
#include "utils.h"

static bool keys[128] = {0};

static void keyboard_handler(struct Registers *regs) {
    uint8_t scancode = inportb(0x60);
    if (scancode & 0x80) {
        keys[scancode & 0x7F] = false;
    } else {
        keys[scancode] = true;
    }
}

bool key_down(uint8_t scancode) { return keys[scancode]; }

void keyboard_init() { irq_install(1, keyboard_handler); }
