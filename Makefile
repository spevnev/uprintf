EXAMPLES := struct
#union primitive

CC      := gcc
FLAGS  := -g3 -O2 -std=c11
CFLAGS := -Wall -Wextra -pedantic -Isrc -fsanitize=undefined

BUILD_DIR := build
SRC_DIR   := examples
BIN_DIR   := $(BUILD_DIR)/examples

COMPILERS := clang-18 gcc
O_LEVELS  := -O0 -O1 -O2 -O3 -Os
G_LEVELS  := -g1 -g2 -g3
C_STDS    := -std=c99 -std=c11 -std=c17


.PHONY: all
all: examples

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all_examples
all_examples: $(patsubst %, $(BIN_DIR)/all/%, $(EXAMPLES))

.PHONY: examples
examples: $(patsubst %, $(BIN_DIR)/%, $(EXAMPLES))

$(BIN_DIR)/all/%: $(SRC_DIR)/%.c src/uprintf.h Makefile
	@mkdir -p $(@D)
	for cc in $(COMPILERS); do 											 \
		for o_level in $(O_LEVELS); do 									 \
			for g_level in $(G_LEVELS); do 								 \
				for c_std in $(C_STDS); do 								 \
					echo "\n$$cc $$o_level $$g_level $$c_std $<";	 	 \
					$$cc $$o_level $$g_level $$c_std $(CFLAGS) -o $@ $<; \
					./$@; 												 \
				done 													 \
			done														 \
		done 															 \
	done

$(BIN_DIR)/%: $(SRC_DIR)/%.c src/uprintf.h Makefile
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $<