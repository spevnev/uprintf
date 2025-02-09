// Usage and other information can be found in README at https://github.com/spevnev/uprintf
// Bugs and suggestions can be submitted to https://github.com/spevnev/uprintf/issues

// MIT License
//
// Copyright (c) 2024 Serhii Pievniev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// ====================== CHECKS ==========================

// clang-format off
#ifndef __linux__
#error [ERROR] uprintf only supports Linux
#endif

#ifdef __cplusplus
#error [ERROR] uprintf does NOT support C++
#endif
// clang-format on

// ====================== HEADER ==========================

#ifndef UPRINTF_H
#define UPRINTF_H

void _upf_uprintf(const char *file, int line, const char *fmt, const char *args, ...);

// If variadic arguments were to be stringified directly, the arguments which
// use macros would stringify to the macro name instead of being expanded, but
// by calling another macro the argument-macros will be expanded and stringified
// to their real values.
#define _upf_stringify_va_args(...) #__VA_ARGS__

// The noop following uprintf is required to guarantee that return PC of the
// function is within the same scope. This is especially problematic if uprintf
// is the last call in the function because then its return PC is that of the
// caller, which optimizes two returns to one.
#define uprintf(fmt, ...)                                                                        \
    do {                                                                                         \
        _upf_uprintf(__FILE__, __LINE__, fmt, _upf_stringify_va_args(__VA_ARGS__), __VA_ARGS__); \
        __asm__ volatile("nop");                                                                 \
    } while (0)

#ifdef UPRINTF_TEST
extern int _upf_test_status;
#endif

#endif  // UPRINTF_H

// ====================== SOURCE ==========================

#ifdef UPRINTF_IMPLEMENTATION

// ===================== OPTIONS ==========================

#ifndef UPRINTF_INDENTATION_WIDTH
#define UPRINTF_INDENTATION_WIDTH 4
#endif

#ifndef UPRINTF_MAX_DEPTH
#define UPRINTF_MAX_DEPTH 10
#endif

#ifndef UPRINTF_IGNORE_STDIO_FILE
#define UPRINTF_IGNORE_STDIO_FILE true
#endif

#ifndef UPRINTF_ARRAY_COMPRESSION_THRESHOLD
#define UPRINTF_ARRAY_COMPRESSION_THRESHOLD 4
#endif

#ifndef UPRINTF_MAX_STRING_LENGTH
#define UPRINTF_MAX_STRING_LENGTH 200
#endif

// ===================== INCLUDES =========================

#define __USE_XOPEN_EXTENDED
#define _GNU_SOURCE

#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <link.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// =================== DECLARATIONS =======================

// Feature test macros might not work since it is possible that the header has
// been included before this one and thus expanded without the macro, so the
// functions must be declared here.

ssize_t readlink(const char *path, char *buf, size_t bufsiz);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// Part of dl_phdr_info used instead of it, since it may not be defined, but we
// also cannot define it ourselves since it could cause redefinition error.
typedef struct {
    Elf64_Addr dlpi_addr;
    const char *dlpi_name;
} _upf_dl_phdr_info;

struct dl_phdr_info;
int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data);

// ===================== dwarf.h ==========================

// dwarf.h's location is inconsistent and the package containing it may not be
// install, so we include a partial implementation of the header.

#define DW_UT_compile 0x01

#define DW_TAG_array_type 0x01
#define DW_TAG_enumeration_type 0x04
#define DW_TAG_formal_parameter 0x05
#define DW_TAG_lexical_block 0x0b
#define DW_TAG_member 0x0d
#define DW_TAG_pointer_type 0x0f
#define DW_TAG_compile_unit 0x11
#define DW_TAG_structure_type 0x13
#define DW_TAG_subroutine_type 0x15
#define DW_TAG_typedef 0x16
#define DW_TAG_union_type 0x17
#define DW_TAG_unspecified_parameters 0x18
#define DW_TAG_inlined_subroutine 0x1d
#define DW_TAG_subrange_type 0x21
#define DW_TAG_base_type 0x24
#define DW_TAG_const_type 0x26
#define DW_TAG_enumerator 0x28
#define DW_TAG_subprogram 0x2e
#define DW_TAG_variable 0x34
#define DW_TAG_volatile_type 0x35
#define DW_TAG_restrict_type 0x37
#define DW_TAG_atomic_type 0x47
#define DW_TAG_call_site 0x48
#define DW_TAG_call_site_parameter 0x49

#define DW_FORM_addr 0x01
#define DW_FORM_block2 0x03
#define DW_FORM_block4 0x04
#define DW_FORM_data2 0x05
#define DW_FORM_data4 0x06
#define DW_FORM_data8 0x07
#define DW_FORM_string 0x08
#define DW_FORM_block 0x09
#define DW_FORM_block1 0x0a
#define DW_FORM_data1 0x0b
#define DW_FORM_flag 0x0c
#define DW_FORM_sdata 0x0d
#define DW_FORM_strp 0x0e
#define DW_FORM_udata 0x0f
#define DW_FORM_ref_addr 0x10
#define DW_FORM_ref1 0x11
#define DW_FORM_ref2 0x12
#define DW_FORM_ref4 0x13
#define DW_FORM_ref8 0x14
#define DW_FORM_ref_udata 0x15
#define DW_FORM_indirect 0x16
#define DW_FORM_sec_offset 0x17
#define DW_FORM_exprloc 0x18
#define DW_FORM_flag_present 0x19
#define DW_FORM_strx 0x1a
#define DW_FORM_addrx 0x1b
#define DW_FORM_ref_sup4 0x1c
#define DW_FORM_strp_sup 0x1d
#define DW_FORM_data16 0x1e
#define DW_FORM_line_strp 0x1f
#define DW_FORM_ref_sig8 0x20
#define DW_FORM_implicit_const 0x21
#define DW_FORM_loclistx 0x22
#define DW_FORM_rnglistx 0x23
#define DW_FORM_ref_sup8 0x24
#define DW_FORM_strx1 0x25
#define DW_FORM_strx2 0x26
#define DW_FORM_strx3 0x27
#define DW_FORM_strx4 0x28
#define DW_FORM_addrx1 0x29
#define DW_FORM_addrx2 0x2a
#define DW_FORM_addrx3 0x2b
#define DW_FORM_addrx4 0x2c

#define DW_AT_name 0x03
#define DW_AT_byte_size 0x0b
#define DW_AT_bit_offset 0x0c
#define DW_AT_bit_size 0x0d
#define DW_AT_low_pc 0x11
#define DW_AT_high_pc 0x12
#define DW_AT_language 0x13
#define DW_AT_const_value 0x1c
#define DW_AT_upper_bound 0x2f
#define DW_AT_abstract_origin 0x31
#define DW_AT_count 0x37
#define DW_AT_data_member_location 0x38
#define DW_AT_encoding 0x3e
#define DW_AT_type 0x49
#define DW_AT_ranges 0x55
#define DW_AT_data_bit_offset 0x6b
#define DW_AT_str_offsets_base 0x72
#define DW_AT_addr_base 0x73
#define DW_AT_rnglists_base 0x74

#define DW_ATE_address 0x01
#define DW_ATE_boolean 0x02
#define DW_ATE_complex_float 0x03
#define DW_ATE_float 0x04
#define DW_ATE_signed 0x05
#define DW_ATE_signed_char 0x06
#define DW_ATE_unsigned 0x07
#define DW_ATE_unsigned_char 0x08
#define DW_ATE_imaginary_float 0x09
#define DW_ATE_packed_decimal 0x0a
#define DW_ATE_numeric_string 0x0b
#define DW_ATE_edited 0x0c
#define DW_ATE_signed_fixed 0x0d
#define DW_ATE_unsigned_fixed 0x0e
#define DW_ATE_decimal_float 0x0f
#define DW_ATE_UTF 0x10
#define DW_ATE_UCS 0x11
#define DW_ATE_ASCII 0x12

#define DW_RLE_end_of_list 0x00
#define DW_RLE_base_addressx 0x01
#define DW_RLE_startx_endx 0x02
#define DW_RLE_startx_length 0x03
#define DW_RLE_offset_pair 0x04
#define DW_RLE_base_address 0x05
#define DW_RLE_start_end 0x06
#define DW_RLE_start_length 0x07

#define DW_LANG_C 0x0002
#define DW_LANG_C89 0x0001
#define DW_LANG_C99 0x000c
#define DW_LANG_C11 0x001d
#define DW_LANG_C17 0x002c

// ===================== TESTING ==========================

#ifdef UPRINTF_TEST
int _upf_test_status = EXIT_SUCCESS;

#define _UPF_SET_TEST_STATUS(status) _upf_test_status = status
#else
#define _UPF_SET_TEST_STATUS(status) (void) status
#endif

// ====================== ERRORS ==========================

#define _UPF_LOG(type, ...)                         \
    do {                                            \
        fprintf(stderr, "(uprintf) [%s] ", (type)); \
        fprintf(stderr, __VA_ARGS__);               \
        fprintf(stderr, "\n");                      \
    } while (0)

#define _UPF_WARN(...)                      \
    do {                                    \
        _UPF_LOG("WARNING", __VA_ARGS__);   \
        _UPF_SET_TEST_STATUS(EXIT_FAILURE); \
    } while (0)

#define _UPF_ERROR(...)                            \
    do {                                           \
        _UPF_LOG("ERROR", __VA_ARGS__);            \
        _UPF_SET_TEST_STATUS(EXIT_FAILURE);        \
        longjmp(_upf_state.jmp_buf, EXIT_FAILURE); \
    } while (0)

