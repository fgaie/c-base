OUT_FILE ?= lib.so
BUILD_DIR ?= build

CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -ggdb
CFLAGS += -std=c11 -Iinclude

ALL := $(OUT_FILE)

.phony: all
all: $(ALL)

OUT_REQS := $(wildcard src/*.c)
$(OUT_FILE): $(OUT_REQS:src/%.c=$(BUILD_DIR)/%.o)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: src/%.c include/base/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(OUT_FILE) $(BUILD_DIR)
