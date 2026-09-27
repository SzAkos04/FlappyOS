[bits 32]

global _start
extern _main_c

section .text.kernel_entry

_start:
	call _main_c

.hang:
	cli
	hlt
	jmp .hang
