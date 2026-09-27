#pragma once

#include "isr.h"
#include <cstdint.h>

void irq_install(size_t i, void (*handler)(struct Registers *));
void irq_uninstall(size_t i);
void irq_set_mask(size_t i);
void irq_init(void);
