MODULE_NAME = libc

KERNEL_BUILD ?= 0

CC ?=
CC_FLAGS ?=

AR ?=
AR_FLAGS ?=

MODULE_SRC = src/math.c   \
			 src/stdio.c  \
			 src/stdlib.c \
			 src/string.c \

ifeq ($(KERNEL_BUILD),1)
	MODULE_BUILD_DIR := ../$(BUILD_DIR)/$(MODULE_NAME)/
	MODULE_OBJECTS := $(addprefix $(MODULE_BUILD_DIR), $(MODULE_SRC:.c=.o))
	MODULE_TARGET_PATH := ../$(BUILD_DIR)/$(MODULE_NAME).a
else
	MODULE_BUILD_DIR := build
	MODULE_OBJECTS := $(addprefix $(MODULE_BUILD_DIR), $(MODULE_SRC:.c=.o))
	MODULE_TARGET_PATH := $(MODULE_BUILD_DIR)/$(MODULE_NAME).a
endif

.PHONY: all build clean

all: build

build: $(MODULE_TARGET_PATH)

$(MODULE_TARGET_PATH): $(MODULE_OBJECTS)
	$(AR) $(AR_FLAGS) $@ $(MODULE_OBJECTS)

$(MODULE_BUILD_DIR)%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -DKERNEL_BUILD=$(KERNEL_BUILD) -c $< -o $@

clean:
	rm -rf $(MODULE_BUILD_DIR)/*
