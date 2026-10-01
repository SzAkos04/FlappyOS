[bits 32]

global _start
extern _main_c

section .text.kernel_entry

PML4_TABLE equ 0x70000
PDPT_TABLE equ 0x71000
PD_TABLE   equ 0x72000

CODE_SEG64 equ 0x08
DATA_SEG64 equ 0x10

_start:
	call setup_long_mode

	lgdt [gdt64_descriptor]
	jmp  CODE_SEG64:long_mode_start

setup_long_mode:
	push eax
	push ecx
	push edx
	push edi

	mov edi, PML4_TABLE
	mov ecx, (0x1000 * 3) / 4
	xor eax, eax
	cld
	rep stosd

	mov dword [PML4_TABLE], PDPT_TABLE | 0x3
	mov dword [PML4_TABLE + 4], 0

	mov dword [PDPT_TABLE], PD_TABLE | 0x3
	mov dword [PDPT_TABLE + 4], 0

	mov dword [PD_TABLE + 0], 0x00000000 | 0x83
	mov dword [PD_TABLE + 4], 0
	mov dword [PD_TABLE + 8], 0x00200000 | 0x83
	mov dword [PD_TABLE + 12], 0
	mov dword [PD_TABLE + 16], 0x00400000 | 0x83
	mov dword [PD_TABLE + 20], 0
	mov dword [PD_TABLE + 24], 0x00600000 | 0x83
	mov dword [PD_TABLE + 28], 0

	mov eax, cr4
	or  eax, 1 << 5
	mov cr4, eax

	mov eax, PML4_TABLE
	mov cr3, eax

	mov ecx, 0xC0000080
	rdmsr
	or  eax, 1 << 8
	wrmsr

	mov eax, cr0
	or  eax, 1 << 31
	mov cr0, eax

	pop edi
	pop edx
	pop ecx
	pop eax
	ret

[bits 64]

long_mode_start:
	mov ax, DATA_SEG64
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov rsp, 0x90000

	call _main_c

.hang:
	cli
	hlt
	jmp .hang

section .data
align   8

gdt64_start:
	dq 0x0000000000000000; null descriptor

gdt64_code:
	dq 0x00209A0000000000; 64-bit code: present, ring0, exec, long-mode

gdt64_data:
	dq 0x0000920000000000; 64-bit data: present, ring0, writable

gdt64_end:

gdt64_descriptor:
	dw gdt64_end - gdt64_start - 1
	dd gdt64_start
