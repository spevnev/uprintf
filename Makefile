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

.PHONY: all_tests
all_tests: $(patsubst %, $(BUILD_DIR)/all/%, $(TESTS))

$(BUILD_DIR)/all/%: $(TEST_DIR)/%.c src/uprintf.h Makefile
	@mkdir -p $@
	@rm -f $@/log
	@baseline="$(BASELINE_DIR)/$(@F).out";                                                       \
	sed -E "s/0x[0-9a-fA-F]{6,16}/POINTER/" -i $$baseline;                                       \
	for cc in $(COMPILERS); do                                                                   \
		for o_level in $(O_LEVELS); do                                                           \
			for g_level in $(G_LEVELS); do                                                       \
				for c_std in $(C_STDS); do                                                       \
					bin="$@/$(@F)-$$cc$$o_level$$g_level$$c_std";                                \
					log="$$bin.log";                                                             \
					output="$$bin.out";                                                          \
					diff="$$bin.diff";                                                           \
																								 \
					$$cc $(CFLAGS) -Werror $$o_level $$g_level $$c_std -o $$bin $< > $$log 2>&1; \
					ret=$$?;                                                                     \
					if [ $$ret -ne 0 ]; then                                                     \
						echo "[COMPILATION FAILED] $$log";                                       \
						exit 1;                                                                  \
					fi;                                                                          \
																								 \
					./$$bin > $$output 2>&1;                                                     \
					ret=$$?;                                                                     \
					if [ $$ret -ne 0 ]; then                                                     \
						echo "[TEST FAILED] $$log";                                              \
						exit 1;                                                                  \
					fi;                                                                          \
					cat $$output >> $$log;                                                       \
																								 \
					sed -E "s/0x[0-9a-fA-F]{6,16}/POINTER/" -i $$output;                         \
					similarity=$$(wdiff -123 --statistics $$baseline $$output | tail -n 1 |      \
						awk -F ':' '{print $$NF}' | awk -F ' ' '{print $$4}' | sed 's/%$$//');   \
					echo "Similiarity is $$similarity%" >> $$log;                                \
					if [ $$similarity -lt 95 ]; then                                             \
						wdiff $$baseline $$output > $$diff;                                      \
						echo "[DIFF FAILED] Similarity is $$similarity%, see $$diff. $$log.";    \
						exit 1;                                                                  \
					fi;                                                                          \
				done                                                                             \
			done                                                                                 \
		done                                                                                     \
	done
