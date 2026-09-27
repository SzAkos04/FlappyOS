bits 64

extern isr_handler

global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31
global _isr32
global _isr33
global _isr34
global _isr35
global _isr36
global _isr37
global _isr38
global _isr39
global _isr40
global _isr41
global _isr42
global _isr43
global _isr44
global _isr45
global _isr46
global _isr47

section .text

%macro ISR_NO_ERR 1

_isr%1:
	cli

	push qword 0
	push qword %1

	jmp isr_common

%endmacro

%macro ISR_ERR 1

_isr%1:
	cli

	push qword %1

	jmp isr_common

%endmacro

	ISR_NO_ERR 0
	ISR_NO_ERR 1
	ISR_NO_ERR 2
	ISR_NO_ERR 3
	ISR_NO_ERR 4
	ISR_NO_ERR 5
	ISR_NO_ERR 6
	ISR_NO_ERR 7

	ISR_ERR    8

	ISR_NO_ERR 9

	ISR_ERR    10
	ISR_ERR    11
	ISR_ERR    12
	ISR_ERR    13
	ISR_ERR    14

	ISR_NO_ERR 15
	ISR_NO_ERR 16
	ISR_NO_ERR 17
	ISR_NO_ERR 18
	ISR_NO_ERR 19
	ISR_NO_ERR 20
	ISR_NO_ERR 21
	ISR_NO_ERR 22
	ISR_NO_ERR 23
	ISR_NO_ERR 24
	ISR_NO_ERR 25
	ISR_NO_ERR 26
	ISR_NO_ERR 27
	ISR_NO_ERR 28
	ISR_NO_ERR 29
	ISR_NO_ERR 30
	ISR_NO_ERR 31

	ISR_NO_ERR 32
	ISR_NO_ERR 33
	ISR_NO_ERR 34
	ISR_NO_ERR 35
	ISR_NO_ERR 36
	ISR_NO_ERR 37
	ISR_NO_ERR 38
	ISR_NO_ERR 39
	ISR_NO_ERR 40
	ISR_NO_ERR 41
	ISR_NO_ERR 42
	ISR_NO_ERR 43
	ISR_NO_ERR 44
	ISR_NO_ERR 45
	ISR_NO_ERR 46
	ISR_NO_ERR 47

isr_common:
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rsi
	push rdi

	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rdi, rsp
	cld

	;   Align the stack for the SysV x86-64 ABI.
	;   Save the original frame pointer on the aligned stack.
	and rsp, -16
	sub rsp, 16
	mov [rsp], rdi

	call isr_handler

	mov rsp, [rsp]

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8

	pop rdi
	pop rsi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16
	iretq