#define _UPF_ASSERT(condition)                                                                        \
    do {                                                                                              \
        if (!(condition)) _UPF_ERROR("Assert (%s) failed at %s:%d.", #condition, __FILE__, __LINE__); \
    } while (0)

#define _UPF_OUT_OF_MEMORY() _UPF_ERROR("Process ran out of memory.")

// ====================== VECTOR ==========================

#define _UPF_INITIAL_VECTOR_CAPACITY 4

#define _UPF_VECTOR_TYPEDEF(name, type) \
    typedef struct {                    \
        uint32_t capacity;              \
        uint32_t length;                \
        type *data;                     \
    } name

#define _UPF_VECTOR_PUSH(vec, element)                                          \
    do {                                                                        \
        if ((vec)->capacity == 0) {                                             \
            (vec)->capacity = _UPF_INITIAL_VECTOR_CAPACITY;                     \
            uint32_t size = (vec)->capacity * sizeof(*(vec)->data);             \
            (vec)->data = _upf_arena_alloc(&_upf_state.arena, size);            \
        } else if ((vec)->capacity == (vec)->length) {                          \
            uint32_t old_size = (vec)->capacity * sizeof(*(vec)->data);         \
            (vec)->capacity *= 2;                                               \
            void *new_data = _upf_arena_alloc(&_upf_state.arena, old_size * 2); \
            memcpy(new_data, (vec)->data, old_size);                            \
            (vec)->data = new_data;                                             \
        }                                                                       \
        (vec)->data[(vec)->length++] = (element);                               \
    } while (0)

#define _UPF_VECTOR_COPY(dst, src)                               \
    do {                                                         \
        (dst)->capacity = (src)->length;                         \
        (dst)->length = (src)->length;                           \
        uint32_t size = (dst)->capacity * sizeof(*(dst)->data);  \
        (dst)->data = _upf_arena_alloc(&_upf_state.arena, size); \
        memcpy((dst)->data, (src)->data, size);                  \
    } while (0)

#define _UPF_VECTOR_TOP(vec) (vec)->data[(vec)->length - 1]

#define _UPF_VECTOR_POP(vec) (vec)->length--

// ====================== TYPES ===========================

typedef struct _upf_arena_region {
    uint8_t *data;
    size_t capacity;
    size_t length;
    struct _upf_arena_region *prev;
} _upf_arena_region;

typedef struct {
    _upf_arena_region *head;
} _upf_arena;

_UPF_VECTOR_TYPEDEF(_upf_size_t_vec, size_t);
_UPF_VECTOR_TYPEDEF(_upf_cstr_vec, const char *);

typedef struct {
    uint64_t name;
    uint64_t form;
    int64_t implicit_const;
} _upf_attr;

_UPF_VECTOR_TYPEDEF(_upf_attr_vec, _upf_attr);

typedef struct {
    uint64_t code;
    uint64_t tag;
    bool has_children;
    _upf_attr_vec attrs;
} _upf_abbrev;

_UPF_VECTOR_TYPEDEF(_upf_abbrev_vec, _upf_abbrev);

typedef struct _upf_type _upf_type;
_UPF_VECTOR_TYPEDEF(_upf_type_ptr_vec, struct _upf_type *);

enum _upf_type_kind {
    _UPF_TK_STRUCT,
    _UPF_TK_UNION,
    _UPF_TK_ENUM,
    _UPF_TK_ARRAY,
    _UPF_TK_POINTER,
    _UPF_TK_FUNCTION,
    _UPF_TK_U1,
    _UPF_TK_U2,
    _UPF_TK_U4,
    _UPF_TK_U8,
    _UPF_TK_S1,
    _UPF_TK_S2,
    _UPF_TK_S4,
    _UPF_TK_S8,
    _UPF_TK_F4,
    _UPF_TK_F8,
    _UPF_TK_BOOL,
    _UPF_TK_SCHAR,
    _UPF_TK_UCHAR,
    _UPF_TK_VOID,
    _UPF_TK_UNKNOWN
};

typedef struct {
    const char *name;
    _upf_type *type;
    size_t offset;
    int bit_size;  // non-zero means bit field
} _upf_member;

_UPF_VECTOR_TYPEDEF(_upf_member_vec, _upf_member);

typedef struct {
    const char *name;
    int64_t value;
} _upf_enum;

_UPF_VECTOR_TYPEDEF(_upf_enum_vec, _upf_enum);

#define _UPF_MOD_CONST 1 << 0
#define _UPF_MOD_VOLATILE 1 << 1
#define _UPF_MOD_RESTRICT 1 << 2
#define _UPF_MOD_ATOMIC 1 << 3

struct _upf_type {
    const char *name;
    enum _upf_type_kind kind;
    int modifiers;
    size_t size;
    union {
        struct {
            _upf_member_vec members;
        } cstruct;
        struct {
            _upf_type *underlying_type;
            _upf_enum_vec enums;
        } cenum;
        struct {
            _upf_type *element_type;
            _upf_size_t_vec lengths;
        } array;
        struct {
            _upf_type *type;
        } pointer;
        struct {
            _upf_type *return_type;
            _upf_type_ptr_vec arg_types;
        } function;
    } as;
};

typedef struct {
    uint64_t start;
    uint64_t end;
} _upf_range;

_UPF_VECTOR_TYPEDEF(_upf_range_vec, _upf_range);

typedef struct {
    const uint8_t *die;
    const char *name;
} _upf_named_type;

_UPF_VECTOR_TYPEDEF(_upf_named_type_vec, _upf_named_type);

typedef struct {
    const char *name;
    const uint8_t *return_type_die;
    _upf_named_type_vec args;
    bool is_variadic;
    uint64_t pc;
} _upf_function;

_UPF_VECTOR_TYPEDEF(_upf_function_vec, _upf_function);

typedef struct {
    const char *name;
    uint64_t pc;
} _upf_extern_function;

_UPF_VECTOR_TYPEDEF(_upf_extern_function_vec, _upf_extern_function);

typedef struct {
    const void *data;
    const _upf_type *type;
    bool is_visited;
    int id;
} _upf_indexed_struct;

_UPF_VECTOR_TYPEDEF(_upf_indexed_struct_vec, _upf_indexed_struct);

struct _upf_scope;
_UPF_VECTOR_TYPEDEF(_upf_scope_vec, struct _upf_scope);

typedef struct _upf_scope {
    _upf_range_vec ranges;
    _upf_named_type_vec vars;
    _upf_scope_vec scopes;
} _upf_scope;

typedef struct {
    int depth;
    _upf_scope *scope;
} _upf_scope_stack_entry;

_UPF_VECTOR_TYPEDEF(_upf_scope_stack, _upf_scope_stack_entry);

typedef struct {
    const uint8_t *die;
    _upf_type *type_ptr;
} _upf_type_map_entry;

_UPF_VECTOR_TYPEDEF(_upf_type_map_vec, _upf_type_map_entry);

typedef struct {
    const uint8_t *base;
    _upf_scope scope;

    uint64_t addr_base;
    uint64_t str_offsets_base;
    uint64_t rnglists_base;

    _upf_abbrev_vec abbrevs;
    _upf_named_type_vec types;
    _upf_function_vec functions;
} _upf_cu;

_UPF_VECTOR_TYPEDEF(_upf_cu_vec, _upf_cu);

enum _upf_token_kind {
    _UPF_TOK_NONE,
    _UPF_TOK_NUMBER,
    _UPF_TOK_STRING,
    _UPF_TOK_ID,
    _UPF_TOK_TYPE_SPECIFIER,
    _UPF_TOK_TYPE_QUALIFIER,
    _UPF_TOK_OPEN_PAREN,
    _UPF_TOK_CLOSE_PAREN,
    _UPF_TOK_OPEN_BRACKET,
    _UPF_TOK_CLOSE_BRACKET,
    _UPF_TOK_COMMA,
    _UPF_TOK_DOT,
    _UPF_TOK_ARROW,
    _UPF_TOK_INCREMENT,
    _UPF_TOK_DECREMENT,
    _UPF_TOK_PLUS,
    _UPF_TOK_MINUS,
    _UPF_TOK_STAR,
    _UPF_TOK_EXCLAMATION,
    _UPF_TOK_TILDE,
    _UPF_TOK_AMPERSAND,
    _UPF_TOK_QUESTION,
    _UPF_TOK_COLON,
    _UPF_TOK_ASSIGNMENT,
    _UPF_TOK_COMPARISON,
    _UPF_TOK_MATH
};

typedef struct {
    enum _upf_token_kind kind;
    const char *string;
} _upf_token;

_UPF_VECTOR_TYPEDEF(_upf_token_vec, _upf_token);

typedef struct {
    _upf_token_vec tokens;
    size_t idx;
} _upf_tokenizer;

enum _upf_base_type {
    _UPF_BT_INT_TYPE,
    _UPF_BT_TYPE,
    _UPF_BT_VARIABLE,
    _UPF_BT_FUNCTION,
};

typedef struct {
    _upf_cu *cu;
    int dereference;
    int suffix_calls;
    union {
        const char *name;
        _upf_type *type;
    } base;
    enum _upf_base_type base_type;
    _upf_cstr_vec members;
} _upf_parser_state;

// =================== GLOBAL STATE =======================

struct _upf_state {
    _upf_arena arena;
    // has _upf_init finished
    bool is_init;
    // file loaded by dynamic linker (only with info required to run it)
    const uint8_t *base;
    // mmap-ed file (with debug info)
    uint8_t *file;
    off_t file_size;
    // DWARF info
    bool is64bit;
    uint8_t offset_size;
    uint8_t address_size;
    // DWARF sections
    const uint8_t *die;
    size_t die_size;
    const uint8_t *abbrev;
    const char *str;
    const char *line_str;
    const uint8_t *str_offsets;
    const uint8_t *addr;
    const uint8_t *rnglists;
    // parsed DWARF info
    _upf_type_map_vec type_map;
    _upf_cu_vec cus;
    _upf_extern_function_vec extern_functions;
    // sequential id for handling circular structs
    int circular_id;
    // valid memory regions
    _upf_range_vec addresses;
    // bprintf
    char *buffer;
    int size;
    char *ptr;
    int free;
    // error handling
    jmp_buf jmp_buf;
    const char *file_path;
    int line;
};

static struct _upf_state _upf_state = {0};

// ====================== ARENA ===========================

#define _UPF_INITIAL_ARENA_SIZE 65535  // 64kb

static _upf_arena_region *_upf_arena_alloc_region(size_t capacity, _upf_arena_region *prev) {
    _upf_arena_region *region = (_upf_arena_region *) malloc(sizeof(*region));
    if (region == NULL) _UPF_OUT_OF_MEMORY();
    region->capacity = capacity;
    region->data = (uint8_t *) malloc(region->capacity * sizeof(*region->data));
    if (region->data == NULL) _UPF_OUT_OF_MEMORY();
    region->length = 0;
    region->prev = prev;
    return region;
}

static void *_upf_arena_alloc(_upf_arena *a, size_t size) {
    _UPF_ASSERT(a != NULL);

    if (a->head == NULL) {
        size_t capacity = _UPF_INITIAL_ARENA_SIZE;
        if (size > capacity) capacity = size;
        a->head = _upf_arena_alloc_region(capacity, NULL);
    }

    int alignment = a->head->length % sizeof(void *);
    if (alignment > 0) alignment = sizeof(void *) - alignment;

    if (alignment + size > a->head->capacity - a->head->length) {
        size_t new_capacity = a->head->capacity * 2;
        if (size > new_capacity) new_capacity = size;
        a->head = _upf_arena_alloc_region(new_capacity, a->head);
        alignment = 0;
    }

    void *memory = a->head->data + a->head->length + alignment;
    a->head->length += alignment + size;
    return memory;
}

static void _upf_arena_free(_upf_arena *a) {
    if (a == NULL || a->head == NULL) return;

    _upf_arena_region *region = a->head;
    while (region != NULL) {
        _upf_arena_region *prev = region->prev;

        free(region->data);
        free(region);

        region = prev;
    }

    a->head = NULL;
}

// Copies [begin, end) to arena-allocated string
static char *_upf_arena_string(_upf_arena *a, const char *begin, const char *end) {
    _UPF_ASSERT(a != NULL && begin != NULL && end != NULL);

    size_t len = end - begin;

    char *string = (char *) _upf_arena_alloc(a, len + 2);
    memcpy(string, begin, len);
    string[len] = '\0';

    return string;
}

static char *_upf_arena_concat2(_upf_arena *a, ...) {
    _UPF_ASSERT(a != NULL);

    va_list va_args;

    size_t size = 0;
    va_start(va_args, a);
    while (true) {
        const char *str = va_arg(va_args, const char *);
        if (str == NULL) break;
        size += strlen(str);
    }
    va_end(va_args);

    char *result = _upf_arena_alloc(a, size + 1);
    result[size] = '\0';

    char *p = result;
    va_start(va_args, a);
    while (true) {
        const char *str = va_arg(va_args, const char *);
        if (str == NULL) break;

        size_t len = strlen(str);
        memcpy(p, str, len);
        p += len;
    }
    va_end(va_args);

    return result;
}

#define _upf_arena_concat(a, ...) _upf_arena_concat2((a), __VA_ARGS__, NULL)

// ====================== DWARF ===========================

// Converts unsigned LEB128 to uint64_t and returns the size of LEB in bytes
static size_t _upf_uLEB_to_uint64(const uint8_t *leb, uint64_t *result) {
    _UPF_ASSERT(leb != NULL && result != NULL);

    static const uint8_t BITS_MASK = 0x7f;      // 01111111
    static const uint8_t CONTINUE_MASK = 0x80;  // 10000000

    size_t i = 0;
    int shift = 0;

    *result = 0;
    while (true) {
        uint8_t b = leb[i++];
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        if ((b & CONTINUE_MASK) == 0) break;
        shift += 7;
    }

    return i;
}

// Converts signed LEB128 to int64_t and returns the size of LEB in bytes
static size_t _upf_LEB_to_int64(const uint8_t *leb, int64_t *result) {
    _UPF_ASSERT(leb != NULL && result != NULL);

    static const uint8_t BITS_MASK = 0x7f;      // 01111111
    static const uint8_t CONTINUE_MASK = 0x80;  // 10000000
    static const uint8_t SIGN_MASK = 0x40;      // 01000000

    size_t i = 0;
    uint8_t b = 0;
    size_t shift = 0;

    *result = 0;
    while (true) {
        b = leb[i++];
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        shift += 7;
        if ((b & CONTINUE_MASK) == 0) break;
    }
    if ((shift < sizeof(*result) * 8) && (b & SIGN_MASK)) *result |= -(1 << shift);

    return i;
}

static bool _upf_is_primitive(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);

    switch (type->kind) {
        case _UPF_TK_STRUCT:
        case _UPF_TK_UNION:
        case _UPF_TK_ARRAY:
            return false;
        case _UPF_TK_FUNCTION:
        case _UPF_TK_ENUM:
        case _UPF_TK_POINTER:
        case _UPF_TK_U1:
        case _UPF_TK_U2:
        case _UPF_TK_U4:
        case _UPF_TK_U8:
        case _UPF_TK_S1:
        case _UPF_TK_S2:
        case _UPF_TK_S4:
        case _UPF_TK_S8:
        case _UPF_TK_F4:
        case _UPF_TK_F8:
        case _UPF_TK_BOOL:
        case _UPF_TK_SCHAR:
        case _UPF_TK_UCHAR:
        case _UPF_TK_VOID:
        case _UPF_TK_UNKNOWN:
            return true;
    }
    _UPF_ERROR("Invalid type: %d.", type->kind);
}

static uint64_t _upf_offset_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);

    uint64_t offset = 0;
    memcpy(&offset, die, _upf_state.offset_size);
    return offset;
}

static uint64_t _upf_address_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);

    uint64_t address = 0;
    memcpy(&address, die, _upf_state.address_size);
    return address;
}

static size_t _upf_get_attr_size(const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(die != NULL);

    switch (form) {
        case DW_FORM_addr:
            return _upf_state.address_size;
        case DW_FORM_strx1:
        case DW_FORM_addrx1:
        case DW_FORM_flag:
        case DW_FORM_ref1:
        case DW_FORM_data1:
            return 1;
        case DW_FORM_strx2:
        case DW_FORM_addrx2:
        case DW_FORM_ref2:
        case DW_FORM_data2:
            return 2;
        case DW_FORM_strx3:
        case DW_FORM_addrx3:
            return 3;
        case DW_FORM_ref_sup4:
        case DW_FORM_strx4:
        case DW_FORM_addrx4:
        case DW_FORM_ref4:
        case DW_FORM_data4:
            return 4;
        case DW_FORM_ref_sig8:
        case DW_FORM_ref_sup8:
        case DW_FORM_ref8:
        case DW_FORM_data8:
            return 8;
        case DW_FORM_data16:
            return 16;
        case DW_FORM_block1: {
            uint8_t length;
            memcpy(&length, die, sizeof(length));
            return sizeof(length) + length;
        }
        case DW_FORM_block2: {
            uint16_t length;
            memcpy(&length, die, sizeof(length));
            return sizeof(length) + length;
        }
        case DW_FORM_block4: {
            uint32_t length;
            memcpy(&length, die, sizeof(length));
            return sizeof(length) + length;
        }
        case DW_FORM_sdata: {
            int64_t result;
            return _upf_LEB_to_int64(die, &result);
        } break;
        case DW_FORM_loclistx:
        case DW_FORM_rnglistx:
        case DW_FORM_addrx:
        case DW_FORM_strx:
        case DW_FORM_ref_udata:
        case DW_FORM_udata: {
            uint64_t result;
            return _upf_uLEB_to_uint64(die, &result);
        } break;
        case DW_FORM_string:
            return strlen((const char *) die) + 1;
        case DW_FORM_exprloc:
        case DW_FORM_block: {
            uint64_t length;
            size_t leb_size = _upf_uLEB_to_uint64(die, &length);
            return leb_size + length;
        } break;
        case DW_FORM_line_strp:
        case DW_FORM_strp_sup:
        case DW_FORM_sec_offset:
        case DW_FORM_ref_addr:
        case DW_FORM_strp:
            return _upf_state.offset_size;
        case DW_FORM_indirect: {
            uint64_t form;
            size_t offset = _upf_uLEB_to_uint64(die, &form);
            return _upf_get_attr_size(die + offset, form);
        } break;
        case DW_FORM_flag_present:
        case DW_FORM_implicit_const:
            return 0;
    }
    _UPF_ERROR("Invalid DWARF attribute type(form): %lu.", form);
}

static uint64_t _upf_get_x_offset(const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(die != NULL);

    uint64_t offset = 0;
    switch (form) {
        case DW_FORM_strx1:
        case DW_FORM_addrx1:
            memcpy(&offset, die, 1);
            return offset;
        case DW_FORM_strx2:
        case DW_FORM_addrx2:
            memcpy(&offset, die, 2);
            return offset;
        case DW_FORM_strx3:
        case DW_FORM_addrx3:
            memcpy(&offset, die, 3);
            return offset;
        case DW_FORM_strx4:
        case DW_FORM_addrx4:
            memcpy(&offset, die, 4);
            return offset;
        case DW_FORM_addrx:
        case DW_FORM_strx:
            _upf_uLEB_to_uint64(die, &offset);
            return offset;
    }
    _UPF_ERROR("Invalid DWARF x-* type(form): %lu.", form);
}

static const char *_upf_get_str(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    switch (form) {
        case DW_FORM_strp:
            return _upf_state.str + _upf_offset_cast(die);
        case DW_FORM_line_strp:
            _UPF_ASSERT(_upf_state.line_str != NULL);
            return _upf_state.line_str + _upf_offset_cast(die);
        case DW_FORM_string:
            return (const char *) die;
        case DW_FORM_strx:
        case DW_FORM_strx1:
        case DW_FORM_strx2:
        case DW_FORM_strx3:
        case DW_FORM_strx4: {
            _UPF_ASSERT(_upf_state.str_offsets != NULL && cu->str_offsets_base != UINT64_MAX);
            uint64_t offset = _upf_get_x_offset(die, form) * _upf_state.offset_size;
            return _upf_state.str + _upf_offset_cast(_upf_state.str_offsets + cu->str_offsets_base + offset);
        }
    }
    _UPF_ERROR("Invalid DWARF string type(form): %lu.", form);
}

static uint64_t _upf_get_ref(const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(die != NULL);

    uint64_t ref = 0;
    switch (form) {
        case DW_FORM_ref1:
        case DW_FORM_ref2:
        case DW_FORM_ref4:
        case DW_FORM_ref8:
            memcpy(&ref, die, _upf_get_attr_size(die, form));
            return ref;
        case DW_FORM_ref_udata:
            _upf_uLEB_to_uint64(die, &ref);
            return ref;
    }
    _UPF_ERROR("Only references within single compilation unit are supported.");
}

