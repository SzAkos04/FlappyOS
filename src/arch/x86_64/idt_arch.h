#pragma once

#include <cstdint.h>

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static inline void idt_entry_set(struct IDTEntry *entry, uintptr_t base,
                                 uint16_t selector, uint8_t flags) {
    entry->offset_low = base & 0xFFFF;
    entry->offset_mid = (base >> 16) & 0xFFFF;
    entry->offset_high = (base >> 32) & 0xFFFFFFFF;

    entry->selector = selector;
    entry->ist = 0;
    entry->type = flags;
    entry->reserved = 0;
}
