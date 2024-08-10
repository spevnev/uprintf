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
        __asm__("nop\n\t");                                                                      \
    } while (0)

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

// ===================== INCLUDES =========================

#define __USE_XOPEN_EXTENDED
#include <dwarf.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
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

// ===================== TESTING ==========================

#ifdef UPRINTF_TEST
static int _upf_test_status = EXIT_SUCCESS;

#define _UPF_SET_TEST_STATUS(status) _upf_test_status = status
#else
#define _UPF_SET_TEST_STATUS(status) (void) status
#endif

// ====================== ERRORS ==========================

#define _UPF_INVALID -1UL

#define _UPF_LOG(type, ...)                       \
    do {                                          \
        fprintf(stderr, "(uprintf) [%s] ", type); \
        fprintf(stderr, __VA_ARGS__);             \
        fprintf(stderr, "\n");                    \
    } while (0)

#define _UPF_ERROR(...)                      \
    do {                                     \
        _UPF_LOG("ERROR", __VA_ARGS__);      \
        _UPF_SET_TEST_STATUS(EXIT_FAILURE);  \
        longjmp(_upf_jmp_buf, EXIT_FAILURE); \
    } while (0)

#define _UPF_WARN(...)                      \
    do {                                    \
        _UPF_LOG("WARN", __VA_ARGS__);      \
        _UPF_SET_TEST_STATUS(EXIT_FAILURE); \
    } while (0)

#define _UPF_ASSERT(condition)                                                                        \
    do {                                                                                              \
        if (!(condition)) _UPF_ERROR("Assert (%s) failed at %s:%d.", #condition, __FILE__, __LINE__); \
    } while (0);

#define _UPF_OUT_OF_MEMORY() _UPF_ERROR("Process ran out of memory.")
#define _UPF_UNREACHABLE() _UPF_ERROR("Unreachable.");

// ====================== VECTOR ==========================

#define _UPF_INITIAL_VECTOR_CAPACITY 16

#define _UPF_VECTOR_NEW(a) \
    {                      \
        .arena = (a),      \
        .capacity = 0,     \
        .length = 0,       \
        .data = NULL,      \
    }

#define _UPF_VECTOR_TYPEDEF(name, type) \
    typedef struct {                    \
        struct _upf_arena *arena;       \
        size_t capacity;                \
        size_t length;                  \
        type *data;                     \
    } name

#define _UPF_VECTOR_PUSH(vec, element)                                     \
    do {                                                                   \
        if ((vec)->capacity == 0) {                                        \
            (vec)->capacity = _UPF_INITIAL_VECTOR_CAPACITY;                \
            size_t size = (vec)->capacity * sizeof(*(vec)->data);          \
            (vec)->data = _upf_arena_alloc((vec)->arena, size);            \
        } else if ((vec)->capacity == (vec)->length) {                     \
            size_t old_size = (vec)->capacity * sizeof(*(vec)->data);      \
            (vec)->capacity *= 2;                                          \
            void *new_data = _upf_arena_alloc((vec)->arena, old_size * 2); \
            memcpy(new_data, (vec)->data, old_size);                       \
            (vec)->data = new_data;                                        \
        }                                                                  \
        (vec)->data[(vec)->length++] = (element);                          \
    } while (0);

#define _UPF_VECTOR_TOP(vec) (vec)->data[(vec)->length - 1]

#define _UPF_VECTOR_POP(vec) (vec)->length--

// ====================== TYPES ===========================

typedef struct _upf_arena_region {
    uint8_t *data;
    size_t capacity;
    size_t length;
    struct _upf_arena_region *next;
} _upf_arena_region;

struct _upf_arena {
    _upf_arena_region *tail;
    _upf_arena_region *head;
};

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

enum _upf_type_kind {
    _UPF_TK_STRUCT,
    _UPF_TK_UNION,
    _UPF_TK_ENUM,
    _UPF_TK_ARRAY,
    _UPF_TK_POINTER,
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
    size_t type;
    size_t offset;
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

typedef struct {
    const char *name;
    enum _upf_type_kind kind;
    int modifiers;
    size_t size;
    union {
        struct {
            _upf_member_vec members;
        } cstruct;
        struct {
            size_t underlying_type;
            _upf_enum_vec enums;
        } cenum;
        struct {
            size_t element_type;
            _upf_size_t_vec lengths;
        } array;
        struct {
            size_t type;
        } pointer;
    } as;
} _upf_type;

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

enum _upf_token_kind {
    _UPF_TOK_NONE,
    _UPF_TOK_OPEN_PAREN,
    _UPF_TOK_CLOSE_PAREN,
    _UPF_TOK_OPEN_BRACKET,
    _UPF_TOK_CLOSE_BRACKET,
    _UPF_TOK_STAR,
    _UPF_TOK_AMPERSAND,
    _UPF_TOK_COMMA,
    _UPF_TOK_DOT,
    _UPF_TOK_ARROW,
    _UPF_TOK_NUMBER,
    _UPF_TOK_ID,
    _UPF_TOK_KEYWORD
};

typedef struct {
    enum _upf_token_kind kind;
    const char *string;
} _upf_token;

_UPF_VECTOR_TYPEDEF(_upf_token_vec, _upf_token);

typedef struct {
    _upf_token_vec tokens;
    size_t idx;
    // debug info
    const char *file;
    int line;
    _upf_cstr_vec args;
    size_t arg_idx;
} _upf_tokenizer;

struct _upf_dwarf {
    uint8_t *file;
    off_t file_size;

    bool is64bit;
    uint8_t offset_size;
    uint8_t address_size;

    const uint8_t *die;
    size_t die_size;
    const uint8_t *abbrev;
    const char *str;
    const char *line_str;
    const uint8_t *str_offsets;
    const uint8_t *addr;
    const uint8_t *rnglists;
};

typedef struct {
    const uint8_t *die;
    _upf_type type;
} _upf_type_map_entry;

_UPF_VECTOR_TYPEDEF(_upf_type_map_vec, _upf_type_map_entry);

typedef struct {
    const uint8_t *base;

    _upf_abbrev_vec abbrevs;
    _upf_named_type_vec types;

    uint64_t addr_base;
    uint64_t str_offsets_base;
    uint64_t rnglists_base;

    _upf_scope scope;
} _upf_cu;

_UPF_VECTOR_TYPEDEF(_upf_cu_vec, _upf_cu);

typedef struct {
    char *buffer;
    size_t size;

    char *ptr;
    size_t free;

    _upf_range_vec addresses;
} _upf_call_info;

// ================= GLOBAL VARIABLES =====================

#define _UPF_INITIAL_BUFFER_SIZE 512

static jmp_buf _upf_jmp_buf;
static bool _upf_is_init = false;
static struct _upf_arena _upf_arena;
static struct _upf_dwarf _upf_dwarf = {0};
static _upf_type_map_vec _upf_type_map = _UPF_VECTOR_NEW(&_upf_arena);
static _upf_cu_vec _upf_cus = _UPF_VECTOR_NEW(&_upf_arena);
static _upf_call_info _upf_call = {0};

// ====================== ARENA ===========================

#define _UPF_INITIAL_ARENA_SIZE 4096

static _upf_arena_region *_upf_arena_alloc_region(size_t capacity) {
    _upf_arena_region *region = (_upf_arena_region *) malloc(sizeof(*region));
    if (region == NULL) _UPF_OUT_OF_MEMORY();
    region->capacity = capacity;
    region->data = (uint8_t *) malloc(region->capacity * sizeof(*region->data));
    if (region->data == NULL) _UPF_OUT_OF_MEMORY();
    region->length = 0;
    region->next = NULL;
    return region;
}

static void _upf_arena_init(struct _upf_arena *a) {
    _UPF_ASSERT(a != NULL);

    _upf_arena_region *region = _upf_arena_alloc_region(_UPF_INITIAL_ARENA_SIZE);
    a->head = region;
    a->tail = region;
}

static void *_upf_arena_alloc(struct _upf_arena *a, size_t size) {
    _UPF_ASSERT(a != NULL && a->head != NULL && a->tail != NULL);

    size_t alignment = (a->head->length % sizeof(void *));
    if (alignment > 0) alignment = sizeof(void *) - alignment;

    if (alignment + size > a->head->capacity - a->head->length) {
        _upf_arena_region *region = _upf_arena_alloc_region(a->head->capacity * 2);
        a->head->next = region;
        a->head = region;
        alignment = 0;
    }

    void *memory = a->head->data + a->head->length + alignment;
    a->head->length += alignment + size;
    return memory;
}

static void _upf_arena_free(struct _upf_arena *a) {
    if (a == NULL || a->head == NULL || a->tail == NULL) return;

    _upf_arena_region *region = a->tail;
    while (region != NULL) {
        _upf_arena_region *next = region->next;

        free(region->data);
        free(region);

        region = next;
    }

    a->head = NULL;
    a->tail = NULL;
}

// Copies [begin, end) to arena-allocated string
static char *_upf_arena_string(struct _upf_arena *a, const char *begin, const char *end) {
    _UPF_ASSERT(a != NULL && begin != NULL && end != NULL);

    size_t len = end - begin;

    char *string = (char *) _upf_arena_alloc(a, len + 2);
    memcpy(string, begin, len);
    string[len] = '\0';

    return string;
}

static char *_upf_arena_concat2(struct _upf_arena *a, ...) {
    _UPF_ASSERT(a != NULL);

    va_list va_args;

    size_t size = 0;
    va_start(va_args, a);
    while (1) {
        const char *str = va_arg(va_args, const char *);
        if (str == NULL) break;
        size += strlen(str);
    }
    va_end(va_args);

    char *result = _upf_arena_alloc(a, size + 1);
    result[size] = '\0';

    char *p = result;
    va_start(va_args, a);
    while (1) {
        const char *str = va_arg(va_args, const char *);
        if (str == NULL) break;

        size_t len = strlen(str);
        memcpy(p, str, len);
        p += len;
    }
    va_end(va_args);

    return result;
}

#define _upf_arena_concat(a, ...) _upf_arena_concat2(a, __VA_ARGS__, NULL)

// ====================== HELPERS =========================

// Converts unsigned LEB128 to uint64_t and returns the size of LEB in bytes
static size_t _upf_uLEB_to_uint64(const uint8_t *leb, uint64_t *result) {
    _UPF_ASSERT(leb != NULL && result != NULL);

    static const uint8_t BITS_MASK = 0x7f;      // 01111111
    static const uint8_t CONTINUE_MASK = 0x80;  // 10000000

    size_t i = 0;
    int shift = 0;

    *result = 0;
    while (1) {
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
    while (1) {
        b = leb[i++];
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        shift += 7;
        if ((b & CONTINUE_MASK) == 0) break;
    }
    if ((shift < sizeof(*result) * 8) && (b & SIGN_MASK)) *result |= -(1 << shift);

    return i;
}

static const _upf_abbrev *_upf_get_abbrev(const _upf_cu *cu, size_t code) {
    _UPF_ASSERT(cu != NULL && code > 0 && code - 1 < cu->abbrevs.length);
    return &cu->abbrevs.data[code - 1];
}

static const _upf_type *_upf_get_type(size_t type_idx) {
    _UPF_ASSERT(type_idx != _UPF_INVALID && type_idx < _upf_type_map.length);
    return &_upf_type_map.data[type_idx].type;
}

static bool _upf_is_in_range(uint64_t pc, _upf_range_vec ranges) {
    for (size_t i = 0; i < ranges.length; i++) {
        if (ranges.data[i].start <= pc && pc < ranges.data[i].end) return true;
    }
    return false;
}

static bool _upf_is_primitive(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);

    switch (type->kind) {
        case _UPF_TK_STRUCT:
        case _UPF_TK_UNION:
        case _UPF_TK_ARRAY:
            return false;
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
    _UPF_UNREACHABLE();
}

// ================== DWARF PARSING =======================

static uint64_t _upf_offset_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);

    uint64_t offset = 0;
    memcpy(&offset, die, _upf_dwarf.offset_size);
    return offset;
}

static uint64_t _upf_address_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);

    uint64_t address = 0;
    memcpy(&address, die, _upf_dwarf.address_size);
    return address;
}

static size_t _upf_get_attr_size(const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(die != NULL);

    switch (form) {
        case DW_FORM_addr:
            return _upf_dwarf.address_size;
        case DW_FORM_strx1:
        case DW_FORM_addrx1:
        case DW_FORM_flag:
        case DW_FORM_ref1:
        case DW_FORM_data1:
        case DW_FORM_block1:
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
        case DW_FORM_block4:
            return 4;
        case DW_FORM_ref_sig8:
        case DW_FORM_ref_sup8:
        case DW_FORM_ref8:
        case DW_FORM_data8:
            return 8;
        case DW_FORM_data16:
            return 16;
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
            return _upf_dwarf.offset_size;
        case DW_FORM_indirect: {
            uint64_t form;
            size_t offset = _upf_uLEB_to_uint64(die, &form);
            return _upf_get_attr_size(die + offset, form);
        } break;
        case DW_FORM_flag_present:
        case DW_FORM_implicit_const:
            return 0;
    }
    _UPF_UNREACHABLE();
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
    _UPF_UNREACHABLE();
}

static const char *_upf_get_str(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    switch (form) {
        case DW_FORM_strp:
            return _upf_dwarf.str + _upf_offset_cast(die);
        case DW_FORM_line_strp:
            _UPF_ASSERT(_upf_dwarf.line_str != NULL);
            return _upf_dwarf.line_str + _upf_offset_cast(die);
        case DW_FORM_string:
            return (const char *) die;
        case DW_FORM_strx:
        case DW_FORM_strx1:
        case DW_FORM_strx2:
        case DW_FORM_strx3:
        case DW_FORM_strx4: {
            _UPF_ASSERT(_upf_dwarf.str_offsets != NULL && cu->str_offsets_base != _UPF_INVALID);
            uint64_t offset = _upf_get_x_offset(die, form) * _upf_dwarf.offset_size;
            return _upf_dwarf.str + _upf_offset_cast(_upf_dwarf.str_offsets + cu->str_offsets_base + offset);
        }
    }
    _UPF_UNREACHABLE();
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
    _UPF_UNREACHABLE();
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
            _UPF_ASSERT(_upf_dwarf.addr != NULL);
            uint64_t offset = cu->addr_base + _upf_get_x_offset(die, form) * _upf_dwarf.address_size;
            return _upf_address_cast(_upf_dwarf.addr + offset);
        }
    }
    _UPF_UNREACHABLE();
}

static const uint8_t *_upf_skip_die(const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(die != NULL && abbrev != NULL);

    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        die += _upf_get_attr_size(die, abbrev->attrs.data[i].form);
    }

    return die;
}

static enum _upf_type_kind _upf_get_type_kind(int64_t encoding, int64_t size) {
    switch (encoding) {
        case DW_ATE_boolean:
            if (size == 1) return _UPF_TK_BOOL;
            _UPF_WARN("Expected boolean to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_address:
            _UPF_WARN(
                "Segmented addresses aren't supported since x86_64/amd64 (the only supported architecture) doesn't have them. "
                "Ignoring this type.");
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
    _UPF_UNREACHABLE();
}

static _upf_type _upf_get_subarray(const _upf_type *array, int count) {
    _UPF_ASSERT(array != NULL);

    _upf_type subarray = *array;
    if (array->name != NULL) {
        subarray.name = _upf_arena_string(&_upf_arena, array->name, array->name + strlen(array->name) - 2 * count);
    }
    subarray.as.array.lengths.length -= count;
    subarray.as.array.lengths.data += count;

    return subarray;
}

static size_t _upf_add_type(const uint8_t *type_die, _upf_type type) {
    _upf_type_map_entry entry = {
        .die = type_die,
        .type = type,
    };
    _UPF_VECTOR_PUSH(&_upf_type_map, entry);

    return _upf_type_map.length - 1;
}

static size_t _upf_parse_type(const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    for (size_t i = 0; i < _upf_type_map.length; i++) {
        if (_upf_type_map.data[i].die == die) return i;
    }

    const uint8_t *base = die;

    uint64_t code;
    die += _upf_uLEB_to_uint64(die, &code);
    const _upf_abbrev *abbrev = _upf_get_abbrev(cu, code);

    const char *name = NULL;
    uint64_t subtype_offset = _UPF_INVALID;
    size_t size = _UPF_INVALID;
    int64_t encoding = 0;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
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
            _UPF_ASSERT(subtype_offset != _UPF_INVALID);

            _upf_type type = {
                .name = name,
                .kind = _UPF_TK_ARRAY,
                .modifiers = 0,
                .size = size,
                .as.array = {
                    .element_type = _upf_parse_type(cu, cu->base + subtype_offset),
                    .lengths = _UPF_VECTOR_NEW(&_upf_arena),
                },
            };

            const _upf_type *element_type = _upf_get_type(type.as.array.element_type);

            bool generate_name = element_type->name != NULL && type.name == NULL;
            if (generate_name) type.name = element_type->name;

            bool is_static = true;
            size_t array_size = element_type->size;
            while (1) {
                die += _upf_uLEB_to_uint64(die, &code);
                if (code == 0) break;

                abbrev = _upf_get_abbrev(cu, code);
                if (abbrev->tag == DW_TAG_enumeration_type) {
                    _UPF_WARN("Enumerator array subranges aren't unsupported. Ignoring this type.");
                    goto unknown_type;
                }
                _UPF_ASSERT(abbrev->tag == DW_TAG_subrange_type);

                size_t length = _UPF_INVALID;
                for (size_t i = 0; i < abbrev->attrs.length; i++) {
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

                if (length == _UPF_INVALID) {
                    is_static = false;
                    array_size = _UPF_INVALID;
                    type.as.array.lengths.length = 0;
                }

                if (is_static) {
                    array_size *= length;
                    _UPF_VECTOR_PUSH(&type.as.array.lengths, length);
                }

                if (generate_name) type.name = _upf_arena_concat(&_upf_arena, type.name, "[]");
            }

            if (element_type->size != _UPF_INVALID && type.size == _UPF_INVALID) {
                type.size = array_size;
            }

            return _upf_add_type(base, type);
        }
        case DW_TAG_enumeration_type: {
            _UPF_ASSERT(subtype_offset != _UPF_INVALID);

            _upf_type type = {
                .name = name ? name : "enum",
                .kind = _UPF_TK_ENUM,
                .modifiers = 0,
                .size = size,
                .as.cenum = {
                    .underlying_type = _upf_parse_type(cu, cu->base + subtype_offset),
                    .enums = _UPF_VECTOR_NEW(&_upf_arena),
                },
            };

            if (type.size == _UPF_INVALID) type.size = _upf_get_type(type.as.cenum.underlying_type)->size;

            while (1) {
                die += _upf_uLEB_to_uint64(die, &code);
                if (code == 0) break;

                abbrev = _upf_get_abbrev(cu, code);
                _UPF_ASSERT(abbrev->tag == DW_TAG_enumerator);

                bool found_value = false;
                _upf_enum cenum = {
                    .name = NULL,
                    .value = 0,
                };
                for (size_t i = 0; i < abbrev->attrs.length; i++) {
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

            return _upf_add_type(base, type);
        }
        case DW_TAG_pointer_type: {
            _UPF_ASSERT(size == _UPF_INVALID || size == sizeof(void *));

            _upf_type type = {
                .name = name,
                .kind = _UPF_TK_POINTER,
                .modifiers = 0,
                .size = sizeof(void*),
                .as.pointer = {
                    .type = _UPF_INVALID,
                },
            };

            // Pointers have to be added before their data gets filled in so that
            // structs that pointer to themselves, such as linked lists, don't get
            // stuck in an infinite loop.
            size_t type_idx = _upf_add_type(base, type);

            if (subtype_offset != _UPF_INVALID) {
                // `void*`s have invalid offset (since they don't point to any type), thus
                // pointer with invalid type represents a `void*`

                size_t subtype_idx = _upf_parse_type(cu, cu->base + subtype_offset);
                // Pointers inside vector may become invalid if _upf_parse_type fills up the vector triggering realloc
                _upf_type *type = &_upf_type_map.data[type_idx].type;
                type->as.pointer.type = subtype_idx;
                type->name = _upf_get_type(subtype_idx)->name;
            }

            return type_idx;
        }
        case DW_TAG_structure_type:
        case DW_TAG_union_type: {
            bool is_struct = abbrev->tag == DW_TAG_structure_type;
            _upf_type type = {
                .name = name ? name : (is_struct ? "struct" : "union"),
                .kind = is_struct ? _UPF_TK_STRUCT : _UPF_TK_UNION,
                .modifiers = 0,
                .size = size,
                .as.cstruct = {
                    .members = _UPF_VECTOR_NEW(&_upf_arena),
                },
            };

            while (1) {
                die += _upf_uLEB_to_uint64(die, &code);
                if (code == 0) break;

                abbrev = _upf_get_abbrev(cu, code);
                if (abbrev->tag != DW_TAG_member) {
                    die = _upf_skip_die(die, abbrev);
                    continue;
                }

                _upf_member member = {
                    .name = NULL,
                    .type = _UPF_INVALID,
                    .offset = is_struct ? _UPF_INVALID : 0,
                };
                for (size_t i = 0; i < abbrev->attrs.length; i++) {
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
                            _UPF_WARN("Non-constant member offsets aren't supported. Ignoring this type.");
                            goto unknown_type;
                        }
                    }

                    die += _upf_get_attr_size(die, attr.form);
                }
                _UPF_ASSERT(member.name != NULL && member.type != _UPF_INVALID && member.offset != _UPF_INVALID);

                _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
            }

            return _upf_add_type(base, type);
        }
        case DW_TAG_typedef: {
            _UPF_ASSERT(name != NULL);

            if (subtype_offset == _UPF_INVALID) {
                // void type is represented by absence of type attribute, e.g. typedef void NAME

                _upf_type type = {
                    .name = name,
                    .kind = _UPF_TK_VOID,
                    .modifiers = 0,
                    .size = _UPF_INVALID,
                };

                return _upf_add_type(base, type);
            }

            size_t type_idx = _upf_parse_type(cu, cu->base + subtype_offset);
            _upf_type type = *_upf_get_type(type_idx);
            type.name = name;

            if (type.kind == _UPF_TK_SCHAR && strcmp(name, "int8_t") == 0) type.kind = _UPF_TK_S1;
            else if (type.kind == _UPF_TK_UCHAR && strcmp(name, "uint8_t") == 0) type.kind = _UPF_TK_U1;

            return _upf_add_type(base, type);
        }
        case DW_TAG_base_type: {
            _UPF_ASSERT(name != NULL && size != _UPF_INVALID && encoding != 0);

            _upf_type type = {
                .name = name,
                .kind = _upf_get_type_kind(encoding, size),
                .modifiers = 0,
                .size = size,
            };

            return _upf_add_type(base, type);
        }
        case DW_TAG_const_type:
        case DW_TAG_volatile_type:
        case DW_TAG_restrict_type:
        case DW_TAG_atomic_type: {
            if (subtype_offset == _UPF_INVALID) {
                _upf_type type = {
                    .name = "void",
                    .kind = _UPF_TK_VOID,
                    .modifiers = _upf_get_type_modifier(abbrev->tag),
                    .size = sizeof(void *),
                };

                return _upf_add_type(base, type);
            } else {
                size_t type_idx = _upf_parse_type(cu, cu->base + subtype_offset);
                _upf_type type = *_upf_get_type(type_idx);
                type.modifiers |= _upf_get_type_modifier(abbrev->tag);

                return _upf_add_type(base, type);
            }
        }
        default:
            _UPF_WARN("Found unsupported type (0x%lx). Ignoring it.", abbrev->tag);
            break;
    }

    _upf_type type;
unknown_type:
    type = (_upf_type){
        .name = name,
        .kind = _UPF_TK_UNKNOWN,
        .modifiers = 0,
        .size = size,
    };
    return _upf_add_type(base, type);
}

static _upf_range_vec _upf_get_ranges(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL && _upf_dwarf.rnglists != NULL);

    const uint8_t *rnglist = NULL;
    if (form == DW_FORM_sec_offset) {
        rnglist = _upf_dwarf.rnglists + _upf_offset_cast(die);
    } else {
        _UPF_ASSERT(cu->rnglists_base != _UPF_INVALID);
        uint64_t index;
        _upf_uLEB_to_uint64(die, &index);
        uint64_t rnglist_offset = _upf_offset_cast(_upf_dwarf.rnglists + cu->rnglists_base + index * _upf_dwarf.offset_size);
        rnglist = _upf_dwarf.rnglists + cu->rnglists_base + rnglist_offset;
    }
    _UPF_ASSERT(rnglist != NULL);

