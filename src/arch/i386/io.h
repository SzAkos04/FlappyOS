#pragma once

#include <cstdint.h>

typedef unsigned char byte;

byte inportb(uint16_t port);
void outportb(uint16_t port, uint8_t data);

uint16_t inportw(uint16_t port);
void outportw(uint16_t port, uint16_t data);
