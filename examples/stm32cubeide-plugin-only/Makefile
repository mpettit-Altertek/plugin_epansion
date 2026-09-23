PROJECT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
EXAMPLES_DIR := $(abspath $(PROJECT_DIR)/..)
REPO_ROOT := $(abspath $(PROJECT_DIR)/../..)

ifeq ($(OS),Windows_NT)
ifneq ($(findstring sh,$(notdir $(SHELL))),sh)
$(error This Makefile requires a Unix-like shell such as sh.exe, bash, MSYS2, or WSL)
endif
endif

CONFIG ?= slot0

ifeq ($(CONFIG),slot0)
PLUGIN_SLOT := 0
else ifeq ($(CONFIG),slot1)
PLUGIN_SLOT := 1
else ifeq ($(CONFIG),slot2)
PLUGIN_SLOT := 2
else ifeq ($(CONFIG),slot3)
PLUGIN_SLOT := 3
else
$(error Unsupported CONFIG '$(CONFIG)'. Use slot0, slot1, slot2, or slot3)
endif

CC ?= arm-none-eabi-gcc
AR ?= arm-none-eabi-ar
LD ?= arm-none-eabi-ld

BUILD_DIR := $(PROJECT_DIR)/build/$(CONFIG)
TARGET := $(BUILD_DIR)/plugin_image_$(CONFIG).o
OBJECT := $(BUILD_DIR)/plugin_only.o
DEPFILE := $(BUILD_DIR)/plugin_only.d
SOURCE := $(EXAMPLES_DIR)/slot0_example_plugin.c
LINKER_SCRIPT := $(PROJECT_DIR)/plugin_layout.ld

CFLAGS ?= -std=c11 -Wall -Wextra -Werror -ffunction-sections -fdata-sections
CFLAGS += -MMD -MP -I$(EXAMPLES_DIR) -I$(REPO_ROOT)/include -DEXAMPLE_PLUGIN_SLOT=$(PLUGIN_SLOT)

.PHONY: all clean slot0 slot1 slot2 slot3

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p "$@"

$(OBJECT): $(SOURCE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(TARGET): $(OBJECT) $(LINKER_SCRIPT) | $(BUILD_DIR)
	$(LD) -r -T "$(LINKER_SCRIPT)" -o "$@" "$<"

slot0:
	$(MAKE) all CONFIG=slot0

slot1:
	$(MAKE) all CONFIG=slot1

slot2:
	$(MAKE) all CONFIG=slot2

slot3:
	$(MAKE) all CONFIG=slot3

clean:
	rm -rf "$(PROJECT_DIR)/build"

-include $(DEPFILE)
