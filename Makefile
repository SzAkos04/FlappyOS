PROJECT := FlappyOS


ASM := nasm
CC := i686-elf-gcc
LD := i686-elf-ld
QEMU := qemu-system-i386


SRC_DIR := src
BUILD_DIR := build

BOOT_DIR := $(SRC_DIR)/boot
ARCH_DIR := $(SRC_DIR)/arch/i386
KERNEL_DIR := $(SRC_DIR)/kernel
INTERRUPTS_DIR := $(SRC_DIR)/interrupts


CFLAGS := \
	-ffreestanding \
	-fno-pie \
	-fno-builtin \
	-fno-stack-protector \
	-nostdlib \
	-Isrc/libc

LDFLAGS := \
	-T linker.ld \
	--oformat binary

QEMUFLAGS := \
	-no-reboot \
	-drive format=raw


BOOTLOADER := $(BOOT_DIR)/boot.asm
KERNEL_ENTRY := $(BOOT_DIR)/kernel_entry.asm


C_SRC := $(shell find $(SRC_DIR) \
	-type f \
	-name '*.c')

KERNEL_ASM := $(shell find \
	$(ARCH_DIR) \
	$(KERNEL_DIR) \
	$(INTERRUPTS_DIR) \
	-type f \
	-name '*.asm')


OBJ := $(BUILD_DIR)/boot/kernel_entry.o

OBJ += $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,\
	$(filter-out $(KERNEL_ENTRY),$(KERNEL_ASM)))

OBJ += $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))


KERNEL_BIN := $(BUILD_DIR)/kernel.bin
DISK_IMAGE := $(BUILD_DIR)/flappyos.img
BOOTLOADER_BIN := $(BUILD_DIR)/bootloader.bin


.PHONY: all build run clean

all: build

build: $(DISK_IMAGE)


$(DISK_IMAGE): $(KERNEL_BIN)
	@mkdir -p $(@D)

	$(eval KERNEL_SECTORS := $(shell python3 -c \
		"import math; print(math.ceil($(shell wc -c < $<) / 512))"))

	@echo "  ASM     $(BOOTLOADER)"
	$(ASM) -f bin \
		-DKERNEL_SECTORS=$(KERNEL_SECTORS) \
		$(BOOTLOADER) \
		-o $(BOOTLOADER_BIN)

	@echo "  IMAGE   $@"
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@


$(BUILD_DIR)/boot/kernel_entry.o: $(KERNEL_ENTRY)
	@mkdir -p $(@D)
	@echo "  ASM     $<"
	$(ASM) -f elf32 $< -o $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	@echo "  ASM     $<"
	$(ASM) -f elf32 $< -o $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "  CC      $<"
	$(CC) $(CFLAGS) -c $< -o $@


$(KERNEL_BIN): $(OBJ)
	@mkdir -p $(@D)
	@echo "  LD      $@"
	$(LD) $(LDFLAGS) $^ -o $@


run: $(DISK_IMAGE)
	@echo "  QEMU    $<"
	$(QEMU) $(QEMUFLAGS),file=$<


clean:
	rm -rf $(BUILD_DIR)
