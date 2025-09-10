prefix     := /usr/local
includedir := $(prefix)/include

BUILD_DIR    := build
LIB_DIR      := libs
EXAMPLE_DIR  := examples
TEST_DIR     := tests
BASELINE_DIR := tests/baselines


COMMON_FLAGS := -Wall -Wextra -Werror -pedantic -I . -DUPRINTF_TEST -fsanitize=undefined,address,leak
CFLAGS       := $(COMMON_FLAGS) -std=c11
CXXFLAGS     := $(COMMON_FLAGS) -std=c++11

C_COMPILERS   ?= clang gcc
CXX_COMPILERS ?= clang++ g++
O_LEVELS      ?= O0 O2 O3 Os
G_LEVELS      ?= g2 g3

C_TESTS   := $(patsubst $(TEST_DIR)/%.c, %, $(wildcard $(TEST_DIR)/*.c))
CXX_TESTS := $(patsubst $(TEST_DIR)/%.cc, %, $(wildcard $(TEST_DIR)/*.cc))


EXAMPLE_CFLAGS := -Wall -Wextra -pedantic -I . -fsanitize=undefined,address,leak -std=c11 -O2 -g2

EXAMPLE_SRCS := $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_OUTS := $(EXAMPLE_SRCS:.c=.out)
EXAMPLES     := $(patsubst %.c, $(BUILD_DIR)/%, $(EXAMPLE_SRCS))

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
	$(CC) $(EXAMPLE_CFLAGS) -I $(LIB_DIR) -o $@ $< -lm

$(LIB_DIR)/stb_vorbis.c:
	@mkdir -p $(@D)
	wget https://raw.githubusercontent.com/nothings/stb/master/stb_vorbis.c -O $@

$(BUILD_DIR)/$(EXAMPLE_DIR)/avl: $(EXAMPLE_DIR)/avl.c $(LIB_DIR)/avl uprintf.h
	@mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -I $(LIB_DIR)/avl -o $@ $< $(LIB_DIR)/avl/avl.c

$(LIB_DIR)/avl:
	@mkdir -p $(@D)
	git clone --depth 1 https://github.com/etherealvisage/avl $(LIB_DIR)/avl_src
	mv $(LIB_DIR)/avl_src/src $@
	rm -rf $(LIB_DIR)/avl_src

$(BUILD_DIR)/$(EXAMPLE_DIR)/sqlite: $(EXAMPLE_DIR)/sqlite.c $(LIB_DIR)/sqlite/sqlite3.c uprintf.h
	@mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -I $(LIB_DIR)/sqlite -o $@ $<

$(LIB_DIR)/sqlite/sqlite3.c:
	mkdir -p $(LIB_DIR)
	wget https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip -O $(LIB_DIR)/sqlite.zip
	cd $(LIB_DIR) && unzip sqlite.zip && mv sqlite-amalgamation-3460100 sqlite
	rm $(LIB_DIR)/sqlite.zip

$(BUILD_DIR)/$(EXAMPLE_DIR)/uprintf: $(EXAMPLE_DIR)/uprintf.c uprintf.h
	@mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -o $@ $<

$(BUILD_DIR)/$(EXAMPLE_DIR)/miniaudio: $(EXAMPLE_DIR)/miniaudio.c $(LIB_DIR)/miniaudio.h uprintf.h
	@mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -I $(LIB_DIR) -o $@ $< -lm -lpthread

$(LIB_DIR)/miniaudio.h:
	@mkdir -p $(@D)
	wget https://raw.githubusercontent.com/mackron/miniaudio/refs/heads/master/miniaudio.h -O $@


readme: README.md.in $(EXAMPLE_OUTS)
	cp README.md.in README.md
	@for file in $(EXAMPLE_OUTS); do                                       \
		sed -i -e "\:^!$$file$$: {" -e "r $$file" -e "d" -e "}" README.md; \
	done

$(EXAMPLE_DIR)/%.out: $(BUILD_DIR)/$(EXAMPLE_DIR)/%
	./$< > $@


test: tests
tests: $(foreach O,$(O_LEVELS),$(foreach G,$(G_LEVELS),\
			$(foreach C,$(C_COMPILERS),$(foreach T,$(C_TESTS),$(BUILD_DIR)/test/$T/$C-$O-$G)) \
			$(foreach C,$(CXX_COMPILERS),$(foreach T,$(CXX_TESTS),$(BUILD_DIR)/test/$T/$C-$O-$G))))

# Export all variables to subprocesses, i.e. test.sh
export

define C_TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$2-$3-$4: $(TEST_DIR)/$1.c $(BUILD_DIR)/impl/$2.o test.sh
	@./test.sh "$(CFLAGS)" $$< $1 $2 $3 $4
endef

define CXX_TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$2-$3-$4: $(TEST_DIR)/$1.cc $(BUILD_DIR)/impl/$2.o test.sh
	@./test.sh "$(CXXFLAGS)" $$< $1 $2 $3 $4
endef

$(foreach O,$(O_LEVELS),                                      \
	$(foreach G,$(G_LEVELS),                                  \
		$(foreach C,$(C_COMPILERS),                           \
			$(foreach T,$(C_TESTS),                           \
				$(eval $(call C_TEST_TEMPLATE,$T,$C,$O,$G))   \
			)                                                 \
		)                                                     \
		$(foreach C,$(CXX_COMPILERS),                         \
			$(foreach T,$(CXX_TESTS),                         \
				$(eval $(call CXX_TEST_TEMPLATE,$T,$C,$O,$G)) \
			)                                                 \
		)                                                     \
	)                                                         \
)

define C_IMPL_TEMPLATE
$(BUILD_DIR)/impl/$1.o: uprintf.h
	@mkdir -p $$(@D)
	$1 $(CFLAGS) -DUPRINTF_IMPLEMENTATION -x c -c $$< -o $$@
endef

define CXX_IMPL_TEMPLATE
$(BUILD_DIR)/impl/$1.o: $(BUILD_DIR)/uprintf.hh
	@mkdir -p $$(@D)
	$1 $(CXXFLAGS) -DUPRINTF_IMPLEMENTATION -x c++ -c $$< -o $$@
endef

$(BUILD_DIR)/uprintf.hh:
	@mkdir -p $(@D)
	ln -s $(shell pwd)/uprintf.h $@

$(foreach C,$(C_COMPILERS),$(eval $(call C_IMPL_TEMPLATE,$C)))
$(foreach C,$(CXX_COMPILERS),$(eval $(call CXX_IMPL_TEMPLATE,$C)))
