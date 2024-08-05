CC     := gcc
FLAGS  := -g3 -O2 -std=c17
CFLAGS := -Wall -Wextra -pedantic -I src -fsanitize=undefined,address,leak -DUPRINTF_TEST

BUILD_DIR    := build
TEST_DIR     := tests
BASELINE_DIR := tests/baselines

COMPILERS := clang-18 gcc
O_LEVELS  := O0 O1 O2 O3 Os
G_LEVELS  := g2 g3
C_STDS    := c99 c17

TESTS := $(patsubst $(TEST_DIR)/%.c, %, $(shell find $(TEST_DIR) -type f -name '*.c'))


.PHONY: all
all: tests

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: install
install:
	cp src/uprintf.h /usr/local/include/uprintf.h
	chmod 644 /usr/local/include/uprintf.h

.PHONY: uninstall
uninstall:
	rm /usr/local/include/uprintf.h

.PHONY: tests
tests: $(patsubst %, $(BUILD_DIR)/tests/%, $(TESTS))

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.c src/uprintf.h Makefile
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $<

.PHONY: all_tests
all_tests: $(foreach C,$(COMPILERS),$(foreach O,$(O_LEVELS),$(foreach G,$(G_LEVELS),$(foreach V,$(C_STDS),$(foreach T,$(TESTS),$(BUILD_DIR)/all/$T/$T-$C-$O-$G-$V)))))

# Export all variables to subprocesses, i.e. test.sh
export

define TEST_TEMPLATE
$(BUILD_DIR)/all/$1/$1-$2-$3-$4-$5: $(TEST_DIR)/$1.c src/uprintf.h Makefile
	@./test.sh $1 $2 $3 $4 $5
endef

$(foreach C,$(COMPILERS),                                        \
	$(foreach O,$(O_LEVELS),                                     \
		$(foreach G,$(G_LEVELS),                                 \
			$(foreach V,$(C_STDS),                               \
				$(foreach T,$(TESTS),                            \
					$(eval $(call TEST_TEMPLATE,$T,$C,$O,$G,$V)) \
				)                                                \
			)                                                    \
		)                                                        \
	)                                                            \
)
