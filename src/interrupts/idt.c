#include "idt.h"

#include <cstring.h>
#include <idt_arch.h>

struct IDT {
    struct IDTEntry entries[256];
    struct IDTPointer pointer;
};

static struct IDT idt;

extern void idt_load(uintptr_t);

void idt_set(uint8_t index, void (*base)(struct Registers *), uint16_t selector,
             uint8_t flags) {
    idt_entry_set(&idt.entries[index], (uintptr_t)base, selector, flags);
}

void idt_init(void) {
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t)&idt.entries[0];

    memset(&idt.entries[0], 0, sizeof(idt.entries));

    idt_load((uintptr_t)&idt.pointer);
}