static bool _upf_is_data(uint64_t form) {
    switch (form) {
        case DW_FORM_data1:
        case DW_FORM_data2:
        case DW_FORM_data4:
        case DW_FORM_data8:
        case DW_FORM_data16:
        case DW_FORM_implicit_const:
        case DW_FORM_sdata:
        case DW_FORM_udata:
            return true;
        default:
            return false;
    }
}

static int64_t _upf_get_data(const uint8_t *die, _upf_attr attr) {
    _UPF_ASSERT(die != NULL);

    switch (attr.form) {
        case DW_FORM_data1:
        case DW_FORM_data2:
        case DW_FORM_data4:
        case DW_FORM_data8: {
            int64_t data = 0;
            memcpy(&data, die, _upf_get_attr_size(die, attr.form));
            return data;
        } break;
        case DW_FORM_data16:
            _UPF_ERROR("16 byte data blocks aren't supported.");
        case DW_FORM_implicit_const:
            return attr.implicit_const;
        case DW_FORM_sdata: {
            int64_t data = 0;
            _upf_LEB_to_int64(die, &data);
            return data;
        } break;
        case DW_FORM_udata: {
            uint64_t data = 0;
            _upf_uLEB_to_uint64(die, &data);
            return data;
        } break;
    }
    _UPF_ERROR("Invalid DWARF data type(form): %lu.", attr.form);
}

static bool _upf_is_addr(uint64_t form) {
    switch (form) {
        case DW_FORM_addr:
        case DW_FORM_addrx:
        case DW_FORM_addrx1:
        case DW_FORM_addrx2:
        case DW_FORM_addrx3:
        case DW_FORM_addrx4:
            return true;
        default:
            return false;
    }
}

static uint64_t _upf_get_addr(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    switch (form) {
        case DW_FORM_addr:
            return _upf_address_cast(die);
        case DW_FORM_addrx1:
        case DW_FORM_addrx2:
        case DW_FORM_addrx3:
        case DW_FORM_addrx4:
        case DW_FORM_addrx: {
            _UPF_ASSERT(_upf_state.addr != NULL);
            uint64_t offset = cu->addr_base + _upf_get_x_offset(die, form) * _upf_state.address_size;
            return _upf_address_cast(_upf_state.addr + offset);
        }
    }
    _UPF_ERROR("Invalid DWARF address type(form): %lu.", form);
}

static const uint8_t *_upf_skip_die(const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(die != NULL && abbrev != NULL);

    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        die += _upf_get_attr_size(die, abbrev->attrs.data[i].form);
    }

    return die;
}

static size_t _upf_get_abbrev(const _upf_abbrev **abbrev, const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(abbrev != NULL && cu != NULL);

    uint64_t code;
    size_t offset = _upf_uLEB_to_uint64(die, &code);
    *abbrev = code == 0 ? NULL : &cu->abbrevs.data[code - 1];
    return offset;
}

static _upf_abbrev_vec _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _UPF_ASSERT(abbrev_table != NULL);

    _upf_abbrev_vec abbrevs = {0};
    while (true) {
        _upf_abbrev abbrev = {
            .code = UINT64_MAX,
            .tag = UINT64_MAX,
            .has_children = false,
        };
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.code);
        if (abbrev.code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_children = *abbrev_table;
        abbrev_table += sizeof(abbrev.has_children);

        while (true) {
            _upf_attr attr = {0};
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.name);
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.form);
            if (attr.form == DW_FORM_implicit_const) abbrev_table += _upf_LEB_to_int64(abbrev_table, &attr.implicit_const);
            if (attr.name == 0 && attr.form == 0) break;
            _UPF_VECTOR_PUSH(&abbrev.attrs, attr);
        }

        _UPF_VECTOR_PUSH(&abbrevs, abbrev);
    }

    return abbrevs;
}

