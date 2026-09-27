#pragma once

#include "cstdbool.h"
#include "cstdint.h"

#define KEY_W 0x11
#define KEY_S 0x1F
#define KEY_A 0x1E
#define KEY_D 0x20
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D

#define KEY_SPACE 0x39

void keyboard_init(void);

bool key_down(uint8_t scancode);
