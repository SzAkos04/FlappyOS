bits 32

global inportb
global outportb
global inportw
global outportw

section .text

inportb:
	mov dx, [esp + 4]
	xor eax, eax
	in  al, dx
	ret

outportb:
	mov dx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret

inportw:
	mov dx, [esp + 4]
	xor eax, eax
	in  ax, dx
	ret

outportw:
	mov dx, [esp + 4]
	mov ax, [esp + 8]
	out dx, ax
	ret