static enum _upf_type_kind _upf_get_type_kind(int64_t encoding, int64_t size) {
    switch (encoding) {
        case DW_ATE_boolean:
            if (size == 1) return _UPF_TK_BOOL;
            _UPF_WARN("Expected boolean to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_address:
            _UPF_WARN("Segmented addresses aren't supported. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed:
            if (size == 1) return _UPF_TK_S1;
            if (size == 2) return _UPF_TK_S2;
            if (size == 4) return _UPF_TK_S4;
            if (size == 8) return _UPF_TK_S8;
            if (size == 16) {
                _UPF_WARN("16 byte integers aren't supported. Ignoring this type.");
                return _UPF_TK_UNKNOWN;
            }
            _UPF_WARN("Expected signed integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed_char:
            if (size == 1) return _UPF_TK_SCHAR;
            _UPF_WARN("Expected char to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_unsigned:
            if (size == 1) return _UPF_TK_U1;
            if (size == 2) return _UPF_TK_U2;
            if (size == 4) return _UPF_TK_U4;
            if (size == 8) return _UPF_TK_U8;
            if (size == 16) {
                _UPF_WARN("16 byte unsigned integers aren't supported. Ignoring this type.");
                return _UPF_TK_UNKNOWN;
            }
            _UPF_WARN("Expected unsigned integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_unsigned_char:
            if (size == 1) return _UPF_TK_UCHAR;
            _UPF_WARN("Expected char to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_ASCII:
        case DW_ATE_UCS:
        case DW_ATE_UTF:
            _UPF_WARN("C shouldn't have character encodings besides DW_ATE_(un)signed_char. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed_fixed:
        case DW_ATE_unsigned_fixed:
            _UPF_WARN("C shouldn't have fixed-point decimals. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_float:
            if (size == 4) return _UPF_TK_F4;
            if (size == 8) return _UPF_TK_F8;
            if (size == 16) {
                _UPF_WARN("16 byte floats aren't supported. Ignoring this type.");
                return _UPF_TK_UNKNOWN;
            }
            _UPF_WARN("Expected floats to be 4, 8 or 16 bytes long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_complex_float:
            _UPF_WARN("Complex floats aren't supported. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_imaginary_float:
            _UPF_WARN("Imaginary floats aren't supported. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_decimal_float:
            _UPF_WARN("Decimal floats aren't supported. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_packed_decimal:
            _UPF_WARN("C shouldn't have packed decimals. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_numeric_string:
            _UPF_WARN("C shouldn't have numeric strings. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_edited:
            _UPF_WARN("C shouldn't have edited strings. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        default:
            _UPF_WARN("Skipping unknown DWARF type encoding (0x%02lx)", encoding);
            return _UPF_TK_UNKNOWN;
    }
}

static int _upf_get_type_modifier(uint64_t tag) {
    // clang-format off
    switch (tag) {
        case DW_TAG_const_type:    return _UPF_MOD_CONST;
        case DW_TAG_volatile_type: return _UPF_MOD_VOLATILE;
        case DW_TAG_restrict_type: return _UPF_MOD_RESTRICT;
        case DW_TAG_atomic_type:   return _UPF_MOD_ATOMIC;
    }
    // clang-format on
    _UPF_ERROR("Invalid DWARF type modifier: %lu.", tag);
}

static _upf_type *_upf_add_type(const uint8_t *type_die, _upf_type type) {
    if (type_die != NULL) {
        for (uint32_t i = 0; i < _upf_state.type_map.length; i++) {
            _upf_type_map_entry entry = _upf_state.type_map.data[i];
            if (entry.die == type_die) return entry.type_ptr;
        }
    }

    _upf_type *type_ptr = _upf_arena_alloc(&_upf_state.arena, sizeof(type));
    memcpy(type_ptr, &type, sizeof(type));

    _upf_type_map_entry entry = {
        .die = type_die,
        .type_ptr = type_ptr,
    };
    _UPF_VECTOR_PUSH(&_upf_state.type_map, entry);

    return type_ptr;
}

static _upf_type _upf_get_subarray(const _upf_type *array, int count) {
    _UPF_ASSERT(array != NULL);

    _upf_type subarray = *array;
    if (array->name != NULL) {
        subarray.name = _upf_arena_string(&_upf_state.arena, array->name, array->name + strlen(array->name) - 2 * count);
    }
    subarray.as.array.lengths.length -= count;
    subarray.as.array.lengths.data += count;

    return subarray;
}

static _upf_type *_upf_get_return_type(_upf_type *type, int count) {
    while (count-- > 0) {
        while (type->kind == _UPF_TK_POINTER) type = type->as.pointer.type;
        if (type->kind != _UPF_TK_FUNCTION) {
            _UPF_ERROR("Unable to get return type of \"%s\" because it is not a function pointer.", type->name);
        }
        type = type->as.function.return_type;
    }

    return type;
}

static _upf_type *_upf_get_void_type(void) {
    static _upf_type *type_ptr = NULL;
    if (type_ptr != NULL) return type_ptr;

    _upf_type type = {
        .name = "void",
        .kind = _UPF_TK_VOID,
        .size = UINT64_MAX,
    };
    type_ptr = _upf_add_type(NULL, type);

    return type_ptr;
}

static _upf_type *_upf_parse_type(const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    for (uint32_t i = 0; i < _upf_state.type_map.length; i++) {
        _upf_type_map_entry entry = _upf_state.type_map.data[i];
        if (entry.die == die) return entry.type_ptr;
    }

    const uint8_t *die_base = die;

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, cu, die);

    const char *name = NULL;
    uint64_t subtype_offset = UINT64_MAX;
    size_t size = UINT64_MAX;
    int64_t encoding = 0;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            name = _upf_get_str(cu, die, attr.form);
        } else if (attr.name == DW_AT_type) {
            subtype_offset = _upf_get_ref(die, attr.form);
        } else if (attr.name == DW_AT_byte_size) {
            if (_upf_is_data(attr.form)) {
                size = _upf_get_data(die, attr);
            } else {
                _UPF_WARN("Non-constant type sizes aren't supported. Marking size as unknown.");
            }
        } else if (attr.name == DW_AT_encoding) {
            encoding = _upf_get_data(die, attr);
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    switch (abbrev->tag) {
        case DW_TAG_array_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);

            _upf_type *element_type = _upf_parse_type(cu, cu->base + subtype_offset);

            _upf_type type = {
                .name = name,
                .kind = _UPF_TK_ARRAY,
                .size = size,
                .as.array = {
                    .element_type = element_type,
                },
            };

            bool generate_name = element_type->name != NULL && type.name == NULL;
            if (generate_name) type.name = element_type->name;

            bool is_static = true;
            size_t array_size = element_type->size;
            if (!abbrev->has_children) return _upf_add_type(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;

                if (abbrev->tag == DW_TAG_enumeration_type) {
                    _UPF_WARN("Enumerator array subranges aren't unsupported. Ignoring this type.");
                    goto unknown_type;
                }
                _UPF_ASSERT(abbrev->tag == DW_TAG_subrange_type);

                size_t length = UINT64_MAX;
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];

                    if (attr.name == DW_AT_count || attr.name == DW_AT_upper_bound) {
                        if (_upf_is_data(attr.form)) {
                            length = _upf_get_data(die, attr);
                            if (attr.name == DW_AT_upper_bound) length++;
                        } else {
                            _UPF_WARN("Non-constant array lengths aren't supported. Marking it unknown.");
                        }
                    }

                    die += _upf_get_attr_size(die, attr.form);
                }

                if (length == UINT64_MAX) {
                    is_static = false;
                    array_size = UINT64_MAX;
                    type.as.array.lengths.length = 0;
                }

                if (is_static) {
                    array_size *= length;
                    _UPF_VECTOR_PUSH(&type.as.array.lengths, length);
                }

                if (generate_name) type.name = _upf_arena_concat(&_upf_state.arena, type.name, "[]");
            }

            if (element_type->size != UINT64_MAX && type.size == UINT64_MAX) {
                type.size = array_size;
            }

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_enumeration_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);

            _upf_type type = {
                .name = name ? name : "enum",
                .kind = _UPF_TK_ENUM,
                .size = size,
                .as.cenum = {
                    .underlying_type = _upf_parse_type(cu, cu->base + subtype_offset),
                },
            };

            if (type.size == UINT64_MAX) type.size = type.as.cenum.underlying_type->size;

            if (!abbrev->has_children) return _upf_add_type(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                _UPF_ASSERT(abbrev->tag == DW_TAG_enumerator);

                bool found_value = false;
                _upf_enum cenum = {0};
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];

                    if (attr.name == DW_AT_name) {
                        cenum.name = _upf_get_str(cu, die, attr.form);
                    } else if (attr.name == DW_AT_const_value) {
                        if (_upf_is_data(attr.form)) {
                            cenum.value = _upf_get_data(die, attr);
                            found_value = true;
                        } else {
                            _UPF_WARN("Non-constant enum values aren't supported. Ignoring this type.");
                            goto unknown_type;
                        }
                    }

                    die += _upf_get_attr_size(die, attr.form);
                }
                _UPF_ASSERT(cenum.name != NULL && found_value);

                _UPF_VECTOR_PUSH(&type.as.cenum.enums, cenum);
            }

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_pointer_type: {
            _UPF_ASSERT(size == UINT64_MAX || size == sizeof(void *));

            _upf_type type = {
                .name = name,
                .kind = _UPF_TK_POINTER,
                .size = sizeof(void *),
            };

            // Pointers have to be added before their data gets filled in so that
            // structs that pointer to themselves, such as linked lists, don't get
            // stuck in an infinite loop.
            _upf_type *type_ptr = _upf_add_type(die_base, type);

            if (subtype_offset != UINT64_MAX) {
                // `void*`s have invalid offset (since they don't point to any type), thus
                // pointer with invalid type represents a `void*`

                type_ptr->as.pointer.type = _upf_parse_type(cu, cu->base + subtype_offset);
                type_ptr->name = type_ptr->as.pointer.type->name;
            }

            return type_ptr;
        }
        case DW_TAG_structure_type:
        case DW_TAG_union_type: {
            bool is_struct = abbrev->tag == DW_TAG_structure_type;
            _upf_type type = {
                .name = name ? name : (is_struct ? "struct" : "union"),
                .kind = is_struct ? _UPF_TK_STRUCT : _UPF_TK_UNION,
                .size = size,
            };

            if (!abbrev->has_children) return _upf_add_type(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                if (abbrev->tag != DW_TAG_member) {
                    die = _upf_skip_die(die, abbrev);
                    continue;
                }

                _upf_member member = {
                    .offset = is_struct ? UINT64_MAX : 0,
                };
                bool skip_member = false;
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];

                    if (attr.name == DW_AT_name) {
                        member.name = _upf_get_str(cu, die, attr.form);
                    } else if (attr.name == DW_AT_type) {
                        const uint8_t *type_die = cu->base + _upf_get_ref(die, attr.form);
                        member.type = _upf_parse_type(cu, type_die);
                    } else if (attr.name == DW_AT_data_member_location) {
                        if (_upf_is_data(attr.form)) {
                            member.offset = _upf_get_data(die, attr);
                        } else {
                            _UPF_WARN("Non-constant member offsets aren't supported. Skipping this field.");
                            skip_member = true;
                        }
                    } else if (attr.name == DW_AT_bit_offset) {
                        _UPF_WARN("DW_AT_bit_offset is deprecated in DWARF v5. Skipping this field.");
                        skip_member = true;
                    } else if (attr.name == DW_AT_data_bit_offset) {
                        member.offset = _upf_get_data(die, attr);
                    } else if (attr.name == DW_AT_bit_size) {
                        if (_upf_is_data(attr.form)) {
                            member.bit_size = _upf_get_data(die, attr);
                        } else {
                            _UPF_WARN("Non-constant bit field sizes aren't supported. Skipping this field.");
                            skip_member = true;
                        }
                    }

                    die += _upf_get_attr_size(die, attr.form);
                }
                if (skip_member) continue;

                _UPF_ASSERT(member.name != NULL && member.type != NULL && member.offset != UINT64_MAX);
                _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
            }

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_subroutine_type: {
            _upf_type type = {
                .name = name,
                .kind = _UPF_TK_FUNCTION,
                .size = size,
            };

            if (subtype_offset != UINT64_MAX) {
                type.as.function.return_type = _upf_parse_type(cu, cu->base + subtype_offset);
            }

            if (!abbrev->has_children) return _upf_add_type(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                if (abbrev->tag != DW_TAG_formal_parameter) {
                    die = _upf_skip_die(die, abbrev);
                    continue;
                }

                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];

                    if (attr.name == DW_AT_type) {
                        const uint8_t *type_die = cu->base + _upf_get_ref(die, attr.form);
                        _UPF_VECTOR_PUSH(&type.as.function.arg_types, _upf_parse_type(cu, type_die));
                    }

                    die += _upf_get_attr_size(die, attr.form);
                }
            }

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_typedef: {
            _UPF_ASSERT(name != NULL);

            if (subtype_offset == UINT64_MAX) {
                // void type is represented by absence of type attribute, e.g. typedef void NAME

                _upf_type type = {
                    .name = name,
                    .kind = _UPF_TK_VOID,
                    .size = UINT64_MAX,
                };
                return _upf_add_type(die_base, type);
            }

            _upf_type type = *_upf_parse_type(cu, cu->base + subtype_offset);
            type.name = name;

            if (type.kind == _UPF_TK_SCHAR && strcmp(name, "int8_t") == 0) type.kind = _UPF_TK_S1;
            else if (type.kind == _UPF_TK_UCHAR && strcmp(name, "uint8_t") == 0) type.kind = _UPF_TK_U1;

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_base_type: {
            _UPF_ASSERT(name != NULL && size != UINT64_MAX && encoding != 0);

            _upf_type type = {
                .name = name,
                .kind = _upf_get_type_kind(encoding, size),
                .size = size,
            };

            return _upf_add_type(die_base, type);
        }
        case DW_TAG_const_type:
        case DW_TAG_volatile_type:
        case DW_TAG_restrict_type:
        case DW_TAG_atomic_type: {
            if (subtype_offset == UINT64_MAX) {
                _upf_type type = {
                    .name = "void",
                    .kind = _UPF_TK_VOID,
                    .modifiers = _upf_get_type_modifier(abbrev->tag),
                    .size = sizeof(void *),
                };

                return _upf_add_type(die_base, type);
            } else {
                _upf_type type = *_upf_parse_type(cu, cu->base + subtype_offset);
                type.modifiers |= _upf_get_type_modifier(abbrev->tag);

                return _upf_add_type(die_base, type);
            }
        }
        default:
            _UPF_WARN("Found unsupported type (0x%lx). Ignoring it.", abbrev->tag);
            break;
    }

    _upf_type type;
unknown_type:
    type = (_upf_type) {
        .name = name,
        .kind = _UPF_TK_UNKNOWN,
        .modifiers = 0,
        .size = size,
    };
    return _upf_add_type(die_base, type);
}

static bool _upf_is_in_range(uint64_t pc, _upf_range_vec ranges) {
    for (uint32_t i = 0; i < ranges.length; i++) {
        if (ranges.data[i].start <= pc && pc < ranges.data[i].end) return true;
    }
    return false;
}

static _upf_range_vec _upf_get_ranges(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL && _upf_state.rnglists != NULL);

    const uint8_t *rnglist = NULL;
    if (form == DW_FORM_sec_offset) {
        rnglist = _upf_state.rnglists + _upf_offset_cast(die);
    } else {
        _UPF_ASSERT(cu->rnglists_base != UINT64_MAX);
        uint64_t index;
        _upf_uLEB_to_uint64(die, &index);
        uint64_t rnglist_offset = _upf_offset_cast(_upf_state.rnglists + cu->rnglists_base + index * _upf_state.offset_size);
        rnglist = _upf_state.rnglists + cu->rnglists_base + rnglist_offset;
    }
    _UPF_ASSERT(rnglist != NULL);

    uint64_t base = 0;
    if (cu->scope.ranges.length == 1) base = cu->scope.ranges.data[0].start;

    _upf_range_vec ranges = {0};
    while (*rnglist != DW_RLE_end_of_list) {
        switch (*rnglist++) {
            case DW_RLE_base_addressx:
                base = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                break;
            case DW_RLE_startx_endx: {
                uint64_t start = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                uint64_t end = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);

                _upf_range range = {
                    .start = start,
                    .end = end,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_startx_length: {
                uint64_t address = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                uint64_t length = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);

                _upf_range range = {
                    .start = address,
                    .end = address + length,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_offset_pair: {
                uint64_t start, end;
                rnglist += _upf_uLEB_to_uint64(rnglist, &start);
                rnglist += _upf_uLEB_to_uint64(rnglist, &end);

                _upf_range range = {
                    .start = base + start,
                    .end = base + end,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_base_address:
                base = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                break;
            case DW_RLE_start_end: {
                uint64_t start = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                uint64_t end = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;

                _upf_range range = {
                    .start = start,
                    .end = end,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_start_length: {
                uint64_t address = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                uint64_t length;
                rnglist += _upf_uLEB_to_uint64(rnglist, &length);

                _upf_range range = {
                    .start = address,
                    .end = address + length,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            default:
                _UPF_WARN("Found unsupported range list entry kind (0x%x). Skipping it.", *(rnglist - 1));
                return ranges;
        }
    }

    return ranges;
}

static bool _upf_is_language_c(int64_t language) {
    switch (language) {
        case DW_LANG_C:
        case DW_LANG_C89:
        case DW_LANG_C99:
        case DW_LANG_C11:
        case DW_LANG_C17:
            return true;
        default:
            return false;
    }
}

static _upf_range_vec _upf_get_cu_ranges(const _upf_cu *cu, const uint8_t *low_pc_die, _upf_attr low_pc_attr, const uint8_t *high_pc_die,
                                         _upf_attr high_pc_attr, const uint8_t *ranges_die, _upf_attr ranges_attr) {
    _UPF_ASSERT(cu != NULL);

    if (ranges_die != NULL) return _upf_get_ranges(cu, ranges_die, ranges_attr.form);

    _upf_range_vec ranges = {0};
    _upf_range range = {
        .start = UINT64_MAX,
        .end = UINT64_MAX,
    };

    if (low_pc_die == NULL) {
        _UPF_VECTOR_PUSH(&ranges, range);
        return ranges;
    }

    range.start = _upf_get_addr(cu, low_pc_die, low_pc_attr.form);
    if (high_pc_die != NULL) {
        if (_upf_is_addr(high_pc_attr.form)) {
            range.end = _upf_get_addr(cu, high_pc_die, high_pc_attr.form);
        } else {
            range.end = range.start + _upf_get_data(high_pc_die, high_pc_attr);
        }
    }

    _UPF_VECTOR_PUSH(&ranges, range);
    return ranges;
}

static void _upf_parse_subprogram_parameters(_upf_function *function, const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(function != NULL && cu != NULL && die != NULL);

    while (true) {
        const _upf_abbrev *abbrev;
        die += _upf_get_abbrev(&abbrev, cu, die);
        if (abbrev == NULL) break;
        if (abbrev->tag == DW_TAG_unspecified_parameters) function->is_variadic = true;
        if (abbrev->tag != DW_TAG_formal_parameter) break;

        _upf_named_type arg = {0};
        for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_name) arg.name = _upf_get_str(cu, die, attr.form);
            else if (attr.name == DW_AT_type) arg.die = cu->base + _upf_get_ref(die, attr.form);

            die += _upf_get_attr_size(die, attr.form);
        }
        _UPF_ASSERT(die != NULL);

        _UPF_VECTOR_PUSH(&function->args, arg);
    }
}

static _upf_function _upf_parse_cu_subprogram(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    _upf_function function = {
        .pc = UINT64_MAX,
    };
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            function.name = _upf_get_str(cu, die, attr.form);
        } else if (attr.name == DW_AT_type) {
            function.return_type_die = cu->base + _upf_get_ref(die, attr.form);
        } else if (attr.name == DW_AT_low_pc) {
            function.pc = _upf_get_addr(cu, die, attr.form);
        } else if (attr.name == DW_AT_ranges) {
            _upf_range_vec ranges = _upf_get_ranges(cu, die, attr.form);
            _UPF_ASSERT(ranges.length > 0);
            function.pc = ranges.data[0].start;
        } else if (attr.name == DW_AT_abstract_origin) {
            const uint8_t *new_die = cu->base + _upf_get_ref(die, attr.form);
            const _upf_abbrev *new_abbrev;
            new_die += _upf_get_abbrev(&new_abbrev, cu, new_die);

            _upf_function origin_function = _upf_parse_cu_subprogram(cu, new_die, new_abbrev);
            origin_function.pc = function.pc;
            function = origin_function;
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    if (abbrev->has_children && function.args.length == 0) _upf_parse_subprogram_parameters(&function, cu, die);

    return function;
}

static bool _upf_parse_cu_scope(const _upf_cu *cu, _upf_scope_stack *scope_stack, int depth, const uint8_t *die,
                                const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && scope_stack != NULL && die != NULL && abbrev != NULL);

    if (!abbrev->has_children) return false;
    _UPF_ASSERT(scope_stack->length > 0);

    _upf_scope new_scope = {0};

    uint64_t low_pc = UINT64_MAX;
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = {0};
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_low_pc) {
            low_pc = _upf_get_addr(cu, die, attr.form);
        } else if (attr.name == DW_AT_high_pc) {
            high_pc_die = die;
            high_pc_attr = attr;
        } else if (attr.name == DW_AT_ranges) {
            new_scope.ranges = _upf_get_ranges(cu, die, attr.form);
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    if (low_pc != UINT64_MAX) {
        if (high_pc_die == NULL) _UPF_ERROR("Expected CU to have both low and high PC.");

        _upf_range range = {
            .start = low_pc,
            .end = UINT64_MAX,
        };

        if (_upf_is_addr(high_pc_attr.form)) {
            range.end = _upf_get_addr(cu, high_pc_die, high_pc_attr.form);
        } else {
            range.end = low_pc + _upf_get_data(high_pc_die, high_pc_attr);
        }

        _UPF_VECTOR_PUSH(&new_scope.ranges, range);
    }

    _upf_scope_stack_entry stack_entry = {
        .depth = depth,
        .scope = NULL,
    };

    if (new_scope.ranges.length > 0) {
        _upf_scope *scope = NULL;
        for (uint32_t i = 0; i < scope_stack->length; i++) {
            scope = scope_stack->data[scope_stack->length - 1 - i].scope;
            if (scope != NULL) break;
        }
        _UPF_ASSERT(scope != NULL);

        _UPF_VECTOR_PUSH(&scope->scopes, new_scope);
        stack_entry.scope = &_UPF_VECTOR_TOP(&scope->scopes);
    } else {
        stack_entry.scope = NULL;
    }

    _UPF_VECTOR_PUSH(scope_stack, stack_entry);
    return new_scope.ranges.length > 0;
}

static const char *_upf_get_typename(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    const char *name = NULL;
    uint64_t type_offset = UINT64_MAX;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) name = _upf_get_str(cu, die, attr.form);
        else if (attr.name == DW_AT_type) type_offset = _upf_get_ref(die, attr.form);

        die += _upf_get_attr_size(die, attr.form);
    }
    // Type-less pointer is void. It must be named to allow casting to `void`.
    if (abbrev->tag == DW_TAG_pointer_type && type_offset == UINT64_MAX) return "void";

    return name;
}

static _upf_named_type _upf_parse_cu_variable(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    _upf_named_type var = {0};
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            var.name = _upf_get_str(cu, die, attr.form);
        } else if (attr.name == DW_AT_type) {
            var.die = cu->base + _upf_get_ref(die, attr.form);
        } else if (attr.name == DW_AT_abstract_origin) {
            const uint8_t *new_die = cu->base + _upf_get_ref(die, attr.form);
            const _upf_abbrev *new_abbrev;
            new_die += _upf_get_abbrev(&new_abbrev, cu, new_die);

            return _upf_parse_cu_variable(cu, new_die, new_abbrev);
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    return var;
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *die, const uint8_t *die_end, const uint8_t *abbrev_table) {
    _UPF_ASSERT(cu_base != NULL && die != NULL && die_end != NULL && abbrev_table != NULL);

    _upf_cu cu = {
        .base = cu_base,
        .str_offsets_base = UINT64_MAX,
        .rnglists_base = UINT64_MAX,
        .abbrevs = _upf_parse_abbrevs(abbrev_table),
    };

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, &cu, die);
    _UPF_ASSERT(abbrev != NULL && abbrev->tag == DW_TAG_compile_unit);

    // Save to parse after the initializing addr_base, str_offsets, and rnglists_base
    const uint8_t *low_pc_die = NULL;
    _upf_attr low_pc_attr = {0};
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = {0};
    const uint8_t *ranges_die = NULL;
    _upf_attr ranges_attr = {0};
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_low_pc) {
            low_pc_attr = attr;
            low_pc_die = die;
        } else if (attr.name == DW_AT_high_pc) {
            high_pc_attr = attr;
            high_pc_die = die;
        } else if (attr.name == DW_AT_ranges) {
            ranges_attr = attr;
            ranges_die = die;
        } else if (attr.name == DW_AT_addr_base) {
            cu.addr_base = _upf_offset_cast(die);
        } else if (attr.name == DW_AT_str_offsets_base) {
            cu.str_offsets_base = _upf_offset_cast(die);
        } else if (attr.name == DW_AT_rnglists_base) {
            cu.rnglists_base = _upf_offset_cast(die);
        } else if (attr.name == DW_AT_language) {
            int64_t language = _upf_get_data(die, attr);
            if (!_upf_is_language_c(language)) return;
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    cu.scope.ranges = _upf_get_cu_ranges(&cu, low_pc_die, low_pc_attr, high_pc_die, high_pc_attr, ranges_die, ranges_attr);

    int depth = 0;
    _upf_scope_stack scope_stack = {0};

    _upf_scope_stack_entry stack_entry = {
        .depth = depth,
        .scope = &cu.scope,
    };
    _UPF_VECTOR_PUSH(&scope_stack, stack_entry);

    while (die < die_end) {
        const uint8_t *die_base = die;

        die += _upf_get_abbrev(&abbrev, &cu, die);
        if (abbrev == NULL) {
            _UPF_ASSERT(scope_stack.length > 0);
            if (depth == _UPF_VECTOR_TOP(&scope_stack).depth) _UPF_VECTOR_POP(&scope_stack);
            depth--;
            continue;
        }

        if (abbrev->has_children) depth++;

        switch (abbrev->tag) {
            case DW_TAG_subprogram: {
                _upf_function function = _upf_parse_cu_subprogram(&cu, die, abbrev);
                if (function.name != NULL) _UPF_VECTOR_PUSH(&cu.functions, function);
                __attribute__((fallthrough));
            }
            case DW_TAG_lexical_block:
            case DW_TAG_inlined_subroutine:
                _upf_parse_cu_scope(&cu, &scope_stack, depth, die, abbrev);
                break;
            case DW_TAG_array_type:
            case DW_TAG_enumeration_type:
            case DW_TAG_pointer_type:
            case DW_TAG_structure_type:
            case DW_TAG_typedef:
            case DW_TAG_union_type:
            case DW_TAG_base_type: {
                const char *typename = _upf_get_typename(&cu, die, abbrev);
                if (typename == NULL) break;

                _upf_named_type type = {
                    .die = die_base,
                    .name = typename,
                };
                _UPF_VECTOR_PUSH(&cu.types, type);
            } break;
            case DW_TAG_variable:
            case DW_TAG_formal_parameter: {
                _upf_scope *scope = _UPF_VECTOR_TOP(&scope_stack).scope;
                if (scope == NULL) break;

                _upf_named_type var = _upf_parse_cu_variable(&cu, die, abbrev);
                if (var.name == NULL) break;
                if (var.die == NULL) {
                    _UPF_ERROR(
                        "Unable to find variable DIE. "
                        "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).");
                }

                _UPF_VECTOR_PUSH(&scope->vars, var);
            } break;
        }

        die = _upf_skip_die(die, abbrev);
    }

    _UPF_VECTOR_PUSH(&_upf_state.cus, cu);
}

static void _upf_parse_dwarf(void) {
    const uint8_t *die = _upf_state.die;
    const uint8_t *die_end = die + _upf_state.die_size;
    while (die < die_end) {
        const uint8_t *cu_base = die;

        uint64_t length = 0;
        memcpy(&length, die, sizeof(uint32_t));
        die += sizeof(uint32_t);

        _upf_state.is64bit = false;
        if (length == UINT32_MAX) {
            memcpy(&length, die, sizeof(uint64_t));
            die += sizeof(uint64_t);
            _upf_state.is64bit = true;
        }

        _upf_state.offset_size = _upf_state.is64bit ? 8 : 4;

        const uint8_t *next = die + length;

        uint16_t version = 0;
        memcpy(&version, die, sizeof(version));
        die += sizeof(version);
        if (version != 5) _UPF_ERROR("uprintf only supports DWARF version 5.");

        uint8_t type = *die;
        die += sizeof(type);
        if (type != DW_UT_compile) _UPF_ERROR("uprintf does NOT support split debug information.");

        uint8_t address_size = *die;
        _UPF_ASSERT(_upf_state.address_size == 0 || _upf_state.address_size == address_size);
        _upf_state.address_size = address_size;
        die += sizeof(address_size);

        uint64_t abbrev_offset = _upf_offset_cast(die);
        die += _upf_state.offset_size;

        _upf_parse_cu(cu_base, die, next, _upf_state.abbrev + abbrev_offset);

        die = next;
    }
}

// ======================= ELF ============================

// Linker-generated entry to dynamic section of ELF.
extern Elf64_Dyn _DYNAMIC[];

// Extern function mapping: function pointer -> GOT -> RELA -> symbol -> name -> DIE
static void _upf_parse_extern_functions(void) {
    _UPF_ASSERT(_DYNAMIC != NULL);

    const char *string_table = NULL;
    const Elf64_Sym *symbol_table = NULL;
    const Elf64_Rela *rela_table = NULL;
    int rela_size = -1;
    for (const Elf64_Dyn *dyn = _DYNAMIC; dyn->d_tag != DT_NULL; dyn++) {
        switch (dyn->d_tag) {
            case DT_STRTAB:
                string_table = (char *) dyn->d_un.d_ptr;
                break;
            case DT_SYMTAB:
                symbol_table = (Elf64_Sym *) dyn->d_un.d_ptr;
                break;
            case DT_RELA:
                rela_table = (Elf64_Rela *) dyn->d_un.d_ptr;
                break;
            case DT_RELASZ:
                rela_size = dyn->d_un.d_val / sizeof(Elf64_Rela);
                break;
            case DT_RELAENT:
                _UPF_ASSERT(dyn->d_un.d_val == sizeof(Elf64_Rela));
                break;
        }
    }
    if (string_table == NULL || symbol_table == NULL || rela_table == NULL || rela_size == -1) {
        _UPF_WARN("Unable to find one of the required ELF sections. Ignoring extern functions.");
        return;
    }
    _UPF_ASSERT(((void *) _upf_state.base) < ((void *) string_table));  // d_ptr may sometimes be relative to the base

    for (int i = 0; i < rela_size; i++) {
        Elf64_Rela rela = rela_table[i];

        int symbol_idx = ELF64_R_SYM(rela.r_info);
        if (symbol_idx == STN_UNDEF) continue;

        Elf64_Sym symbol = symbol_table[symbol_idx];
        const char *symbol_name = string_table + symbol.st_name;
        uint64_t symbol_address = *((uint64_t *) (_upf_state.base + rela.r_offset));

        _upf_extern_function extern_function = {
            .name = symbol_name,
            .pc = symbol_address,
        };
        _UPF_VECTOR_PUSH(&_upf_state.extern_functions, extern_function);
    }
}

static void _upf_parse_elf(void) {
    struct stat file_info;
    if (stat("/proc/self/exe", &file_info) == -1) _UPF_ERROR("Unable to stat \"/proc/self/exe\": %s.", strerror(errno));
    size_t size = file_info.st_size;
    _upf_state.file_size = size;

    int fd = open("/proc/self/exe", O_RDONLY);
    _UPF_ASSERT(fd != -1);

    // A new instance of file must be mmap-ed because the one loaded in memory
    // only contains information needed at runtime, and doesn't include debug
    // information, table with sections' names, etc.
    uint8_t *file = (uint8_t *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    _UPF_ASSERT(file != MAP_FAILED);
    _upf_state.file = file;

    close(fd);

    const Elf64_Ehdr *header = (Elf64_Ehdr *) file;

    if (memcmp(header->e_ident, ELFMAG, SELFMAG) != 0 ||  //
        header->e_ident[EI_CLASS] != ELFCLASS64 ||        //
        header->e_ident[EI_VERSION] != 1 ||               //
        header->e_machine != EM_X86_64 ||                 //
        header->e_version != 1 ||                         //
        header->e_shentsize != sizeof(Elf64_Shdr)) {
        _UPF_ERROR("Unsupported or invalid ELF file.");
    }

    const Elf64_Shdr *string_section = (Elf64_Shdr *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);

    const Elf64_Shdr *section = (Elf64_Shdr *) (file + header->e_shoff);
    for (size_t i = 0; i < header->e_shnum; i++) {
        const char *name = string_table + section->sh_name;

        if (strcmp(name, ".debug_info") == 0) {
            _upf_state.die = file + section->sh_offset;
            _upf_state.die_size = section->sh_size;
        } else if (strcmp(name, ".debug_abbrev") == 0) {
            _upf_state.abbrev = file + section->sh_offset;
        } else if (strcmp(name, ".debug_str") == 0) {
            _upf_state.str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_line_str") == 0) {
            _upf_state.line_str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_str_offsets") == 0) {
            _upf_state.str_offsets = file + section->sh_offset;
        } else if (strcmp(name, ".debug_rnglists") == 0) {
            _upf_state.rnglists = file + section->sh_offset;
        } else if (strcmp(name, ".debug_addr") == 0) {
            _upf_state.addr = file + section->sh_offset;
        }

        section++;
    }

    if (_upf_state.die == NULL || _upf_state.abbrev == NULL || _upf_state.str == NULL) {
        _UPF_ERROR("Unable to find debugging information. Ensure that the executable contains it by compiling with `-g2` or `-g3`.");
    }
}

// ==================== TOKENIZING ========================

static _upf_cstr_vec _upf_get_args(char *string) {
    _UPF_ASSERT(string != NULL);

    _upf_cstr_vec args = {0};

    bool in_quotes = false;
    int paren = 0;
    const char *prev = string;
    for (char *ch = string; *ch != '\0'; ch++) {
        if (*ch == '"') {
            if (in_quotes) {
                if (*(ch - 1) == '\\') in_quotes = true;
                else in_quotes = !in_quotes;
            } else {
                if (*(ch - 1) == '\'') in_quotes = false;
                else in_quotes = !in_quotes;
            }
        }
        if (in_quotes) continue;

        if (*ch == '(') {
            paren++;
        } else if (*ch == ')') {
            paren--;
        } else if (*ch == ',' && paren == 0) {
            *ch = '\0';
            _UPF_VECTOR_PUSH(&args, prev);
            prev = ch + 1;
        }
    }
    _UPF_VECTOR_PUSH(&args, prev);

    return args;
}

static void _upf_tokenize(_upf_tokenizer *t, const char *string) {
    _UPF_ASSERT(t != NULL && string != NULL);

    // Signs should be ordered so that longer ones go first in order to avoid multi character sign
    // being tokenized as multiple single character ones, e.g. >= being '>' '='.
    static const _upf_token signs[]
        = {{_UPF_TOK_ASSIGNMENT, "<<="}, {_UPF_TOK_ASSIGNMENT, ">>="},

           {_UPF_TOK_ARROW, "->"},       {_UPF_TOK_INCREMENT, "++"},   {_UPF_TOK_DECREMENT, "--"},   {_UPF_TOK_COMPARISON, "<="},
           {_UPF_TOK_COMPARISON, ">="},  {_UPF_TOK_COMPARISON, "=="},  {_UPF_TOK_COMPARISON, "!="},  {_UPF_TOK_COMPARISON, "&&"},
           {_UPF_TOK_COMPARISON, "||"},  {_UPF_TOK_MATH, "<<"},        {_UPF_TOK_MATH, ">>"},        {_UPF_TOK_ASSIGNMENT, "*="},
           {_UPF_TOK_ASSIGNMENT, "/="},  {_UPF_TOK_ASSIGNMENT, "%="},  {_UPF_TOK_ASSIGNMENT, "+="},  {_UPF_TOK_ASSIGNMENT, "-="},
           {_UPF_TOK_ASSIGNMENT, "&="},  {_UPF_TOK_ASSIGNMENT, "^="},  {_UPF_TOK_ASSIGNMENT, "|="},

           {_UPF_TOK_COMMA, ","},        {_UPF_TOK_AMPERSAND, "&"},    {_UPF_TOK_STAR, "*"},         {_UPF_TOK_OPEN_PAREN, "("},
           {_UPF_TOK_CLOSE_PAREN, ")"},  {_UPF_TOK_DOT, "."},          {_UPF_TOK_OPEN_BRACKET, "["}, {_UPF_TOK_CLOSE_BRACKET, "]"},
           {_UPF_TOK_QUESTION, "?"},     {_UPF_TOK_COLON, ":"},        {_UPF_TOK_COMPARISON, "<"},   {_UPF_TOK_COMPARISON, ">"},
           {_UPF_TOK_EXCLAMATION, "!"},  {_UPF_TOK_PLUS, "+"},         {_UPF_TOK_MINUS, "-"},        {_UPF_TOK_TILDE, "~"},
           {_UPF_TOK_MATH, "/"},         {_UPF_TOK_MATH, "%"},         {_UPF_TOK_MATH, "^"},         {_UPF_TOK_MATH, "|"},
           {_UPF_TOK_ASSIGNMENT, "="}};
    static const char *type_specifiers[] = {"struct", "union", "enum"};
    static const char *type_qualifiers[] = {"const", "volatile", "restrict", "_Atomic"};

    const char *ch = string;
    while (*ch != '\0') {
        if (*ch == ' ') {
            ch++;
            continue;
        }

        if ('0' <= *ch && *ch <= '9') {
            // Handle floats written as ".123"
            if (t->tokens.length > 0 && _UPF_VECTOR_TOP(&t->tokens).kind == _UPF_TOK_DOT) {
                _UPF_VECTOR_POP(&t->tokens);
                while (*ch != '.') ch--;
            }

            errno = 0;
            char *end = NULL;
            strtof(ch, &end);
            _UPF_ASSERT(errno == 0 && end != NULL);

            _upf_token token = {
                .kind = _UPF_TOK_NUMBER,
                .string = _upf_arena_string(&_upf_state.arena, ch, end),
            };
            _UPF_VECTOR_PUSH(&t->tokens, token);

            ch = end;
        } else if (('a' <= *ch && *ch <= 'z') || ('A' <= *ch && *ch <= 'Z') || *ch == '_') {
            const char *end = ch;
            while (('a' <= *end && *end <= 'z') || ('A' <= *end && *end <= 'Z') || ('0' <= *end && *end <= '9') || *end == '_') end++;

            const char *string = _upf_arena_string(&_upf_state.arena, ch, end);

            enum _upf_token_kind kind = _UPF_TOK_ID;

            for (size_t i = 0; i < sizeof(type_qualifiers) / sizeof(*type_qualifiers); i++) {
                if (strcmp(type_qualifiers[i], string) == 0) {
                    kind = _UPF_TOK_TYPE_QUALIFIER;
                    break;
                }
            }

            for (size_t i = 0; i < sizeof(type_specifiers) / sizeof(*type_specifiers); i++) {
                if (strcmp(type_specifiers[i], string) == 0) {
                    kind = _UPF_TOK_TYPE_SPECIFIER;
                    break;
                }
            }

            _upf_token token = {
                .kind = kind,
                .string = string,
            };
            _UPF_VECTOR_PUSH(&t->tokens, token);

            ch = end;
        } else if (*ch == '"') {
            ch++;

            int escape = 0;
            const char *end = ch;
            while ((*end != '"' || escape % 2 == 1) && *end != '\0') {
                if (*end == '\\') escape++;
                else escape = 0;

                end++;
            }
            _UPF_ASSERT(*end != '\0');

            _upf_token token = {
                .kind = _UPF_TOK_STRING,
                .string = _upf_arena_string(&_upf_state.arena, ch, end),
            };
            _UPF_VECTOR_PUSH(&t->tokens, token);

            ch = end + 1;
        } else {
            bool found = false;
            for (size_t i = 0; i < sizeof(signs) / sizeof(*signs) && !found; i++) {
                size_t len = strlen(signs[i].string);
                if (strncmp(ch, signs[i].string, len) == 0) {
                    _UPF_VECTOR_PUSH(&t->tokens, signs[i]);
                    ch += len;
                    found = true;
                }
            }
            if (!found) {
                _UPF_ERROR("Unknown character '%c' when parsing arguments \"%s\" at %s:%d.", *ch, string, _upf_state.file_path,
                           _upf_state.line);
            }
        }
    }
}

static void _upf_back(_upf_tokenizer *t) {
    _UPF_ASSERT(t != NULL && t->idx > 0);
    t->idx--;
}

static const _upf_token *_upf_consume_any2(_upf_tokenizer *t, ...) {
    _UPF_ASSERT(t != NULL);

    if (t->idx >= t->tokens.length) return NULL;
    const _upf_token *token = &t->tokens.data[t->idx];

    va_list va_args;
    va_start(va_args, t);
    while (true) {
        enum _upf_token_kind kind = va_arg(va_args, enum _upf_token_kind);
        if (kind == _UPF_TOK_NONE) break;

        if (token->kind == kind) {
            t->idx++;
            va_end(va_args);
            return token;
        }
    }
    va_end(va_args);

    return NULL;
}

#define _upf_consume_any(t, ...) _upf_consume_any2((t), __VA_ARGS__, _UPF_TOK_NONE)

// ===================== PARSING ==========================

// Recursive descent parser with backtracking for parsing function arguments.
// Parsing functions contain commented out rules from Yacc used as a reference.
// Complete C11 Yacc grammar that was used as a reference: https://www.quut.com/c/ANSI-C-grammar-y.html

static bool _upf_parse_unary_expr(_upf_tokenizer *t, _upf_parser_state *p);
static bool _upf_parse_expr(_upf_tokenizer *t, _upf_parser_state *p);

static bool _upf_parse_typename(int *dereference, const char **typename, _upf_type **type_ptr, _upf_tokenizer *t) {
    _UPF_ASSERT(t != NULL);
    // pointer
    // 	: '*' type_qualifier[] pointer
    // 	| '*' type_qualifier[]
    // 	| '*' pointer
    // 	| '*'
    // typename
    // 	: specifier_qualifier[] pointer
    // 	| specifier_qualifier[]

    const char *ids[4];  // max is "unsigned long long int"
    int ids_length = 0;
    bool is_type = false;
    const _upf_token *token = NULL;
    while ((token = _upf_consume_any(t, _UPF_TOK_TYPE_SPECIFIER, _UPF_TOK_TYPE_QUALIFIER, _UPF_TOK_ID))) {
        is_type = true;
        if (token->kind == _UPF_TOK_ID) {
            _UPF_ASSERT(ids_length < 4);
            ids[ids_length++] = token->string;
        }
    }
    if (!is_type) return false;

    while (_upf_consume_any(t, _UPF_TOK_STAR)) {
        if (dereference) *dereference -= 1;
        while (_upf_consume_any(t, _UPF_TOK_TYPE_QUALIFIER)) continue;
    }

    if (typename == NULL && type_ptr == NULL) return true;

    int kind = DW_ATE_signed;
    bool is_signed = true;
    int longness = 0;
    for (int i = 0; i < ids_length; i++) {
        if (strcmp(ids[i], "long") == 0) {
            longness++;
        } else if (strcmp(ids[i], "short") == 0) {
            longness--;
        } else if (strcmp(ids[i], "unsigned") == 0) {
            is_signed = false;
        } else if (strcmp(ids[i], "signed") == 0) {
            is_signed = true;
        } else if (strcmp(ids[i], "char") == 0) {
            kind = DW_ATE_signed_char;
        } else if (strcmp(ids[i], "int") == 0) {
            kind = DW_ATE_signed;
        } else if (strcmp(ids[i], "double") == 0) {
            kind = DW_ATE_float;
            longness++;
        } else if (strcmp(ids[i], "float") == 0) {
            kind = DW_ATE_float;
        } else {
            _UPF_ASSERT(ids_length == 1);
            *typename = ids[0];
            return true;
        }
    }

    _upf_type type = {
        .kind = _UPF_TK_UNKNOWN,
    };
    if (kind == DW_ATE_signed_char) {
        type.kind = is_signed ? _UPF_TK_SCHAR : _UPF_TK_UCHAR;
        type.size = sizeof(char);
    } else if (kind == DW_ATE_float) {
        switch (longness) {
            case 0:
                type.kind = _UPF_TK_F4;
                type.size = sizeof(float);
                break;
            case 2:
                if (sizeof(long double) != sizeof(double)) {
                    _UPF_WARN("Long doubles aren't supported. Ignoring this type.");

                    type.kind = _UPF_TK_UNKNOWN;
                    type.size = sizeof(long double);
                    break;
                }
                __attribute__((fallthrough));
            case 1:
                type.kind = _UPF_TK_F8;
                type.size = sizeof(double);
                break;
            default:
                _UPF_ERROR("Invalid floating-point number length.");
        }
    } else if (kind == DW_ATE_signed) {
        switch (longness) {
            case -1:
                type.size = sizeof(short int);
                break;
            case 0:
                type.size = sizeof(int);
                break;
            case 1:
                type.size = sizeof(long int);
                break;
            case 2:
                type.size = sizeof(long long int);
                break;
            default:
                _UPF_ERROR("Invalid integer length.");
        }
        type.kind = _upf_get_type_kind(is_signed ? DW_ATE_signed : DW_ATE_unsigned, type.size);
    } else {
        _UPF_ERROR("Invalid integer type.");
    }

    *type_ptr = _upf_add_type(NULL, type);
    return true;
}

static bool _upf_parse_cast_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // cast_expr
    // 	: unary_expr
    // 	| '(' typename ')' cast_expr

    int dereference = 0;
    const char *typename = NULL;
    _upf_type *type = NULL;
    size_t save = t->idx;
    if (_upf_consume_any(t, _UPF_TOK_OPEN_PAREN) && _upf_parse_typename(&dereference, &typename, &type, t)
        && _upf_consume_any(t, _UPF_TOK_CLOSE_PAREN) && _upf_parse_cast_expr(t, NULL)) {
        if (p) {
            p->dereference = dereference;
            if (typename == NULL) {
                _UPF_ASSERT(type != NULL);
                p->base.type = type;
                p->base_type = _UPF_BT_INT_TYPE;
            } else {
                p->base.name = typename;
                p->base_type = _UPF_BT_TYPE;
            }
        }
        return true;
    }
    t->idx = save;

    return _upf_parse_unary_expr(t, p);
}

static bool _upf_parse_math_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // multiplicative_expr
    // 	: cast_expr
    // 	| cast_expr MULTIPLICATIVE_OP multiplicative_expr
    // additive_expr
    // 	: multiplicative_expr
    // 	| multiplicative_expr ADDITIVE_OP additive_expr
    // shift_expr
    // 	: additive_expr
    // 	| additive_expr SHIFT_OP shift_expr
    // relational_expr
    // 	: shift_expr
    // 	| shift_expr RELATION_OP relational_expr
    // equality_expr
    // 	: relational_expr
    // 	| relational_expr '==' equality_expr
    // 	| relational_expr '!=' equality_expr
    // and_expr
    // 	: equality_expr
    // 	| equality_expr '&' and_expr
    // xor_expr
    // 	: and_expr
    // 	| and_expr '^' xor_expr
    // inclusive_or_expr
    // 	: xor_expr
    // 	| xor_expr '|' inclusive_or_expr
    // logical_and_expr
    // 	: inclusive_or_expr
    // 	| inclusive_or_expr '&&' logical_and_expr
    // logical_or_expr
    // 	: logical_and_expr
    // 	| logical_and_expr '||' logical_or_expr

    bool result = false;
    size_t save = t->idx;
    do {
        if (!_upf_parse_cast_expr(t, p)) {
            t->idx = save;
            return result;
        }

        result = true;
        save = t->idx;
    } while (_upf_consume_any(t, _UPF_TOK_MATH, _UPF_TOK_COMPARISON, _UPF_TOK_STAR, _UPF_TOK_AMPERSAND, _UPF_TOK_PLUS, _UPF_TOK_MINUS));

    return true;
}

static bool _upf_parse_assignment_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // conditional_expr
    // 	: logical_or_expr
    // 	| logical_or_expr '?' expr ':' conditional_expr
    // assignment_expr
    // 	: conditional_expr
    // 	| unary_expr ASSIGNMENT_OP assignment_expr

    size_t save = t->idx;
    while (true) {
        if (!_upf_parse_math_expr(t, p)) break;
        if (!_upf_consume_any(t, _UPF_TOK_QUESTION)) return true;
        if (!_upf_parse_expr(t, NULL) || !_upf_consume_any(t, _UPF_TOK_COLON)) break;
    }
    t->idx = save;

    if (!_upf_parse_unary_expr(t, p) || !_upf_consume_any(t, _UPF_TOK_ASSIGNMENT) || !_upf_parse_assignment_expr(t, p)) {
        t->idx = save;
        return false;
    }
    return true;
}

static bool _upf_parse_postfix_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // postfix_expr
    // 	: IDENTIFIER postfix_expr'
    // 	| STRING postfix_expr'
    //  | NUMBER postfix_expr'
    // 	| '(' expr ')' postfix_expr'

    bool is_base = false;
    int dereference_save = p ? p->dereference : 0;
    size_t save = t->idx;

    const _upf_token *token = _upf_consume_any(t, _UPF_TOK_ID, _UPF_TOK_STRING, _UPF_TOK_NUMBER, _UPF_TOK_OPEN_PAREN);
    if (token == NULL) return false;
    switch (token->kind) {
        case _UPF_TOK_ID:
            if (p) {
                p->base.name = token->string;
                p->base_type = _UPF_BT_VARIABLE;
                is_base = true;
            }
            break;
        case _UPF_TOK_OPEN_PAREN:
            if (!_upf_parse_expr(t, p) || !_upf_consume_any(t, _UPF_TOK_CLOSE_PAREN)) {
                t->idx = save;
                return false;
            }
            break;
        default:
            break;
    }

    // postfix_expr'
    // 	: '[' expr ']' postfix_expr'
    // 	| '(' ')' postfix_expr'
    // 	| '(' assignment_expr[] ')' postfix_expr'
    // 	| DOT IDENTIFIER postfix_expr'
    // 	| POSTFIX_OP postfix_expr'
    // 	| POSTFIX_OP postfix_expr'
    while ((token = _upf_consume_any(t, _UPF_TOK_OPEN_BRACKET, _UPF_TOK_OPEN_PAREN, _UPF_TOK_DOT, _UPF_TOK_ARROW, _UPF_TOK_INCREMENT,
                                     _UPF_TOK_DECREMENT))) {
        if (p && token->kind != _UPF_TOK_OPEN_PAREN) p->suffix_calls = 0;
        switch (token->kind) {
            case _UPF_TOK_OPEN_BRACKET:
                if (p) p->dereference++;
                if (!_upf_parse_expr(t, NULL) || !_upf_consume_any(t, _UPF_TOK_CLOSE_BRACKET)) {
                    t->idx = save;
                    return false;
                }
                break;
            case _UPF_TOK_OPEN_PAREN:
                while (_upf_parse_assignment_expr(t, NULL) && _upf_consume_any(t, _UPF_TOK_COMMA)) continue;
                if (!_upf_consume_any(t, _UPF_TOK_CLOSE_PAREN)) {
                    t->idx = save;
                    return false;
                }

                if (p) {
                    if (is_base) p->base_type = _UPF_BT_FUNCTION;
                    p->dereference = dereference_save;
                    p->suffix_calls++;
                }
                break;
            case _UPF_TOK_DOT:
            case _UPF_TOK_ARROW:
                is_base = false;

                token = _upf_consume_any(t, _UPF_TOK_ID);
                if (token == NULL) {
                    t->idx = save;
                    return false;
                }

                if (p) {
                    p->dereference = dereference_save;
                    _UPF_VECTOR_PUSH(&p->members, token->string);
                }
                break;
            default:
                break;
        }
    }

    return true;
}

static bool _upf_parse_unary_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // unary_expr
    // 	: postfix_expr
    // 	| PREFIX_OP unary_expr
    // 	| UNARY_OP cast_expr
    // 	| sizeof unary_expr
    // 	| sizeof '(' typename ')'
    // 	| alignof '(' typename ')'

    size_t save = t->idx;
    if (_upf_consume_any(t, _UPF_TOK_INCREMENT, _UPF_TOK_DECREMENT)) {
        if (!_upf_parse_unary_expr(t, p)) {
            t->idx = save;
            return false;
        }
        return true;
    }

    if (_upf_consume_any(t, _UPF_TOK_PLUS, _UPF_TOK_MINUS, _UPF_TOK_TILDE, _UPF_TOK_EXCLAMATION)) {
        if (!_upf_parse_cast_expr(t, p)) {
            t->idx = save;
            return false;
        }
        return true;
    }

    if (_upf_consume_any(t, _UPF_TOK_AMPERSAND)) {
        if (!_upf_parse_cast_expr(t, p)) {
            t->idx = save;
            return false;
        }

        if (p) p->dereference--;
        return true;
    }

    if (_upf_consume_any(t, _UPF_TOK_STAR)) {
        if (!_upf_parse_cast_expr(t, p)) {
            t->idx = save;
            return false;
        }

        if (p) p->dereference++;
        return true;
    }

    const _upf_token *token = _upf_consume_any(t, _UPF_TOK_ID);
    if (token && token->kind == _UPF_TOK_ID) {
        if (strcmp(token->string, "sizeof") == 0) {
            size_t save2 = t->idx;
            if (_upf_consume_any(t, _UPF_TOK_OPEN_PAREN) && _upf_parse_typename(NULL, NULL, NULL, t)
                && _upf_consume_any(t, _UPF_TOK_CLOSE_PAREN)) {
                return true;
            }
            t->idx = save2;

            if (!_upf_parse_unary_expr(t, NULL)) {
                t->idx = save;
                return false;
            }
            return true;
        }

        if (strcmp(token->string, "_Alignof") == 0 || strcmp(token->string, "alignof") == 0) {
            if (!_upf_consume_any(t, _UPF_TOK_OPEN_PAREN) || !_upf_parse_typename(NULL, NULL, NULL, t)
                || !_upf_consume_any(t, _UPF_TOK_CLOSE_PAREN)) {
                t->idx = save;
                return false;
            }
            return true;
        }

        _upf_back(t);
    }

    return _upf_parse_postfix_expr(t, p);
}

static bool _upf_parse_expr(_upf_tokenizer *t, _upf_parser_state *p) {
    _UPF_ASSERT(t != NULL);
    // expr
    // 	: assignment_expr
    // 	| assignment_expr ',' expr

    // Copying is safe since vector's data is in the arena, and old entries are not modified.
    _upf_parser_state p_copy;
    size_t save = t->idx;
    do {
        if (p) p_copy = *p;
        if (!_upf_parse_assignment_expr(t, p ? &p_copy : NULL)) {
            t->idx = save;
            return false;
        }
    } while (_upf_consume_any(t, _UPF_TOK_COMMA));
    if (p) *p = p_copy;

    return true;
}

// ================== TYPE INFERENCE ======================

static _upf_type *_upf_get_member_type(const _upf_cstr_vec *member_names, size_t idx, _upf_type *type) {
    _UPF_ASSERT(member_names != NULL && type != NULL);

    if (idx == member_names->length) return type;

    if (type->kind == _UPF_TK_POINTER) {
        return _upf_get_member_type(member_names, idx, type->as.pointer.type);
    }

    if (type->kind == _UPF_TK_FUNCTION && idx < member_names->length) {
        do {
            type = type->as.function.return_type;
            if (type->kind == _UPF_TK_POINTER && type->as.pointer.type->kind == _UPF_TK_FUNCTION) type = type->as.pointer.type;
        } while (type->kind == _UPF_TK_FUNCTION);
        return _upf_get_member_type(member_names, idx, type);
    }

    if (type->kind == _UPF_TK_STRUCT || type->kind == _UPF_TK_UNION) {
        _upf_member_vec members = type->as.cstruct.members;
        for (uint32_t i = 0; i < members.length; i++) {
            if (strcmp(members.data[i].name, member_names->data[idx]) == 0) {
                return _upf_get_member_type(member_names, idx + 1, members.data[i].type);
            }
        }
    }

    _UPF_ERROR("Unable to find member \"%s\" in \"%s\".", member_names->data[idx], type->name);
}

static _upf_type *_upf_get_type_by_name(_upf_parser_state *p) {
    _UPF_ASSERT(p != NULL);

    for (uint32_t i = 0; i < p->cu->types.length; i++) {
        if (strcmp(p->cu->types.data[i].name, p->base.name) == 0) {
            return _upf_parse_type(p->cu, p->cu->types.data[i].die);
        }
    }

    return NULL;
}

static _upf_type *_upf_get_variable_type(const _upf_cu *cu, const _upf_scope *scope, uint64_t pc, const char *var_name) {
    _UPF_ASSERT(cu != NULL && scope != NULL && var_name != NULL);

    if (!_upf_is_in_range(pc, scope->ranges)) return NULL;

    for (uint32_t i = 0; i < scope->scopes.length; i++) {
        _upf_type *result = _upf_get_variable_type(cu, &scope->scopes.data[i], pc, var_name);
        if (result != NULL) return result;
    }

    for (uint32_t i = 0; i < scope->vars.length; i++) {
        if (strcmp(scope->vars.data[i].name, var_name) == 0) return _upf_parse_type(cu, scope->vars.data[i].die);
    }

    return NULL;
}

static _upf_type *_upf_get_function_return_type(_upf_parser_state *p) {
    _UPF_ASSERT(p != NULL);

    for (uint32_t i = 0; i < p->cu->functions.length; i++) {
        _upf_function function = p->cu->functions.data[i];

        if (strcmp(function.name, p->base.name) == 0) {
            return function.return_type_die == NULL ? _upf_get_void_type() : _upf_parse_type(p->cu, function.return_type_die);
        }
    }

    return NULL;
}

static _upf_type *_upf_get_base_type(_upf_parser_state *p, uint64_t pc, const char *arg) {
    _UPF_ASSERT(p != NULL && arg != NULL);

    _upf_type *type_ptr = NULL;
    switch (p->base_type) {
        case _UPF_BT_INT_TYPE:
            type_ptr = p->base.type;
            break;
        case _UPF_BT_TYPE:
            type_ptr = _upf_get_type_by_name(p);

            if (type_ptr == NULL) {
                _UPF_ERROR(
                    "Unable to find type \"%s\" in \"%s\" at %s:%d. "
                    "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).",
                    p->base.name, arg, _upf_state.file_path, _upf_state.line);
            }
            break;
        case _UPF_BT_VARIABLE:
            type_ptr = _upf_get_variable_type(p->cu, &p->cu->scope, pc, p->base.name);
            if (type_ptr != NULL) break;

            if (_upf_get_function_return_type(p) != NULL) {
                _upf_type type = {
                    .kind = _UPF_TK_FUNCTION,
                    .size = sizeof(void *),
                };
                type_ptr = _upf_add_type(NULL, type);
            }

            if (type_ptr == NULL) {
                _UPF_ERROR(
                    "Unable to find type of \"%s\" in \"%s\" at %s:%d. "
                    "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).",
                    p->base.name, arg, _upf_state.file_path, _upf_state.line);
            }
            break;
        case _UPF_BT_FUNCTION:
            type_ptr = _upf_get_variable_type(p->cu, &p->cu->scope, pc, p->base.name);

            if (type_ptr != NULL) {
                if (type_ptr->kind != _UPF_TK_POINTER) goto not_function_error;

                type_ptr = type_ptr->as.pointer.type;
                if (type_ptr->kind != _UPF_TK_FUNCTION) goto not_function_error;

                type_ptr = type_ptr->as.function.return_type;
            } else {
                type_ptr = _upf_get_function_return_type(p);
            }

            if (type_ptr == NULL) {
            not_function_error:
                _UPF_ERROR(
                    "Unable to find type of function \"%s\" in \"%s\" at %s:%d. "
                    "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).",
                    p->base.name, arg, _upf_state.file_path, _upf_state.line);
            }

            if (p->members.length == 0 && p->suffix_calls > 0) p->suffix_calls--;
            break;
    }

    _UPF_ASSERT(type_ptr != NULL);
    return type_ptr;
}

static _upf_type *_upf_dereference_type(_upf_type *type_ptr, int dereference, const char *arg) {
    _UPF_ASSERT(arg != NULL);

    // Arguments are pointers to data that should be printed, so they get dereferenced
    // in order not to be interpreted as actual pointers.
    dereference++;

    while (dereference < 0) {
        _upf_type type = {
            .kind = _UPF_TK_POINTER,
            .size = sizeof(void*),
            .as.pointer = {
                .type = type_ptr,
            },
        };

        type_ptr = _upf_add_type(NULL, type);
        dereference++;
    }

    while (dereference > 0) {
        if (type_ptr->kind == _UPF_TK_POINTER) {
            type_ptr = type_ptr->as.pointer.type;
            dereference--;
        } else if (type_ptr->kind == _UPF_TK_ARRAY) {
            int dimensions = type_ptr->as.array.lengths.length;
            if (dereference > dimensions) {
                goto not_pointer_error;
            } else if (dereference == dimensions) {
                type_ptr = type_ptr->as.array.element_type;
            } else {
                type_ptr = _upf_add_type(NULL, _upf_get_subarray(type_ptr, dereference));
            }

            dereference = 0;
        } else {
        not_pointer_error:
            _UPF_ERROR("Arguments must be pointers to data that should be printed. You must get pointer (&) of \"%s\" at %s:%d.", arg,
                       _upf_state.file_path, _upf_state.line);
        }

        if (type_ptr == NULL) {
            _UPF_ERROR(
                "Unable to print `void*` because it can point to arbitrary data of any length. "
                "To print the pointer itself, you must get pointer (&) of \"%s\" at %s:%d.",
                arg, _upf_state.file_path, _upf_state.line);
        }
    }

    return type_ptr;
}

static const _upf_type *_upf_get_arg_type(const char *arg, uint64_t pc) {
    _UPF_ASSERT(arg != NULL);

    _upf_tokenizer t = {0};
    _upf_tokenize(&t, arg);

    _upf_cu *cu = NULL;
    for (uint32_t i = 0; i < _upf_state.cus.length; i++) {
        if (_upf_is_in_range(pc, _upf_state.cus.data[i].scope.ranges)) {
            cu = &_upf_state.cus.data[i];
            break;
        }
    }
    _UPF_ASSERT(cu != NULL);

    _upf_parser_state p = {
        .cu = cu,
    };
    if (!_upf_parse_expr(&t, &p) || t.idx != t.tokens.length) {
        _UPF_ERROR("Unable to parse argument \"%s\" at %s:%d.", arg, _upf_state.file_path, _upf_state.line);
    }

    _upf_type *base_type = _upf_get_base_type(&p, pc, arg);
    _upf_type *member_type = _upf_get_member_type(&p.members, 0, base_type);
    if (p.suffix_calls > 0) member_type = _upf_get_return_type(member_type, p.suffix_calls);
    const _upf_type *type = _upf_dereference_type(member_type, p.dereference, arg);

    _UPF_ASSERT(type != NULL);
    return type;
}

// ================== /proc/pid/maps ======================

static _upf_range_vec _upf_get_readable_address_ranges(void) {
    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) _UPF_ERROR("Unable to open \"/proc/self/maps\": %s.", strerror(errno));

    _upf_range_vec ranges = {0};
    _upf_range range = {
        .start = UINT64_MAX,
        .end = UINT64_MAX,
    };
    char read_bit = '-';
    size_t length = 0;
    char *line = NULL;
    ssize_t read;
    while ((read = getline(&line, &length, file)) != -1) {
        if (read == 0) continue;
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        if (sscanf(line, "%lx-%lx %c%*s %*x %*x:%*x %*u", &range.start, &range.end, &read_bit) != 3) {
            free(line);
            fclose(file);
            _UPF_ERROR("Unable to parse \"/proc/self/maps\": invalid format.");
        }

        if (read_bit == 'r') _UPF_VECTOR_PUSH(&ranges, range);
    }
    if (line) free(line);
    fclose(file);

    return ranges;
}

static const void *_upf_get_memory_region_end(const void *ptr) {
    _UPF_ASSERT(ptr != NULL);
    for (uint32_t i = 0; i < _upf_state.addresses.length; i++) {
        _upf_range range = _upf_state.addresses.data[i];
        if ((void *) range.start <= ptr && ptr <= (void *) range.end) return (void *) range.end;
    }
    return NULL;
}

// ===================== PRINTING =========================

// All the printing is done to the global buffer stored in the _upf_state, which
// is why calls don't accept or return string pointers.

#define _UPF_INITIAL_BUFFER_SIZE 512

#define _upf_bprintf(...)                                                                         \
    while (true) {                                                                                \
        int bytes = snprintf(_upf_state.ptr, _upf_state.free, __VA_ARGS__);                       \
        if (bytes < 0) _UPF_ERROR("Unexpected error occurred in snprintf: %s.", strerror(errno)); \
        if (bytes >= _upf_state.free) {                                                           \
            int used = _upf_state.size - _upf_state.free;                                         \
            _upf_state.size *= 2;                                                                 \
            _upf_state.buffer = (char *) realloc(_upf_state.buffer, _upf_state.size);             \
            if (_upf_state.buffer == NULL) _UPF_OUT_OF_MEMORY();                                  \
            _upf_state.ptr = _upf_state.buffer + used;                                            \
            _upf_state.free = _upf_state.size - used;                                             \
            continue;                                                                             \
        }                                                                                         \
        _upf_state.free -= bytes;                                                                 \
        _upf_state.ptr += bytes;                                                                  \
        break;                                                                                    \
    }

static const char *_upf_escape_char(char ch) {
    // clang-format off
    switch (ch) {
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\\': return "\\\\";
    }
    // clang-format on

    static char str[2];
    str[0] = ch;
    str[1] = '\0';
    return str;
}

static bool _upf_is_printable(char c) { return ' ' <= c && c <= '~'; }

static void _upf_print_modifiers(int modifiers) {
    if (modifiers & _UPF_MOD_CONST) _upf_bprintf("const ");
    if (modifiers & _UPF_MOD_VOLATILE) _upf_bprintf("volatile ");
    if (modifiers & _UPF_MOD_RESTRICT) _upf_bprintf("restrict ");
    if (modifiers & _UPF_MOD_ATOMIC) _upf_bprintf("atomic ");
}

static void _upf_print_typename(const _upf_type *type, bool print_trailing_whitespace, bool is_return_type) {
    _UPF_ASSERT(type != NULL);

    switch (type->kind) {
        case _UPF_TK_POINTER: {
            if (type->as.pointer.type == NULL) {
                _upf_bprintf("void *");
                _upf_print_modifiers(type->modifiers);
                break;
            }

            const _upf_type *pointer_type = type->as.pointer.type;
            if (pointer_type->kind == _UPF_TK_FUNCTION) {
                _upf_print_typename(pointer_type, print_trailing_whitespace, is_return_type);
                break;
            }

            _upf_print_typename(pointer_type, true, is_return_type);
            _upf_bprintf("*");
            _upf_print_modifiers(type->modifiers);
        } break;
        case _UPF_TK_FUNCTION:
            if (is_return_type) _upf_bprintf("(");

            if (type->as.function.return_type == NULL) {
                _upf_bprintf("void");
            } else {
                _upf_print_typename(type->as.function.return_type, false, true);
            }

            _upf_bprintf("(");
            for (uint32_t i = 0; i < type->as.function.arg_types.length; i++) {
                if (i > 0) _upf_bprintf(", ");
                _upf_print_typename(type->as.function.arg_types.data[i], false, false);
            }
            _upf_bprintf(")");
            if (is_return_type) _upf_bprintf(")");
            if (print_trailing_whitespace) _upf_bprintf(" ");
            break;
        default:
            _upf_print_modifiers(type->modifiers);
            if (type->name) {
                _upf_bprintf("%s", type->name);
            } else {
                _upf_bprintf("<unnamed>");
            }
            if (print_trailing_whitespace) _upf_bprintf(" ");
            break;
    }
}

static void _upf_print_bit_field(const uint8_t *data, int total_bit_offset, int bit_size) {
    _UPF_ASSERT(data != NULL);

    int byte_offset = total_bit_offset / 8;
    int bit_offset = total_bit_offset % 8;

    uint8_t value;
    memcpy(&value, data + byte_offset, sizeof(value));
    value = (value >> bit_offset) & ((1 << bit_size) - 1);
    _upf_bprintf("%hhu <%d bit%s>", value, bit_size, bit_size > 1 ? "s" : "");
}

__attribute__((no_sanitize_address)) static void _upf_print_char_ptr(const char *str) {
    _UPF_ASSERT(str != NULL);

    const char *end = _upf_get_memory_region_end(str);
    if (end == NULL) {
        _upf_bprintf("%p (<out-of-bounds>)", (void *) str);
        return;
    }

    bool is_limited = UPRINTF_MAX_STRING_LENGTH > 0;
    bool is_truncated = false;
    const char *max_str = str + UPRINTF_MAX_STRING_LENGTH;
    _upf_bprintf("%p (\"", (void *) str);
    while (*str != '\0' && str < end) {
        if (is_limited && str >= max_str) {
            is_truncated = true;
            break;
        }
        _upf_bprintf("%s", _upf_escape_char(*str));
        str++;  // Increment inside of macro (_upf_bprintf) may be triggered twice
    }
    _upf_bprintf("\"");
    if (is_truncated) _upf_bprintf("...");
    _upf_bprintf(")");
}

static void _upf_collect_circular_structs(_upf_indexed_struct_vec *seen, _upf_indexed_struct_vec *circular, const uint8_t *data,
                                          const _upf_type *type, int depth) {
    _UPF_ASSERT(seen != NULL && circular != NULL && type != NULL);

    if (UPRINTF_MAX_DEPTH >= 0 && depth >= UPRINTF_MAX_DEPTH) return;
    if (data == NULL || _upf_get_memory_region_end(data) == NULL) return;

    if (type->kind == _UPF_TK_POINTER) {
        void *ptr;
        memcpy(&ptr, data, sizeof(ptr));
        if (ptr == NULL || type->as.pointer.type == NULL) return;

        _upf_collect_circular_structs(seen, circular, ptr, type->as.pointer.type, depth);
        return;
    }

    if (type->kind != _UPF_TK_STRUCT && type->kind != _UPF_TK_UNION) return;

    for (uint32_t i = 0; i < circular->length; i++) {
        if (circular->data[i].data == data && circular->data[i].type == type) return;
    }

    for (uint32_t i = 0; i < seen->length; i++) {
        if (seen->data[i].data == data && seen->data[i].type == type) {
            _UPF_VECTOR_PUSH(circular, seen->data[i]);
            return;
        }
    }

    _upf_indexed_struct indexed_struct = {
        .data = data,
        .type = type,
        .is_visited = false,
        .id = -1,
    };
    _UPF_VECTOR_PUSH(seen, indexed_struct);

    _upf_member_vec members = type->as.cstruct.members;
    for (uint32_t i = 0; i < members.length; i++) {
        const _upf_member *member = &members.data[i];
        if (member->bit_size == 0) {  // non-zero bit_size means bit field
            _upf_collect_circular_structs(seen, circular, data + member->offset, member->type, depth + 1);
        }
    }
}

// Brackets/parentheses printing guidelines:
// () -> additional information, e.g. pointer's value, char's ascii, enum's value, etc.
// [] -> arrays
// {} -> structs/unions
// <> -> meta information, e.g. unnamed, unknown, invalid, out of bounds, truncated, etc.
static void _upf_print_type(_upf_indexed_struct_vec *circular, const uint8_t *data, const _upf_type *type, int depth) {
    _UPF_ASSERT(circular != NULL && type != NULL);

    if (UPRINTF_MAX_DEPTH >= 0 && depth >= UPRINTF_MAX_DEPTH) {
        switch (type->kind) {
            case _UPF_TK_UNION:
            case _UPF_TK_STRUCT:
                _upf_bprintf("{...}");
                return;
            default:
                break;
        }
    }

    if (type->kind == _UPF_TK_UNKNOWN) {
        _upf_bprintf("<unknown>");
        return;
    }

    if (data == NULL) {
        _upf_bprintf("NULL");
        return;
    }

    if (_upf_get_memory_region_end(data) == NULL) {
        _upf_bprintf("<out-of-bounds>");
        return;
    }

    switch (type->kind) {
        case _UPF_TK_UNION:
            _upf_bprintf("<union> ");
            __attribute__((fallthrough));  // Handle union as struct
        case _UPF_TK_STRUCT: {
#if UPRINTF_IGNORE_STDIO_FILE
            if (strcmp(type->name, "FILE") == 0) {
                _upf_bprintf("<ignored>");
                return;
            }
#endif

            _upf_member_vec members = type->as.cstruct.members;

            if (members.length == 0) {
                _upf_bprintf("{}");
                return;
            }

            for (uint32_t i = 0; i < circular->length; i++) {
                if (circular->data[i].data == data && circular->data[i].type == type) {
                    if (circular->data[i].is_visited) {
                        _upf_bprintf("<points to #%d>", circular->data[i].id);
                        return;
                    }

                    circular->data[i].is_visited = true;
                    circular->data[i].id = _upf_state.circular_id++;
                    _upf_bprintf("<#%d> ", circular->data[i].id);
                }
            }

            _upf_bprintf("{\n");
            for (uint32_t i = 0; i < members.length; i++) {
                const _upf_member *member = &members.data[i];

                _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");
                _upf_print_typename(member->type, true, false);
                _upf_bprintf("%s = ", member->name);
                if (member->bit_size == 0) {
                    _upf_print_type(circular, data + member->offset, member->type, depth + 1);
                } else {
                    _upf_print_bit_field(data, member->offset, member->bit_size);
                }
                _upf_bprintf("\n");
            }
            _upf_bprintf("%*s}", UPRINTF_INDENTATION_WIDTH * depth, "");
        } break;
        case _UPF_TK_ENUM: {
            _upf_enum_vec enums = type->as.cenum.enums;
            const _upf_type *underlying_type = type->as.cenum.underlying_type;

            int64_t enum_value;
            if (underlying_type->kind == _UPF_TK_U4) {
                uint32_t temp;
                memcpy(&temp, data, sizeof(temp));
                enum_value = temp;
            } else if (underlying_type->kind == _UPF_TK_S4) {
                int32_t temp;
                memcpy(&temp, data, sizeof(temp));
                enum_value = temp;
            } else {
                _UPF_WARN("Expected enum to use int32_t or uint32_t. Ignoring this type.");
                _upf_bprintf("<enum>");
                break;
            }

            const char *name = NULL;
            for (uint32_t i = 0; i < enums.length; i++) {
                if (enums.data[i].value == enum_value) {
                    name = enums.data[i].name;
                    break;
                }
            }

            _upf_bprintf("%s (", name ? name : "<unknown>");
            _upf_print_type(circular, data, underlying_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_ARRAY: {
            const _upf_type *element_type = type->as.array.element_type;
            size_t element_size = element_type->size;

            if (element_size == UINT64_MAX) {
                _upf_bprintf("<unknown>");
                return;
            }

            if (type->as.array.lengths.length == 0) {
                _upf_bprintf("<non-static array>");
                return;
            }

            _upf_type subarray;
            if (type->as.array.lengths.length > 1) {
                subarray = _upf_get_subarray(type, 1);
                element_type = &subarray;

                for (uint32_t i = 0; i < subarray.as.array.lengths.length; i++) {
                    element_size *= subarray.as.array.lengths.data[i];
                }
            }

            bool is_primitive = _upf_is_primitive(element_type);
            _upf_bprintf(is_primitive ? "[" : "[\n");
            for (size_t i = 0; i < type->as.array.lengths.data[0]; i++) {
                if (i > 0) _upf_bprintf(is_primitive ? ", " : ",\n");
                if (!is_primitive) _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");

                const uint8_t *current = data + element_size * i;
                _upf_print_type(circular, current, element_type, depth + 1);

#if UPRINTF_ARRAY_COMPRESSION_THRESHOLD > 0
                size_t j = i;
                // Advance while element at `j` is same as at `i`.
                while (j < type->as.array.lengths.data[0] && memcmp(current, data + element_size * j, element_size) == 0) j++;

                int count = j - i;
                if (j - i >= UPRINTF_ARRAY_COMPRESSION_THRESHOLD) {
                    _upf_bprintf(" <repeats %d times>", count);
                    i = j - 1;
                }
#endif
            }

            if (is_primitive) {
                _upf_bprintf("]");
            } else {
                _upf_bprintf("\n%*s]", UPRINTF_INDENTATION_WIDTH * depth, "");
            }
        } break;
        case _UPF_TK_POINTER: {
            void *ptr;
            memcpy(&ptr, data, sizeof(ptr));
            if (ptr == NULL) {
                _upf_bprintf("NULL");
                return;
            }

            if (type->as.pointer.type == NULL) {
                _upf_bprintf("%p", ptr);
                return;
            }

            const _upf_type *pointed_type = type->as.pointer.type;
            if (pointed_type->kind == _UPF_TK_POINTER || pointed_type->kind == _UPF_TK_VOID) {
                _upf_bprintf("%p", ptr);
                return;
            }

            if (pointed_type->kind == _UPF_TK_FUNCTION) {
                _upf_print_type(circular, ptr, pointed_type, depth);
                break;
            }

            if (pointed_type->kind == _UPF_TK_SCHAR || pointed_type->kind == _UPF_TK_UCHAR) {
                _upf_print_char_ptr(ptr);
                return;
            }

            _upf_bprintf("%p (", ptr);
            _upf_print_type(circular, ptr, pointed_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_FUNCTION: {
            uint64_t absolute_function_pc = (uint64_t) data;
            uint64_t relative_function_pc = data - _upf_state.base;

            // Check pointer to function name mapping needed for extern functions
            const char *function_name = NULL;
            for (uint32_t i = 0; i < _upf_state.extern_functions.length; i++) {
                if (_upf_state.extern_functions.data[i].pc == absolute_function_pc) {
                    function_name = _upf_state.extern_functions.data[i].name;
                    _UPF_ASSERT(function_name != NULL);
                    break;
                }
            }

            // Find function (and cu) which matches either name(extern) or PC(local)
            _upf_function *function = NULL;
            _upf_cu *cu = NULL;
            for (uint32_t i = 0; i < _upf_state.cus.length; i++) {
                cu = &_upf_state.cus.data[i];

                for (uint32_t j = 0; j < cu->functions.length; j++) {
                    if (function_name == NULL ? (cu->functions.data[j].pc == relative_function_pc)
                                              : (strcmp(cu->functions.data[j].name, function_name) == 0)) {
                        function = &cu->functions.data[j];
                        break;
                    }
                }
                if (function != NULL) break;
            }

            _upf_bprintf("%p", (void *) data);
            if (function != NULL) {
                _UPF_ASSERT(cu != NULL);

                _upf_bprintf(" (");
                _upf_type *return_type
                    = function->return_type_die == NULL ? _upf_get_void_type() : _upf_parse_type(cu, function->return_type_die);
                _upf_print_typename(return_type, true, true);
                _upf_bprintf("%s(", function->name);
                for (uint32_t i = 0; i < function->args.length; i++) {
                    _upf_named_type arg = function->args.data[i];
                    _upf_type *arg_type = _upf_parse_type(cu, arg.die);
                    bool has_name = arg.name != NULL;

                    if (i > 0) _upf_bprintf(", ");
                    _upf_print_typename(arg_type, has_name, false);
                    if (has_name) _upf_bprintf("%s", arg.name);
                }
                if (function->is_variadic) {
                    _UPF_ASSERT(function->args.length > 0);
                    _upf_bprintf(", ...");
                }
                _upf_bprintf("))");
            }
        } break;
        case _UPF_TK_U1:
            _upf_bprintf("%hhu", *data);
            break;
        case _UPF_TK_U2: {
            uint16_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%hu", temp);
        } break;
        case _UPF_TK_U4: {
            uint32_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%u", temp);
        } break;
        case _UPF_TK_U8: {
            uint64_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%lu", temp);
        } break;
        case _UPF_TK_S1: {
            int8_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%hhd", temp);
        } break;
        case _UPF_TK_S2: {
            int16_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%hd", temp);
        } break;
        case _UPF_TK_S4: {
            int32_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%d", temp);
        } break;
        case _UPF_TK_S8: {
            int64_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%ld", temp);
        } break;
        case _UPF_TK_F4: {
            float temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%f", temp);
        } break;
        case _UPF_TK_F8: {
            double temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%lf", temp);
        } break;
        case _UPF_TK_BOOL:
            _upf_bprintf("%s", *data ? "true" : "false");
            break;
        case _UPF_TK_SCHAR: {
            char ch = *((char *) data);
            _upf_bprintf("%hhd", ch);
            if (_upf_is_printable(ch)) _upf_bprintf(" ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_UCHAR: {
            char ch = *((char *) data);
            _upf_bprintf("%hhu", ch);
            if (_upf_is_printable(ch)) _upf_bprintf(" ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_VOID:
            _UPF_WARN("Void must be a pointer. Ignoring this type.");
            break;
        case _UPF_TK_UNKNOWN:
            _upf_bprintf("<unknown>");
            break;
    }
}

// ===================== GETTING PC =======================

static int _upf_phdr_callback(struct dl_phdr_info *info_, size_t _size, void *data_) {
    _upf_dl_phdr_info *info = (_upf_dl_phdr_info *) info_;
    (void) _size;
    void **data = data_;

    // Empty name seems to indicate current executable
    if (strcmp(info->dlpi_name, "") == 0) {
        *data = (void *) info->dlpi_addr;
        return 1;  // return non-zero value to exit early
    }
    return 0;
}

// Returns address at which the current executable is loaded in memory.
static void *_upf_get_this_executable_address(void) {
    void *base = NULL;
    dl_iterate_phdr(&_upf_phdr_callback, &base);
    _UPF_ASSERT(base != NULL);
    return base;
}

// =================== ENTRY POINTS =======================

__attribute__((constructor)) void _upf_init(void) {
    if (setjmp(_upf_state.jmp_buf) != 0) return;

    if (access("/proc/self/exe", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/exe\" to be a valid path.");
    if (access("/proc/self/maps", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/maps\" to be a valid path.");

    _upf_state.base = _upf_get_this_executable_address();

    _upf_parse_elf();
    _upf_parse_extern_functions();
    _upf_parse_dwarf();

    _upf_state.size = _UPF_INITIAL_BUFFER_SIZE;
    _upf_state.buffer = (char *) malloc(_upf_state.size * sizeof(*_upf_state.buffer));
    if (_upf_state.buffer == NULL) _UPF_OUT_OF_MEMORY();
    _upf_state.is_init = true;
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_state.file to avoid unnecessarily copying date.
    if (_upf_state.file != NULL) munmap(_upf_state.file, _upf_state.file_size);
    if (_upf_state.buffer != NULL) free(_upf_state.buffer);
    _upf_arena_free(&_upf_state.arena);
}

__attribute__((noinline)) void _upf_uprintf(const char *file_path, int line, const char *fmt, const char *args_string, ...) {
    _UPF_ASSERT(file_path != NULL && line > 0 && fmt != NULL && args_string != NULL);

    if (setjmp(_upf_state.jmp_buf) != 0) return;
    if (!_upf_state.is_init) return;

    _upf_state.ptr = _upf_state.buffer;
    _upf_state.free = _upf_state.size;
    _upf_state.addresses = _upf_get_readable_address_ranges();
    _upf_state.circular_id = 0;
    _upf_state.file_path = file_path;
    _upf_state.line = line;

    uint8_t *pc_ptr = __builtin_extract_return_addr(__builtin_return_address(0));
    _UPF_ASSERT(pc_ptr != NULL);
    uint64_t pc = pc_ptr - _upf_state.base;

    char *args_string_copy = _upf_arena_string(&_upf_state.arena, args_string, args_string + strlen(args_string));
    _upf_cstr_vec args = _upf_get_args(args_string_copy);
    size_t arg_idx = 0;

    _upf_indexed_struct_vec seen = {0};
    _upf_indexed_struct_vec circular = {0};

    va_list va_args;
    va_start(va_args, args_string);
    const char *ch = fmt;
    while (true) {
        const char *start = ch;
        while (*ch != '%' && *ch != '\0') ch++;
        _upf_bprintf("%.*s", (int) (ch - start), start);

        if (*ch == '\0') break;
        ch++;  // Skip percent sign

        if (*ch == '%') {
            _upf_bprintf("%%");
        } else if (('a' <= *ch && *ch <= 'z') || ('A' <= *ch && *ch <= 'Z')) {
            if (arg_idx >= args.length) _UPF_ERROR("There are more format specifiers than arguments provided at %s:%d.", file_path, line);

            const void *ptr = va_arg(va_args, void *);
            const _upf_type *type = _upf_get_arg_type(args.data[arg_idx++], pc);
            seen.length = 0;
            circular.length = 0;
            _upf_collect_circular_structs(&seen, &circular, ptr, type, 0);
            _upf_print_type(&circular, ptr, type, 0);
        } else if (*ch == '\n' || *ch == '\0') {
            _UPF_ERROR("Unfinished format specifier at the end of the line at %s:%d.", file_path, line);
        } else {
            _UPF_ERROR("Unknown format specifier \"%%%c\" at %s:%d.", *ch, file_path, line);
        }

        ch++;
    }
    va_end(va_args);

    if (arg_idx < args.length) _UPF_ERROR("There are more arguments provided than format specifiers at %s:%d.", file_path, line);

    printf("%s", _upf_state.buffer);
    fflush(stdout);
}

// ====================== UNDEF ===========================

#undef _UPF_SET_TEST_STATUS
#undef _UPF_LOG
#undef _UPF_WARN
#undef _UPF_ERROR
#undef _UPF_ASSERT
#undef _UPF_OUT_OF_MEMORY
#undef _UPF_INITIAL_VECTOR_CAPACITY
#undef _UPF_VECTOR_TYPEDEF
#undef _UPF_VECTOR_INIT
#undef _UPF_VECTOR_PUSH
#undef _UPF_VECTOR_COPY
#undef _UPF_VECTOR_TOP
#undef _UPF_VECTOR_POP
#undef _UPF_MOD_CONST
#undef _UPF_MOD_VOLATILE
#undef _UPF_MOD_RESTRICT
#undef _UPF_MOD_ATOMIC
#undef _UPF_INITIAL_ARENA_SIZE
#undef _upf_arena_concat
#undef _upf_consume_any
#undef _UPF_INITIAL_BUFFER_SIZE
#undef _upf_bprintf

#endif  // UPRINTF_IMPLEMENTATION
