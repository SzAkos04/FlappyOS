#pragma once

#include <cstdint.h>

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t __ignored;
    uint8_t type;
    uint16_t offset_high;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed));

static inline void idt_entry_set(struct IDTEntry *entry, uintptr_t base,
                                 uint16_t selector, uint8_t flags) {
    entry->offset_low = base & 0xFFFF;
    entry->offset_high = (base >> 16) & 0xFFFF;
    entry->selector = selector;
    entry->__ignored = 0;
    entry->type = flags;
}
