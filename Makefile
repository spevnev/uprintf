CC     := gcc
FLAGS  := -g3 -O2 -std=c17
CFLAGS := -Wall -Wextra -pedantic -Isrc -fsanitize=undefined,address,leak -DUPRINTF_TEST

BUILD_DIR    := build
TEST_DIR     := tests
BASELINE_DIR := tests/baselines

COMPILERS := clang-18 gcc
O_LEVELS  := -O0 -O1 -O2 -O3 -Os
G_LEVELS  := -g2 -g3
C_STDS    := -std=c99 -std=c17

TESTS := $(patsubst $(TEST_DIR)/%.c, %, $(shell find $(TEST_DIR) -type f -name '*.c'))


.PHONY: all
all: tests

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: tests
tests: $(patsubst %, $(BUILD_DIR)/tests/%, $(TESTS))

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.c src/uprintf.h Makefile
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $<
