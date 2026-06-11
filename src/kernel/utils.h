#pragma once

#include <cstdint.h>

#define asm __asm__
#define CLI() asm("cli")
#define STI() asm("sti")

typedef unsigned char byte;

byte    inportb(uint16_t port);
void    outportb(uint16_t port, uint8_t data);

// FIX: was declared (and defined) returning uint8_t while actually reading a
// 16-bit word. Return type is now uint16_t throughout.
uint16_t inportw(uint16_t port);
void     outportw(uint16_t port, uint16_t data);
