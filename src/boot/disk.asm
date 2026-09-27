[bits 16]

disk_load:
	pusha

	;   BIOS extended read packet
	mov si, DAP
	mov ah, 0x42
	int 0x13
	jc  disk_error

	popa
	ret

disk_error:
	mov  si, DISK_ERROR
	call print_str

	cli
	hlt
	jmp $

DAP:
	db 0x10; packet size
	db 0x00
	dw KERNEL_SECTORS; number of sectors to read
	dw KERNEL_OFFSET; destination offset
	dw 0x0000; destination segment
	dq 0x00000000000001; starting LBA

DISK_ERROR:
	db "Disk read error", 0x0d, 0x0a, 0
