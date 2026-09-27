#pragma once

#include <stddef.h>

#include <registers.h>

void isr_init(void);

void isr_install(size_t i, void (*handler)(struct Registers *));

void isr_handler(struct Registers *regs);
