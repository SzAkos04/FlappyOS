bits 64

global cpu_cli
global cpu_sti
global cpu_halt

section .text

cpu_cli:
	cli
	ret

cpu_sti:
	sti
	ret

cpu_halt:
	hlt
	ret
