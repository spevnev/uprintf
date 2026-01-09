prefix     := /usr/local
includedir := $(prefix)/include

BUILD_DIR    := build
LIB_DIR      := libs
EXAMPLE_DIR  := examples
TEST_DIR     := tests
BASELINE_DIR := tests/baselines

# Test flags
COMMON_FLAGS := -Wall -Wextra -Werror -pedantic -I . -DUPRINTF_TEST -fsanitize=undefined,address,leak -g2
CFLAGS       := $(COMMON_FLAGS) -std=c11
CXXFLAGS     := $(COMMON_FLAGS) -std=c++11

C_COMPILERS   ?= clang gcc
CXX_COMPILERS ?= clang++ g++
O_LEVELS      ?= O0 O2 O3 Os

C_TESTS   ?= $(patsubst $(TEST_DIR)/%.c, %, $(wildcard $(TEST_DIR)/*.c))
CXX_TESTS ?= $(patsubst $(TEST_DIR)/%.cc, %, $(wildcard $(TEST_DIR)/*.cc))

# Example flags
EXAMPLE_COMMON_FLAGS := -Wall -Wextra -pedantic -I . -fsanitize=undefined,address,leak -g2 -O0
EXAMPLE_CFLAGS       := $(EXAMPLE_COMMON_FLAGS) -std=c11
EXAMPLE_CXXFLAGS     := $(EXAMPLE_COMMON_FLAGS) -std=c++17

EXAMPLE_SRCS := $(wildcard $(EXAMPLE_DIR)/*.c $(EXAMPLE_DIR)/*.cc)
EXAMPLE_OUTS := $(addsuffix .out, $(basename $(EXAMPLE_SRCS)))
EXAMPLES     := $(addprefix $(BUILD_DIR)/, $(basename $(EXAMPLE_SRCS)))

.PHONY: clean install uninstall examples readme test tests
all: examples

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

install:
	install -D -m644 uprintf.h $(includedir)/uprintf.h

uninstall:
	rm $(includedir)/uprintf.h

# ====================== Examples =========================

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

$(BUILD_DIR)/$(EXAMPLE_DIR)/cxxopts: $(EXAMPLE_DIR)/cxxopts.cc $(LIB_DIR)/cxxopts.hpp uprintf.h
	@mkdir -p $(@D)
	$(CXX) $(EXAMPLE_CXXFLAGS) -I $(LIB_DIR) -o $@ $<

$(LIB_DIR)/cxxopts.hpp:
	@mkdir -p $(@D)
	wget https://raw.githubusercontent.com/jarro2783/cxxopts/refs/heads/master/include/cxxopts.hpp -O $@

$(BUILD_DIR)/$(EXAMPLE_DIR)/taskflow: $(EXAMPLE_DIR)/taskflow.cc $(LIB_DIR)/taskflow uprintf.h
	@mkdir -p $(@D)
	$(CXX) $(EXAMPLE_CXXFLAGS) -I $(LIB_DIR) -o $@ $<

$(LIB_DIR)/taskflow:
	@mkdir -p $(@D)
	git clone --depth 1 --branch v3.11.0 https://github.com/taskflow/taskflow $(LIB_DIR)/taskflow_src
	mv $(LIB_DIR)/taskflow_src/taskflow $@
	rm -rf $(LIB_DIR)/taskflow_src

# ===================== README ============================

readme: README.md.in $(EXAMPLE_OUTS)
	cp README.md.in README.md
	@for file in $(EXAMPLE_OUTS); do                                       \
		sed -i -e "\:^!$$file$$: {" -e "r $$file" -e "d" -e "}" README.md; \
	done

$(EXAMPLE_DIR)/%.out: $(BUILD_DIR)/$(EXAMPLE_DIR)/%
	./$< > $@

# ======================== Tests ==========================

test: tests
tests: $(foreach O,$(O_LEVELS),                                                               \
			$(foreach C,$(C_COMPILERS),$(foreach T,$(C_TESTS),$(BUILD_DIR)/test/$T/$C-$O))    \
			$(foreach C,$(CXX_COMPILERS),$(foreach T,$(CXX_TESTS),$(BUILD_DIR)/test/$T/$C-$O)))

define C_TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$2-$3: $(TEST_DIR)/$1.c $(BUILD_DIR)/impl/$2.o
	@./test.sh $1 "$1-$2-$3" $2 "$(CFLAGS) -$3" $$^ $$@ $(BASELINE_DIR)/$1.out
endef

define CXX_TEST_TEMPLATE
$(BUILD_DIR)/test/$1/$2-$3: $(TEST_DIR)/$1.cc $(BUILD_DIR)/impl/$2.o
	@./test.sh $1 "$1-$2-$3" $2 "$(CXXFLAGS) -$3" $$^ $$@ $(BASELINE_DIR)/$1.out
endef

$(foreach O,$(O_LEVELS),                               \
	$(foreach C,$(C_COMPILERS),                        \
		$(foreach T,$(C_TESTS),                        \
			$(eval $(call C_TEST_TEMPLATE,$T,$C,$O))   \
		)                                              \
	)                                                  \
	$(foreach C,$(CXX_COMPILERS),                      \
		$(foreach T,$(CXX_TESTS),                      \
			$(eval $(call CXX_TEST_TEMPLATE,$T,$C,$O)) \
		)                                              \
	)                                                  \
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
