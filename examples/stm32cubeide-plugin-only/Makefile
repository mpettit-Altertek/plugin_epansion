PROJECT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
EXAMPLES_DIR := $(abspath $(PROJECT_DIR)/..)
REPO_ROOT := $(abspath $(PROJECT_DIR)/../..)

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

BUILD_DIR := $(PROJECT_DIR)/build/$(CONFIG)
TARGET := $(BUILD_DIR)/libplugin_example_$(CONFIG).a
OBJECT := $(BUILD_DIR)/plugin_only.o
SOURCE := $(PROJECT_DIR)/plugin_only.c

CFLAGS ?= -std=c11 -Wall -Wextra -Werror -ffunction-sections -fdata-sections
CFLAGS += -I$(EXAMPLES_DIR) -I$(REPO_ROOT)/include -DEXAMPLE_PLUGIN_SLOT=$(PLUGIN_SLOT)

.PHONY: all clean slot0 slot1 slot2 slot3

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p "$@"

$(OBJECT): $(SOURCE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(TARGET): $(OBJECT)
	$(AR) rcs "$@" "$<"

slot0:
	$(MAKE) CONFIG=slot0

slot1:
	$(MAKE) CONFIG=slot1

slot2:
	$(MAKE) CONFIG=slot2

slot3:
	$(MAKE) CONFIG=slot3

clean:
	rm -rf "$(PROJECT_DIR)/build"
