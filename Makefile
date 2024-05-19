NAME := flo
BUILD_DIR ?= build

CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -ggdb
CFLAGS += -O2 -std=c11 -Iinclude

ALL := lib$(NAME).a lib$(NAME).so

.phony: all
all: $(ALL)

INPUTS := $(wildcard src/*.c)
OBJECTS := $(INPUTS:src/%.c=$(BUILD_DIR)/%.o)

lib$(NAME).a: $(OBJECTS)
	$(AR) rcs $@ $^

lib$(NAME).so: $(OBJECTS)
	$(AR) rcs $@ $^

lib$(BUILD_DIR)/%.o: src/%.c include/$(NAME)/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(OUT_FILE) $(BUILD_DIR)
