prefix     := /usr/local
includedir := $(prefix)/include

CC     := gcc
CFLAGS := -O2 -g2 -std=c11 -Wall -Wextra -pedantic -I . -DUPRINTF_TEST -fsanitize=undefined,address,leak

BUILD_DIR    := build
LIB_DIR      := libs
EXAMPLE_DIR  := examples
TEST_DIR     := tests
BASELINE_DIR := tests/baselines

COMPILERS := clang gcc
O_LEVELS  := O0 O2 O3 Os
G_LEVELS  := g2 g3

EXAMPLE_SRCS := $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_OUTS := $(EXAMPLE_SRCS:.c=.out)
EXAMPLES     := $(patsubst %.c, $(BUILD_DIR)/%, $(EXAMPLE_SRCS))

TESTS        := $(patsubst $(TEST_DIR)/%.c, %, $(wildcard $(TEST_DIR)/*.c))

.PHONY: clean install uninstall examples readme test tests
all: examples

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

install:
	install -D -m644 uprintf.h $(includedir)/uprintf.h

uninstall:
	rm $(includedir)/uprintf.h


examples: $(EXAMPLES)

$(BUILD_DIR)/$(EXAMPLE_DIR)/vorbis: $(EXAMPLE_DIR)/vorbis.c $(LIB_DIR)/stb_vorbis.c uprintf.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(LIB_DIR) -o $@ $< -lm

$(LIB_DIR)/stb_vorbis.c:
	@mkdir -p $(@D)
	wget https://raw.githubusercontent.com/nothings/stb/master/stb_vorbis.c -O $@

$(BUILD_DIR)/$(EXAMPLE_DIR)/avl: $(EXAMPLE_DIR)/avl.c $(LIB_DIR)/avl uprintf.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(LIB_DIR)/avl -o $@ $< $(LIB_DIR)/avl/avl.c

$(LIB_DIR)/avl:
	@mkdir -p $(@D)
	git clone --depth 1 https://github.com/etherealvisage/avl $(LIB_DIR)/avl_src
	mv $(LIB_DIR)/avl_src/src $@
	rm -rf $(LIB_DIR)/avl_src

$(BUILD_DIR)/$(EXAMPLE_DIR)/sqlite: $(EXAMPLE_DIR)/sqlite.c $(LIB_DIR)/sqlite/sqlite3.c uprintf.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(LIB_DIR)/sqlite -o $@ $<

$(LIB_DIR)/sqlite/sqlite3.c:
	mkdir -p $(LIB_DIR)
	wget https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip -O $(LIB_DIR)/sqlite.zip
	cd $(LIB_DIR) && unzip sqlite.zip && mv sqlite-amalgamation-3460100 sqlite
	rm $(LIB_DIR)/sqlite.zip

$(BUILD_DIR)/$(EXAMPLE_DIR)/uprintf: $(EXAMPLE_DIR)/uprintf.c uprintf.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<


readme: README.md.in $(EXAMPLE_OUTS)
	cp README.md.in README.md
	@for file in $(EXAMPLE_OUTS); do                     \
		sed -i -e "\:^!$$file$$: {" -e "r $$file" -e "d" -e "}" README.md; \
	done

$(EXAMPLE_DIR)/%.out: $(BUILD_DIR)/$(EXAMPLE_DIR)/%
	./$< > $@

test: tests
tests: $(foreach C,$(COMPILERS),$(foreach O,$(O_LEVELS),$(foreach G,$(G_LEVELS),$(foreach T,$(TESTS),$(BUILD_DIR)/test/$T/$C-$O-$G))))

# Export all variables to subprocesses, i.e. test.sh
export

define TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$2-$3-$4: $(BUILD_DIR)/impl/$2.o $(TEST_DIR)/$1.c uprintf.h test.sh
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

define IMPL_TEMPLATE
$(BUILD_DIR)/impl/$1.o: uprintf.h
	@mkdir -p $$(@D)
	$1 $(CFLAGS) -DUPRINTF_IMPLEMENTATION -x c -c $$< -o $$@
endef

$(foreach C,$(COMPILERS),$(eval $(call IMPL_TEMPLATE,$C)))