    uint64_t base = 0;
    if (cu->scope.ranges.length == 1) {
        base = cu->scope.ranges.data[0].start;
    }

    _upf_range_vec ranges = _UPF_VECTOR_NEW(&_upf_arena);
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
                rnglist += _upf_dwarf.address_size;
                break;
            case DW_RLE_start_end: {
                uint64_t start = _upf_address_cast(rnglist);
                rnglist += _upf_dwarf.address_size;
                uint64_t end = _upf_address_cast(rnglist);
                rnglist += _upf_dwarf.address_size;

                _upf_range range = {
                    .start = start,
                    .end = end,
                };

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_start_length: {
                uint64_t address = _upf_address_cast(rnglist);
                rnglist += _upf_dwarf.address_size;
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

static _upf_named_type _upf_get_var(const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    uint64_t code;
    die += _upf_uLEB_to_uint64(die, &code);
    const _upf_abbrev *abbrev = _upf_get_abbrev(cu, code);

    _upf_named_type var = {
        .name = NULL,
        .die = NULL,
    };
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            var.name = _upf_get_str(cu, die, attr.form);
        } else if (attr.name == DW_AT_type) {
            var.die = cu->base + _upf_get_ref(die, attr.form);
        } else if (attr.name == DW_AT_abstract_origin) {
            return _upf_get_var(cu, cu->base + _upf_get_ref(die, attr.form));
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    return var;
}

static _upf_abbrev_vec _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _UPF_ASSERT(abbrev_table != NULL);

    _upf_abbrev_vec abbrevs = _UPF_VECTOR_NEW(&_upf_arena);
    while (1) {
        _upf_abbrev abbrev = {
            .attrs = _UPF_VECTOR_NEW(&_upf_arena),
        };
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.code);
        if (abbrev.code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_children = *abbrev_table;
        abbrev_table += sizeof(abbrev.has_children);

        while (1) {
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

static bool _upf_is_language_c(int64_t language) {
    switch (language) {
        case DW_LANG_C:
        case DW_LANG_C89:
        case DW_LANG_C99:
        case DW_LANG_C11:
            return true;
        default:
            return false;
    }
}

static _upf_range_vec _upf_get_cu_ranges(const _upf_cu *cu, const uint8_t *low_pc_die, _upf_attr low_pc_attr, const uint8_t *high_pc_die,
                                         _upf_attr high_pc_attr, const uint8_t *ranges_die, _upf_attr ranges_attr) {
    _UPF_ASSERT(cu != NULL);

    if (ranges_die != NULL) return _upf_get_ranges(cu, ranges_die, ranges_attr.form);

    _upf_range_vec ranges = _UPF_VECTOR_NEW(&_upf_arena);
    _upf_range range = {
        .start = _UPF_INVALID,
        .end = _UPF_INVALID,
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

static void _upf_parse_cu_scope(const _upf_cu *cu, _upf_scope_stack *scope_stack, int depth, const uint8_t *die,
                                const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && scope_stack != NULL && die != NULL && abbrev != NULL);

    if (!abbrev->has_children) return;
    _UPF_ASSERT(scope_stack->length > 0);

    _upf_scope new_scope = {
        .ranges = _UPF_VECTOR_NEW(&_upf_arena),
        .vars = _UPF_VECTOR_NEW(&_upf_arena),
        .scopes = _UPF_VECTOR_NEW(&_upf_arena),
    };

    uint64_t low_pc = _UPF_INVALID;
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = {0};
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
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

    if (low_pc != _UPF_INVALID) {
        if (high_pc_die == NULL) _UPF_ERROR("Expected CU to have both low and high PC.");

        _upf_range range = {
            .start = low_pc,
            .end = _UPF_INVALID,
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
        for (size_t i = 0; i < scope_stack->length && scope == NULL; i++) {
            scope = scope_stack->data[scope_stack->length - 1 - i].scope;
        }
        _UPF_ASSERT(scope != NULL);

        _UPF_VECTOR_PUSH(&scope->scopes, new_scope);
        stack_entry.scope = &_UPF_VECTOR_TOP(&scope->scopes);
    } else {
        stack_entry.scope = NULL;
    }

    _UPF_VECTOR_PUSH(scope_stack, stack_entry);
}

static void _upf_parse_cu_type(_upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    const uint8_t *die_base = die;

    uint64_t code;
    die += _upf_uLEB_to_uint64(die, &code);
    const _upf_abbrev *abbrev = _upf_get_abbrev(cu, code);

    const char *name = NULL;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) name = _upf_get_str(cu, die, attr.form);

        die += _upf_get_attr_size(die, attr.form);
    }
    if (abbrev->tag == DW_TAG_pointer_type) {
        if (name) return;
        name = "void";
    }
    if (name == NULL) return;

    _upf_named_type type = {
        .die = die_base,
        .name = name,
    };
    _UPF_VECTOR_PUSH(&cu->types, type);
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *die, const uint8_t *die_end, const uint8_t *abbrev_table) {
    _UPF_ASSERT(cu_base != NULL && die != NULL && die_end != NULL && abbrev_table != NULL);

    _upf_cu cu = {
        .base = cu_base,
        .abbrevs = _upf_parse_abbrevs(abbrev_table),
        .types = _UPF_VECTOR_NEW(&_upf_arena),
        .addr_base = 0,
        .str_offsets_base = _UPF_INVALID,
        .rnglists_base = _UPF_INVALID,
        .scope = {
            .ranges = {0},
            .vars = _UPF_VECTOR_NEW(&_upf_arena),
            .scopes = _UPF_VECTOR_NEW(&_upf_arena),
        },
    };

    uint64_t code;
    die += _upf_uLEB_to_uint64(die, &code);
    const _upf_abbrev *abbrev = _upf_get_abbrev(&cu, code);
    _UPF_ASSERT(abbrev->tag == DW_TAG_compile_unit);

    // Save to parse after the initializing addr_base, str_offsets, and rnglists_base
    const uint8_t *low_pc_die = NULL;
    _upf_attr low_pc_attr = {0};
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = {0};
    const uint8_t *ranges_die = NULL;
    _upf_attr ranges_attr = {0};
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
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
    _upf_scope_stack scope_stack = _UPF_VECTOR_NEW(&_upf_arena);

    _upf_scope_stack_entry stack_entry = {
        .depth = depth,
        .scope = &cu.scope,
    };
    _UPF_VECTOR_PUSH(&scope_stack, stack_entry);

    while (die < die_end) {
        const uint8_t *die_base = die;

        die += _upf_uLEB_to_uint64(die, &code);
        if (code == 0) {
            _UPF_ASSERT(scope_stack.length > 0);
            if (depth == _UPF_VECTOR_TOP(&scope_stack).depth) _UPF_VECTOR_POP(&scope_stack);
            depth--;
            continue;
        }

        abbrev = _upf_get_abbrev(&cu, code);
        if (abbrev->has_children) depth++;

        switch (abbrev->tag) {
            case DW_TAG_lexical_block:
            case DW_TAG_subprogram:
            case DW_TAG_inlined_subroutine:
                _upf_parse_cu_scope(&cu, &scope_stack, depth, die, abbrev);
                break;
            case DW_TAG_array_type:
            case DW_TAG_enumeration_type:
            case DW_TAG_pointer_type:
            case DW_TAG_structure_type:
            case DW_TAG_typedef:
            case DW_TAG_union_type:
            case DW_TAG_base_type:
                _upf_parse_cu_type(&cu, die_base);
                break;
            case DW_TAG_variable:
            case DW_TAG_formal_parameter: {
                _upf_scope *scope = _UPF_VECTOR_TOP(&scope_stack).scope;
                if (scope == NULL) break;

                _upf_named_type var = _upf_get_var(&cu, die_base);
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

    _UPF_ASSERT(cu.scope.scopes.length > 0);
    if (cu.scope.ranges.length == 1) {
        _upf_range *range = &cu.scope.ranges.data[0];

        if (range->start == _UPF_INVALID) {
            if (cu.scope.scopes.data[0].ranges.length == 0) {
                range->start = 0;
            } else {
                range->start = cu.scope.scopes.data[0].ranges.data[0].start;
            }
        }

        if (range->end == _UPF_INVALID) {
            if (_UPF_VECTOR_TOP(&cu.scope.scopes).ranges.length == 0) {
                range->end = UINT64_MAX;
            } else {
                range->end = _UPF_VECTOR_TOP(&_UPF_VECTOR_TOP(&cu.scope.scopes).ranges).end;
            }
        }
    }

    _UPF_VECTOR_PUSH(&_upf_cus, cu);
}

static void _upf_parse_dwarf(void) {
    const uint8_t *die = _upf_dwarf.die;
    const uint8_t *die_end = die + _upf_dwarf.die_size;
    while (die < die_end) {
        const uint8_t *cu_base = die;

        uint64_t length = 0;
        memcpy(&length, die, sizeof(uint32_t));
        die += sizeof(uint32_t);

        _upf_dwarf.is64bit = false;
        if (length == 0xffffffffU) {
            memcpy(&length, die, sizeof(uint64_t));
            die += sizeof(uint64_t);
            _upf_dwarf.is64bit = true;
        }

        _upf_dwarf.offset_size = _upf_dwarf.is64bit ? 8 : 4;

        const uint8_t *next = die + length;

        uint16_t version = 0;
        memcpy(&version, die, sizeof(version));
        die += sizeof(version);
        if (version != 5) _UPF_ERROR("uprintf only supports DWARF version 5.");

        uint8_t type = *die;
        die += sizeof(type);
        _UPF_ASSERT(type == DW_UT_compile);

        uint8_t address_size = *die;
        _UPF_ASSERT(_upf_dwarf.address_size == 0 || _upf_dwarf.address_size == address_size);
        _upf_dwarf.address_size = address_size;
        die += sizeof(address_size);

        uint64_t abbrev_offset = _upf_offset_cast(die);
        die += _upf_dwarf.offset_size;

        _upf_parse_cu(cu_base, die, next, _upf_dwarf.abbrev + abbrev_offset);

        die = next;
    }
}

// ===================== PRINTING =========================

// All the printing is done to the global buffer stored in the _upf_call, which
// is why calls don't accept or return string pointers.

#define _upf_bprintf(...)                                                                         \
    while (1) {                                                                                   \
        int bytes = snprintf(_upf_call.ptr, _upf_call.free, __VA_ARGS__);                         \
        if (bytes < 0) _UPF_ERROR("Unexpected error occurred in snprintf: %s.", strerror(errno)); \
        if ((size_t) bytes >= _upf_call.free) {                                                   \
            size_t used = _upf_call.size - _upf_call.free;                                        \
            _upf_call.size *= 2;                                                                  \
            _upf_call.buffer = realloc(_upf_call.buffer, _upf_call.size);                         \
            if (_upf_call.buffer == NULL) _UPF_OUT_OF_MEMORY();                                   \
            _upf_call.ptr = _upf_call.buffer + used;                                              \
            _upf_call.free = _upf_call.size - used;                                               \
            continue;                                                                             \
        }                                                                                         \
        _upf_call.free -= bytes;                                                                  \
        _upf_call.ptr += bytes;                                                                   \
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

static void _upf_print_typename(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);

    if (type->kind == _UPF_TK_POINTER) {
        if (type->as.pointer.type == _UPF_INVALID) {
            _upf_bprintf("void ");
        } else {
            _upf_print_typename(_upf_get_type(type->as.pointer.type));
        }
        _upf_bprintf("*");
        _upf_print_modifiers(type->modifiers);
    } else {
        _upf_print_modifiers(type->modifiers);
        _upf_bprintf("%s ", type->name ? type->name : "(unnamed)");
    }
}

__attribute__((no_sanitize_address)) static void _upf_print_char_ptr(const char *str) {
    const char *end = NULL;
    for (size_t i = 0; i < _upf_call.addresses.length; i++) {
        _upf_range range = _upf_call.addresses.data[i];
        if ((char *) range.start <= str && str <= (char *) range.end) {
            end = (char *) range.end;
            break;
        }
    }

    if (end == NULL) {
        _upf_bprintf("%p (<out-of-bounds>)", (void *) str);
        return;
    }

    _upf_bprintf("%p (\"", (void *) str);
    while (*str != '\0' && str < end) {
        _upf_bprintf("%s", _upf_escape_char(*str));
        str++;  // Increment inside of macro (_upf_bprintf) may be triggered twice
    }
    _upf_bprintf("\")");
}

static void _upf_print_type(const uint8_t *data, const _upf_type *type, int depth) {
    _UPF_ASSERT(type != NULL);

    if (UPRINTF_MAX_DEPTH != -1 && depth >= UPRINTF_MAX_DEPTH) {
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

            _upf_bprintf("{\n");
            for (size_t i = 0; i < members.length; i++) {
                const _upf_member *member = &members.data[i];
                const _upf_type *member_type = _upf_get_type(member->type);

                _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");
                _upf_print_typename(member_type);
                _upf_bprintf("%s = ", member->name);
                _upf_print_type(data + member->offset, member_type, depth + 1);
                _upf_bprintf("\n");
            }
            _upf_bprintf("%*s}", UPRINTF_INDENTATION_WIDTH * depth, "");
        } break;
        case _UPF_TK_ENUM: {
            _upf_enum_vec enums = type->as.cenum.enums;
            const _upf_type *underlying_type = _upf_get_type(type->as.cenum.underlying_type);

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
            for (size_t i = 0; i < enums.length; i++) {
                if (enum_value == enums.data[i].value) {
                    name = enums.data[i].name;
                    break;
                }
            }

            _upf_bprintf("%s (", name ? name : "<unknown>");
            _upf_print_type(data, underlying_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_ARRAY: {
            const _upf_type *element_type = _upf_get_type(type->as.array.element_type);
            size_t element_size = element_type->size;

            if (element_size == _UPF_INVALID) {
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

                for (size_t i = 0; i < subarray.as.array.lengths.length; i++) {
                    element_size *= subarray.as.array.lengths.data[i];
                }
            }

            bool is_primitive = _upf_is_primitive(element_type);
            _upf_bprintf(is_primitive ? "[" : "[\n");
            for (size_t i = 0; i < type->as.array.lengths.data[0]; i++) {
                if (i > 0) _upf_bprintf(is_primitive ? ", " : ",\n");
                if (!is_primitive) _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");

                const uint8_t *current = data + element_size * i;
                _upf_print_type(current, element_type, depth + 1);

#if UPRINTF_ARRAY_COMPRESSION_THRESHOLD > 0
                size_t j = i;
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

            if (type->as.pointer.type == _UPF_INVALID) {
                _upf_bprintf("%p", ptr);
                return;
            }

            const _upf_type *pointed_type = _upf_get_type(type->as.pointer.type);
            if (pointed_type->kind == _UPF_TK_POINTER || pointed_type->kind == _UPF_TK_VOID) {
                _upf_bprintf("%p", ptr);
                return;
            }

            if (pointed_type->kind == _UPF_TK_SCHAR || pointed_type->kind == _UPF_TK_UCHAR) {
                _upf_print_char_ptr(ptr);
                return;
            }

            _upf_bprintf("%p (", ptr);
            _upf_print_type(ptr, pointed_type, depth);
            _upf_bprintf(")");
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
            _UPF_WARN("void must be a pointer. Ignoring this type.");
            break;
        case _UPF_TK_UNKNOWN:
            _upf_bprintf("<unknown>");
            break;
    }

    return;
}

// ====================== PARSER ==========================

static const char *_upf_tok_to_str(enum _upf_token_kind kind) {
    // clang-format off
    switch (kind) {
        case _UPF_TOK_NONE:          return "none";
        case _UPF_TOK_OPEN_PAREN:    return "open paren";
        case _UPF_TOK_CLOSE_PAREN:   return "close paren";
        case _UPF_TOK_OPEN_BRACKET:  return "open bracket";
        case _UPF_TOK_CLOSE_BRACKET: return "close bracket";
        case _UPF_TOK_STAR:          return "star";
        case _UPF_TOK_AMPERSAND:     return "ampersand";
        case _UPF_TOK_COMMA:         return "comma";
        case _UPF_TOK_DOT:           return "dot";
        case _UPF_TOK_ARROW:         return "arrow";
        case _UPF_TOK_NUMBER:        return "number";
        case _UPF_TOK_ID:            return "identifier";
        case _UPF_TOK_KEYWORD:       return "keyword";
    }
    // clang-format on
    _UPF_UNREACHABLE();
}

static _upf_tokenizer _upf_tokenize(const char *file, int line, const char *string) {
    _UPF_ASSERT(file != NULL && string != NULL);

    static const _upf_token signs[] = {{_UPF_TOK_OPEN_PAREN, "("},    {_UPF_TOK_CLOSE_PAREN, ")"}, {_UPF_TOK_OPEN_BRACKET, "["},
                                       {_UPF_TOK_CLOSE_BRACKET, "]"}, {_UPF_TOK_STAR, "*"},        {_UPF_TOK_AMPERSAND, "&"},
                                       {_UPF_TOK_COMMA, ","},         {_UPF_TOK_DOT, "."},         {_UPF_TOK_ARROW, "->"}};
    static const char *keywords[] = {"struct", "union", "enum"};

    _upf_tokenizer tokenizer = {
        .tokens = _UPF_VECTOR_NEW(&_upf_arena),
        .idx = 0,
        .file = file,
        .line = line,
        .args = _UPF_VECTOR_NEW(&_upf_arena),
        .arg_idx = 0,
    };

    const char *ch = string;
    while (*ch != '\0') {
        if (*ch == ' ') {
            ch++;
            continue;
        }

        bool found = false;
        for (size_t i = 0; i < sizeof(signs) / sizeof(*signs) && !found; i++) {
            size_t len = strlen(signs[i].string);
            if (strncmp(ch, signs[i].string, len) == 0) {
                _UPF_VECTOR_PUSH(&tokenizer.tokens, signs[i]);
                ch += len;
                found = true;
            }
        }
        if (found) continue;

        if (*ch == '-' || ('0' <= *ch && *ch <= '9')) {
            errno = 0;
            char *end = NULL;
            strtol(ch, &end, 0);
            _UPF_ASSERT(errno == 0 && end != NULL);

            _upf_token token = {
                .kind = _UPF_TOK_NUMBER,
                .string = _upf_arena_string(&_upf_arena, ch, end),
            };
            _UPF_VECTOR_PUSH(&tokenizer.tokens, token);

            ch = end;
            continue;
        }

        if (('a' <= *ch && *ch <= 'z') || ('A' <= *ch && *ch <= 'Z') || *ch == '_') {
            const char *end = ch;
            while (('a' <= *end && *end <= 'z') || ('A' <= *end && *end <= 'Z') || ('0' <= *end && *end <= '9') || *end == '_') end++;

            const char *string = _upf_arena_string(&_upf_arena, ch, end);

            enum _upf_token_kind kind = _UPF_TOK_ID;
            for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
                if (strcmp(string, keywords[i]) == 0) {
                    kind = _UPF_TOK_KEYWORD;
                    break;
                }
            }

            _upf_token token = {
                .kind = kind,
                .string = string,
            };
            _UPF_VECTOR_PUSH(&tokenizer.tokens, token);

            ch = end;
            continue;
        }

        _UPF_ERROR("Unknown character '%c' when parsing arguments \"%s\" at %s:%d.", *ch, string, file, line);
    }

    const char *prev = string;
    ch = string;
    do {
        if (*ch == ',' || *ch == '\0') {
            const char *arg = _upf_arena_string(&_upf_arena, prev, ch);
            _UPF_VECTOR_PUSH(&tokenizer.args, arg);
            prev = ch + 1;
        }
    } while (*ch++ != '\0');

    return tokenizer;
}

static bool _upf_can_consume(const _upf_tokenizer *t) {
    _UPF_ASSERT(t != NULL);
    return t->idx < t->tokens.length;
}

static _upf_token _upf_get_next(const _upf_tokenizer *t) {
    _UPF_ASSERT(t != NULL);
    if (!_upf_can_consume(t)) {
        _UPF_ERROR("Expected a token but reached the end of the input(\"%s\") at %s:%d.", t->args.data[t->arg_idx], t->file, t->line);
    }

    return t->tokens.data[t->idx];
}

static void _upf_skip(_upf_tokenizer *t) {
    _UPF_ASSERT(t != NULL);
    if (!_upf_can_consume(t)) {
        _UPF_ERROR("Expected a token but reached the end of the input(\"%s\") at %s:%d.", t->args.data[t->arg_idx], t->file, t->line);
    }

    t->idx++;
}

static const char *_upf_consume(_upf_tokenizer *t, enum _upf_token_kind kind) {
    _UPF_ASSERT(t != NULL);
    if (!_upf_can_consume(t)) {
        _UPF_ERROR("Expected a token but reached the end of the input(\"%s\") at %s:%d.", t->args.data[t->arg_idx], t->file, t->line);
    }

    _upf_token token = t->tokens.data[t->idx++];
    if (token.kind != kind) {
        _UPF_ERROR("Expected a token of type %s, but found %s in \"%s\" at %s:%d.", _upf_tok_to_str(kind), _upf_tok_to_str(token.kind),
                   t->args.data[t->arg_idx], t->file, t->line);
    }

    return token.string;
}

static bool _upf_try_consume(_upf_tokenizer *t, enum _upf_token_kind kind) {
    _UPF_ASSERT(t != NULL);
    if (!_upf_can_consume(t)) return false;

    _upf_token token = t->tokens.data[t->idx];
    if (token.kind != kind) return false;

    t->idx++;
    return true;
}

static _upf_token _upf_consume_any2(_upf_tokenizer *t, ...) {
    _UPF_ASSERT(t != NULL);
    if (!_upf_can_consume(t)) {
        _UPF_ERROR("Expected a token but reached the end of the input(\"%s\") at %s:%d.", t->args.data[t->arg_idx], t->file, t->line);
    }

    _upf_token token = t->tokens.data[t->idx++];

    va_list va_args;
    va_start(va_args, t);
    while (1) {
        enum _upf_token_kind kind = va_arg(va_args, enum _upf_token_kind);
        if (kind == _UPF_TOK_NONE) break;

        if (token.kind == kind) {
            va_end(va_args);
            return token;
        }
    }
    va_end(va_args);

    va_start(va_args, t);
    static char tokens[104];
    char *ch = tokens;
    while (1) {
        enum _upf_token_kind kind = va_arg(va_args, enum _upf_token_kind);
        if (kind == _UPF_TOK_NONE) break;

        const char *str = _upf_tok_to_str(kind);
        size_t len = strlen(str);
        if (ch - tokens + len > 100) {
            ch -= 2;
            memcpy(ch, "...", 3);
            ch += 3;
            break;
        }

        memcpy(ch, str, len);
        ch += len;
        *ch++ = ',';
        *ch++ = ' ';
    }
    *(ch - 2) = '\0';
    va_end(va_args);

    _UPF_ERROR("Expected one of the following: %s; but found %s in \"%s\" at %s:%d.", tokens, _upf_tok_to_str(token.kind),
               t->args.data[t->arg_idx], t->file, t->line);
}

#define _upf_consume_any(t, ...) _upf_consume_any2(t, __VA_ARGS__, _UPF_TOK_NONE)

static const uint8_t *_upf_find_var_type(uint64_t pc, const char *var, const _upf_scope *scope) {
    _UPF_ASSERT(var != NULL && scope != NULL);

    if (!_upf_is_in_range(pc, scope->ranges)) return NULL;

    for (size_t i = 0; i < scope->scopes.length; i++) {
        const uint8_t *result = _upf_find_var_type(pc, var, &scope->scopes.data[i]);
        if (result != NULL) return result;
    }

    for (size_t i = 0; i < scope->vars.length; i++) {
        if (strcmp(scope->vars.data[i].name, var) == 0) {
            return scope->vars.data[i].die;
        }
    }

    return NULL;
}

static size_t _upf_get_member_type(_upf_cstr_vec member_names, size_t idx, size_t type_idx) {
    if (idx == member_names.length) return type_idx;

    const _upf_type *type = _upf_get_type(type_idx);

    if (type->kind == _UPF_TK_POINTER) {
        return _upf_get_member_type(member_names, idx, type->as.pointer.type);
    }

    if (type->kind != _UPF_TK_STRUCT && type->kind != _UPF_TK_UNION) return _UPF_INVALID;

    _upf_member_vec members = type->as.cstruct.members;
    for (size_t i = 0; i < members.length; i++) {
        if (strcmp(members.data[i].name, member_names.data[idx]) == 0) {
            return _upf_get_member_type(member_names, idx + 1, members.data[i].type);
        }
    }

    _UPF_WARN("Unable to find member \"%s\" in \"%s\".", member_names.data[idx], type->name);
    return _UPF_INVALID;
}

static size_t _upf_get_expression_type(const _upf_cu *cu, const uint8_t *type_die, _upf_cstr_vec vars) {
    _UPF_ASSERT(cu != NULL && type_die != NULL);
    size_t base_type = _upf_parse_type(cu, type_die);
    return _upf_get_member_type(vars, 1, base_type);
}

static _upf_size_t_vec _upf_parse_args(_upf_tokenizer *t, uint64_t pc) {
    _UPF_ASSERT(t != NULL);

    _upf_size_t_vec types = _UPF_VECTOR_NEW(&_upf_arena);
    _upf_cstr_vec vars = _UPF_VECTOR_NEW(&_upf_arena);
    while (_upf_can_consume(t)) {
        const char *casted_typename = NULL;
        int dereference = 0;
        vars.length = 0;

        bool paren = false;
        while (_upf_can_consume(t) && _upf_get_next(t).kind != _UPF_TOK_ID && _upf_get_next(t).kind != _UPF_TOK_KEYWORD) {
            _upf_token token = _upf_consume_any(t, _UPF_TOK_AMPERSAND, _UPF_TOK_STAR, _UPF_TOK_OPEN_PAREN);
            if (token.kind == _UPF_TOK_AMPERSAND) dereference--;
            if (token.kind == _UPF_TOK_STAR) dereference++;
            paren = token.kind == _UPF_TOK_OPEN_PAREN;
        }

        _upf_try_consume(t, _UPF_TOK_KEYWORD);
        const char *type_or_var = _upf_consume(t, _UPF_TOK_ID);

        if (paren && _upf_try_consume(t, _UPF_TOK_STAR)) {
            casted_typename = type_or_var;
            dereference--;

            while (_upf_try_consume(t, _UPF_TOK_STAR)) dereference--;
            _upf_consume(t, _UPF_TOK_CLOSE_PAREN);

            while (_upf_can_consume(t) && !_upf_try_consume(t, _UPF_TOK_COMMA)) _upf_skip(t);
        } else {
            _UPF_VECTOR_PUSH(&vars, type_or_var);
            while (_upf_can_consume(t) && !_upf_try_consume(t, _UPF_TOK_COMMA)) {
                while (_upf_try_consume(t, _UPF_TOK_CLOSE_PAREN)) continue;

                int array_count = 0;
                while (_upf_try_consume(t, _UPF_TOK_OPEN_BRACKET)) {
                    _upf_consume(t, _UPF_TOK_NUMBER);
                    _upf_consume(t, _UPF_TOK_CLOSE_BRACKET);
                    array_count++;
                }

                while (_upf_try_consume(t, _UPF_TOK_CLOSE_PAREN)) continue;
                if (!_upf_can_consume(t) || _upf_try_consume(t, _UPF_TOK_COMMA)) {
                    dereference += array_count;
                    break;
                }

                _upf_consume_any(t, _UPF_TOK_DOT, _UPF_TOK_ARROW);
                _UPF_VECTOR_PUSH(&vars, _upf_consume(t, _UPF_TOK_ID));
            }
        }

        size_t type_idx = _UPF_INVALID;
        if (casted_typename != NULL) {
            for (size_t i = 0; i < _upf_cus.length && type_idx == _UPF_INVALID; i++) {
                const _upf_cu *cu = &_upf_cus.data[i];
                if (!_upf_is_in_range(pc, cu->scope.ranges)) continue;

                for (size_t j = 0; j < cu->types.length && type_idx == _UPF_INVALID; j++) {
                    if (strcmp(cu->types.data[j].name, casted_typename) == 0) {
                        type_idx = _upf_parse_type(cu, cu->types.data[j].die);
                    }
                }
            }
            if (type_idx == _UPF_INVALID) {
                _UPF_ERROR(
                    "Unable to find type \"%s\" in \"%s\" at %s:%d. "
                    "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).",
                    casted_typename, t->args.data[t->arg_idx], t->file, t->line);
            }
        } else {
            if (vars.length == 0) {
                _UPF_ERROR("Unable to find variable name while parsing \"%s\" at %s:%d.", t->args.data[t->arg_idx], t->file, t->line);
            }

            for (size_t i = 0; i < _upf_cus.length; i++) {
                const _upf_cu *cu = &_upf_cus.data[i];

                const uint8_t *type_die = _upf_find_var_type(pc, vars.data[0], &cu->scope);
                if (type_die == NULL) continue;

                type_idx = _upf_get_expression_type(cu, type_die, vars);
                break;
            }
            if (type_idx == _UPF_INVALID) {
                _UPF_ERROR(
                    "Unable to find type of \"%s\" in \"%s\" at %s:%d. "
                    "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3).",
                    vars.data[0], t->args.data[t->arg_idx], t->file, t->line);
            }
        }

        // Arguments are pointers to data that should be printed, so they get dereferenced
        // in order not to be interpreted as actual pointers.
        dereference++;

        while (dereference < 0) {
            _upf_type type = {
                .name = NULL,
                .kind = _UPF_TK_POINTER,
                .modifiers = 0,
                .size = sizeof(void*),
                .as.pointer = {
                    .type = type_idx,
                },
            };

            type_idx = _upf_add_type(NULL, type);
            dereference++;
        }

        while (dereference > 0) {
            const _upf_type *type = _upf_get_type(type_idx);

            if (type->kind == _UPF_TK_POINTER) {
                type_idx = type->as.pointer.type;
                dereference--;
            } else if (type->kind == _UPF_TK_ARRAY) {
                int dimensions = type->as.array.lengths.length;
                if (dereference > dimensions) {
                    goto arg_isn_t_pointer_error;
                } else if (dereference == dimensions) {
                    type_idx = type->as.array.element_type;
                } else {
                    // This may invalidate type pointer
                    type_idx = _upf_add_type(NULL, _upf_get_subarray(type, dereference));
                }

                dereference = 0;
            } else {
            arg_isn_t_pointer_error:
                _UPF_ERROR("Arguments must be pointers to data that should be printed. You must take pointer (&) of \"%s\" at %s:%d.",
                           t->args.data[t->arg_idx], t->file, t->line);
            }

            if (type_idx == _UPF_INVALID) {
                _UPF_ERROR(
                    "Unable to print void* because it can point to arbitrary data of any length. "
                    "To print the pointer itself, you must take pointer (&) of \"%s\" at %s:%d.",
                    t->args.data[t->arg_idx], t->file, t->line);
            }
        }

        _UPF_VECTOR_PUSH(&types, type_idx)
        t->arg_idx++;
    }

    return types;
}

// ======================= ELF ============================

static void _upf_parse_elf(void) {
    struct stat file_info;
    if (stat("/proc/self/exe", &file_info) == -1) _UPF_ERROR("Unable to stat \"/proc/self/exe\": %s.", strerror(errno));
    size_t size = file_info.st_size;
    _upf_dwarf.file_size = size;

    int fd = open("/proc/self/exe", O_RDONLY);
    _UPF_ASSERT(fd != -1);

    uint8_t *file = (uint8_t *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    _UPF_ASSERT(file != MAP_FAILED);
    _upf_dwarf.file = file;

    close(fd);

    const Elf64_Ehdr *header = (Elf64_Ehdr *) file;

    if (memcmp(header->e_ident, ELFMAG, SELFMAG) != 0 || header->e_ident[EI_CLASS] != ELFCLASS64 || header->e_ident[EI_VERSION] != 1
        || header->e_machine != EM_X86_64 || header->e_version != 1 || header->e_shentsize != sizeof(Elf64_Shdr)) {
        _UPF_ERROR("Unsupported or invalid ELF file.");
    }

    const Elf64_Shdr *string_section = (Elf64_Shdr *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);

    const Elf64_Shdr *section = (Elf64_Shdr *) (file + header->e_shoff);
    for (size_t i = 0; i < header->e_shnum; i++) {
        const char *name = string_table + section->sh_name;

        if (strcmp(name, ".debug_info") == 0) {
            _upf_dwarf.die = file + section->sh_offset;
            _upf_dwarf.die_size = section->sh_size;
        } else if (strcmp(name, ".debug_abbrev") == 0) {
            _upf_dwarf.abbrev = file + section->sh_offset;
        } else if (strcmp(name, ".debug_str") == 0) {
            _upf_dwarf.str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_line_str") == 0) {
            _upf_dwarf.line_str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_str_offsets") == 0) {
            _upf_dwarf.str_offsets = file + section->sh_offset;
        } else if (strcmp(name, ".debug_rnglists") == 0) {
            _upf_dwarf.rnglists = file + section->sh_offset;
        } else if (strcmp(name, ".debug_addr") == 0) {
            _upf_dwarf.addr = file + section->sh_offset;
        }

        section++;
    }

    if (_upf_dwarf.die == NULL || _upf_dwarf.abbrev == NULL || _upf_dwarf.str == NULL) {
        _UPF_ERROR("Unable to find debugging information. Ensure that the executable contains it by using -g2 or -g3.");
    }
}

// ================== /proc/pid/maps ======================

// Function returns the address to which this executable is mapped to.
// It is retrieved by reading `/proc/self/maps` (see `man proc_pid_maps`).
// It is used to convert between DWARF addresses and runtime/real ones: DWARF
// addresses are relative to the beginning of the file, thus real = base + dwarf.
static void *_upf_get_this_file_address(void) {
    static const ssize_t PATH_BUFFER_SIZE = 1024;

    char this_path[PATH_BUFFER_SIZE];
    ssize_t read = readlink("/proc/self/exe", this_path, PATH_BUFFER_SIZE);
    if (read == -1) _UPF_ERROR("Unable to readlink \"/proc/self/exe\": %s.", strerror(errno));
    if (read == PATH_BUFFER_SIZE) _UPF_ERROR("Unable to readlink \"/proc/self/exe\": path is too long.");
    this_path[read] = '\0';

    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) _UPF_ERROR("Unable to open \"/proc/self/maps\": %s.", strerror(errno));

    uint64_t address = _UPF_INVALID;
    size_t length = 0;
    char *line = NULL;
    while ((read = getline(&line, &length, file)) != -1) {
        if (read == 0) continue;
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        int path_offset;
        if (sscanf(line, "%lx-%*x %*s %*x %*x:%*x %*u %n", &address, &path_offset) != 1) {
            _UPF_ERROR("Unable to parse \"/proc/self/maps\": invalid format.");
        }

        if (strcmp(this_path, line + path_offset) == 0) break;
    }
    if (line) free(line);
    fclose(file);

    _UPF_ASSERT(address != _UPF_INVALID);
    return (void *) address;
}

static _upf_range_vec _upf_get_address_ranges(void) {
    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) _UPF_ERROR("Unable to open \"/proc/self/maps\": %s.", strerror(errno));

    _upf_range_vec ranges = _UPF_VECTOR_NEW(&_upf_arena);
    _upf_range range = {
        .start = _UPF_INVALID,
        .end = _UPF_INVALID,
    };
    size_t length = 0;
    char *line = NULL;
    ssize_t read;
    while ((read = getline(&line, &length, file)) != -1) {
        if (read == 0) continue;
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        if (sscanf(line, "%lx-%lx %*s %*x %*x:%*x %*u", &range.start, &range.end) != 2) {
            _UPF_ERROR("Unable to parse \"/proc/self/maps\": invalid format.");
        }

        _UPF_VECTOR_PUSH(&ranges, range);
    }
    if (line) free(line);
    fclose(file);

    return ranges;
}

// =================== ENTRY POINTS =======================

__attribute__((constructor)) void _upf_init(void) {
    if (setjmp(_upf_jmp_buf) != 0) return;

    if (access("/proc/self/exe", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/exe\" to be a valid path.");
    if (access("/proc/self/maps", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/maps\" to be a valid path.");

    _upf_arena_init(&_upf_arena);

    _upf_parse_elf();
    _upf_parse_dwarf();

    _upf_is_init = true;
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    if (_upf_dwarf.file != NULL) munmap(_upf_dwarf.file, _upf_dwarf.file_size);
    if (_upf_call.buffer != NULL) free(_upf_call.buffer);
    _upf_arena_free(&_upf_arena);
}

__attribute__((noinline)) void _upf_uprintf(const char *file, int line, const char *fmt, const char *args, ...) {
    _UPF_ASSERT(file != NULL && line > 0 && fmt != NULL && args != NULL);

    if (!_upf_is_init) return;
    if (setjmp(_upf_jmp_buf) != 0) return;

    if (_upf_call.buffer == NULL) {
        _upf_call.size = _UPF_INITIAL_BUFFER_SIZE;
        _upf_call.buffer = (char *) malloc(_upf_call.size * sizeof(*_upf_call.buffer));
        if (_upf_call.buffer == NULL) _UPF_OUT_OF_MEMORY();
    }
    _upf_call.ptr = _upf_call.buffer;
    _upf_call.free = _upf_call.size;
    _upf_call.addresses = _upf_get_address_ranges();

    void *return_pc = __builtin_extract_return_addr(__builtin_return_address(0));
    _UPF_ASSERT(return_pc != NULL);
#ifdef __clang__
    static char *this_file = NULL;
    if (this_file == NULL) this_file = _upf_get_this_file_address();
    uint64_t pc = ((char *) return_pc) - this_file;
#else
    (void) _upf_get_this_file_address;
    uint64_t pc = (uint64_t) return_pc;
#endif

    _upf_tokenizer t = _upf_tokenize(file, line, args);
    _upf_size_t_vec types = _upf_parse_args(&t, pc);
    size_t arg_idx = 0;

    va_list va_args;
    va_start(va_args, args);
    const char *ch = fmt;
    while (1) {
        const char *start = ch;
        while (*ch != '%' && *ch != '\0') ch++;
        _upf_bprintf("%.*s", (int) (ch - start), start);

        if (*ch == '\0') break;
        ch++;  // Skip percent sign

        switch (*ch) {
            case '%':
                _upf_bprintf("%%");
                break;
            case 'S': {
                if (arg_idx >= types.length) _UPF_ERROR("There are more format specifiers than arguments provided at %s:%d.", file, line);

                const void *ptr = va_arg(va_args, void *);
                const _upf_type *type = _upf_get_type(types.data[arg_idx++]);
                _upf_print_type(ptr, type, 0);
            } break;
            case '\0':
            case '\n':
                _UPF_ERROR("Unfinished format specifier at the end of the line at %s:%d.", file, line);
            default:
                _UPF_ERROR("Unknown format specifier \"%%%c\" at %s:%d.", *ch, file, line);
        }
        ch++;
    }
    va_end(va_args);

    if (arg_idx < types.length) _UPF_ERROR("There are more arguments provided than format specifiers at %s:%d.", file, line);

    printf("%s", _upf_call.buffer);
    fflush(stdout);
}

// ====================== UNDEF ===========================

#undef _UPF_SET_TEST_STATUS
#undef _UPF_INVALID
#undef _UPF_LOG
#undef _UPF_ERROR
#undef _UPF_WARN
#undef _UPF_ASSERT
#undef _UPF_OUT_OF_MEMORY
#undef _UPF_UNREACHABLE
#undef _UPF_INITIAL_VECTOR_CAPACITY
#undef _UPF_VECTOR_NEW
#undef _UPF_VECTOR_TYPEDEF
#undef _UPF_VECTOR_PUSH
#undef _UPF_VECTOR_TOP
#undef _UPF_VECTOR_POP
#undef _UPF_MOD_CONST
#undef _UPF_MOD_VOLATILE
#undef _UPF_MOD_RESTRICT
#undef _UPF_MOD_ATOMIC
#undef _UPF_INITIAL_BUFFER_SIZE
#undef _UPF_INITIAL_ARENA_SIZE
#undef _upf_arena_concat
#undef _upf_bprintf
#undef _upf_consume_any

#endif  // UPRINTF_IMPLEMENTATION
