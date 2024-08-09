CC     := gcc
FLAGS  := -O2 -g2 -std=c99
CFLAGS := -Wall -Wextra -pedantic -I src -fsanitize=undefined,address,leak -DUPRINTF_TEST

BUILD_DIR    := build
LIB_DIR      := libs
EXAMPLE_DIR  := examples
TEST_DIR     := tests
BASELINE_DIR := tests/baselines

COMPILERS := clang-18 gcc
O_LEVELS  := O0 O2 O3 Os
G_LEVELS  := g2 g3

EXAMPLES := $(patsubst $(EXAMPLE_DIR)/%.c, %, $(shell find $(EXAMPLE_DIR) -type f -name '*.c'))
TESTS    := $(patsubst $(TEST_DIR)/%.c, %, $(shell find $(TEST_DIR) -type f -name '*.c'))


.PHONY: all
all: examples

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


.PHONY: examples
examples: $(patsubst %, $(BUILD_DIR)/$(EXAMPLE_DIR)/%, $(EXAMPLES))

$(BUILD_DIR)/$(EXAMPLE_DIR)/vorbis: $(EXAMPLE_DIR)/vorbis.c $(LIB_DIR)/stb_vorbis.c src/uprintf.h Makefile
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(CFLAGS) -I $(LIB_DIR) -o $@ $< -lm

$(LIB_DIR)/stb_vorbis.c:
	@mkdir -p $(@D)
	wget https://raw.githubusercontent.com/nothings/stb/master/stb_vorbis.c -O $@


.PHONY: tests
tests: $(foreach C,$(COMPILERS),$(foreach O,$(O_LEVELS),$(foreach G,$(G_LEVELS),$(foreach T,$(TESTS),$(BUILD_DIR)/test/$T/$T-$C-$O-$G))))

# Export all variables to subprocesses, i.e. test.sh
export

define TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$1-$2-$3-$4: $(TEST_DIR)/$1.c src/uprintf.h Makefile test.sh
	@./test.sh $1 $2 $3 $4
endef

$(foreach C,$(COMPILERS),                                 \
	$(foreach O,$(O_LEVELS),                              \
		$(foreach G,$(G_LEVELS),                          \
			$(foreach T,$(TESTS),                         \
				$(eval $(call TEST_TEMPLATE,$T,$C,$O,$G)) \
			)                                             \
		)                                                 \
	)                                                     \
)
