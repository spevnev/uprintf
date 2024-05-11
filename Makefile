EXAMPLES := struct

CC      := gcc
CFLAGS  := -O2 -std=c11 -Wall -Wextra -pedantic -Isrc -g3
LDFLAGS := 

BUILD_DIR       := build
EXAMPLE_SRC_DIR := examples
EXAMPLE_BIN_DIR := $(BUILD_DIR)/examples
EXAMPLE_OBJ_DIR := $(BUILD_DIR)/objs


.PHONY: all
all: examples

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: examples
examples: $(patsubst %, $(EXAMPLE_BIN_DIR)/%, $(EXAMPLES))

$(EXAMPLE_BIN_DIR)/%: $(EXAMPLE_OBJ_DIR)/%.o
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(EXAMPLE_OBJ_DIR)/%.o: $(EXAMPLE_SRC_DIR)/%.c Makefile src/uprintf.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<
