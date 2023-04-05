# FOR x86_64 BUILD:
#
# AS_FLAGS = -felf64
# CC = x86_64-elf-gcc
# AR = x86_64-elf-ar
# AR_FLAGS = crs --target=elf64-x86-64
# LINKER = x86_64-elf-ld
# LINKER_FLAGS = -T$(LD_SCRIPT) -nostdlib --nmagic
#
# YOUR QEMU MUST BE X86_64:
# qemu-system-x86_64
#
# YOUR KERNEL-CODE MUST CONTAIN:
# kernel/arch/boot/x86_64-boot.s - _start function must setup and jump to the long mode and then call kernel entry point.
#
# TOOLCHAIN:
# https://github.com/danidoco/x86_64-elf-gcc-toolchain

AS = nasm
AS_FLAGS = -felf32

CC = i686-elf-gcc
CC_FLAGS = -std=gnu99 -Os -Wall -Wextra -Werror -ffreestanding

AR = i686-elf-ar
AR_FLAGS = crs --target=elf32-i386

BUILD_SCRIPTS = scripts

LINKER = i686-elf-ld
LD_SCRIPT = $(BUILD_SCRIPTS)/kernel.ld
LINKER_FLAGS = -T$(LD_SCRIPT) -nostdlib --nmagic --oformat=elf32-i386

ASM_SRC = kernel/kernel/arch/boot/i386-boot.s
KERNEL_SRC = kernel/kernel/kernel.c \
			 kernel/kernel/dctors.c \
			 kernel/kernel/init.c   \
			 kernel/kernel/fini.c   \

######################################################################
# IF YOU WANT TO ADD SOME ADDITIONAL KERNEL MODULES CREATE DIR       #
# WITH  MAKEFILE AND ADD CORRESPONDING DIR'S NAME TO THE LIST BELOW: #
KERNEL_MODULES = libc                                                #
######################################################################

KERNEL_INCLUDE_DIRS = kernel
MODULES_INCLUDE_DIRS := $(addsuffix /include, $(KERNEL_MODULES))

# Each module must keep it's interface headers in $(MODULE_NAME)/$(MODULE_HEADERS_DIR) directory.
MODULE_HEADERS_DIR = include

CC_FLAGS += $(addprefix -I, $(KERNEL_INCLUDE_DIRS) $(MODULES_INCLUDE_DIRS) $(MODULE_HEADERS_DIR))

ISO_DIR = iso
BUILD_DIR = build

OBJECTS := $(addprefix $(BUILD_DIR)/, $(ASM_SRC:.s=.s.o) $(KERNEL_SRC:.c=.o))
MODULES_OBJECTS := $(addprefix $(BUILD_DIR)/, $(addsuffix .a, $(KERNEL_MODULES)))

IMAGE = ROS.iso
TARGET = ROS.elf

IMAGE_PATH := $(ISO_DIR)/$(IMAGE)
TARGET_PATH := $(BUILD_DIR)/$(TARGET)

.PHONY: all iso build run clean

all: iso

iso: $(TARGET_PATH)
	rm -f $(IMAGE_PATH)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(TARGET_PATH) $(ISO_DIR)/boot/$(TARGET)
	cp $(BUILD_SCRIPTS)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(IMAGE_PATH) $(ISO_DIR)

build: $(TARGET_PATH)

$(TARGET_PATH): $(OBJECTS) $(MODULES_OBJECTS) $(LD_SCRIPT)
	$(LINKER) $(LINKER_FLAGS) $(OBJECTS) $(MODULES_OBJECTS) -o $@
	# https://stackoverflow.com/questions/31453859/how-to-remove-a-specific-elf-section-without-stripping-other-symbols
	objcopy --remove-section .eh_frame $@
	objcopy --remove-section .symtab   $@
	objcopy --remove-section .comment  $@

$(BUILD_DIR)/%.a: %/Makefile
	$(MAKE) -C $(dir $<) KERNEL_BUILD=1 BUILD_DIR=$(BUILD_DIR) CC=$(CC) CC_FLAGS="$(CC_FLAGS)" AR=$(AR) AR_FLAGS="$(AR_FLAGS)"

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $< -o $@

run: $(IMAGE_PATH)
	qemu-system-i386 -cdrom $(IMAGE_PATH)

clean:
	rm -rf $(ISO_DIR)/*
	rm -rf $(BUILD_DIR)/*
