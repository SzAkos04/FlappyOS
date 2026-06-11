#include "utils.h"

byte inportb(uint16_t port) {
    byte result;
    asm("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void outportb(uint16_t port, uint8_t data) {
    asm("out %%al, %%dx" : : "a"(data), "d"(port));
}

// FIX: was returning uint8_t while reading a 16-bit value into a uint16_t
// local, silently truncating the high byte before returning. Now correctly
// declared and implemented as uint16_t -> uint16_t.
uint16_t inportw(uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}

void outportw(uint16_t port, uint16_t data) {
    asm("out %%ax, %%dx" : : "a"(data), "d"(port));
}
