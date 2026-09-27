bits 64

global inportb
global outportb
global inportw
global outportw

section .text

inportb:
	mov dx, di
	xor eax, eax
	in  al, dx
	ret

outportb:
	mov dx, di
	mov al, sil
	out dx, al
	ret

inportw:
	mov dx, di
	xor eax, eax
	in  ax, dx
	ret

outportw:
	mov dx, di
	mov ax, si
	out dx, ax
	ret
