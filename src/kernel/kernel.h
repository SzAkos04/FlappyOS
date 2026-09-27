#pragma once

#include "../interrupts/isr.h"

void panic(const char *msg, struct Registers *regs);
