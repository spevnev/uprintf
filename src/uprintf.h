// Usage and other information can be found in README at https://github.com/spevnev/uprintf

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


#ifndef __linux__
#error ERROR: uprintf only supports Linux
#endif

#ifdef __cplusplus
#error ERROR: uprintf does NOT support C++
#endif


#ifndef UPRINTF_H
#define UPRINTF_H

#include <stdint.h>

void _upf_uprintf(const char *file, uint64_t line, const char *fmt, const char *args, ...);

// If variadic arguments were to be stringified directly, the arguments which
// use macros would stringify to the macro name instead of being expanded, but
// by calling another macro the argument-macros will be expanded and stringified
// to their real values.
#define _upf_stringify_va_args(...) #__VA_ARGS__

#define uprintf(fmt, ...) _upf_uprintf(__FILE__, __LINE__, fmt, _upf_stringify_va_args(__VA_ARGS__), __VA_ARGS__);

#endif  // UPRINTF_H


#ifdef UPRINTF_IMPLEMENTATION

#define __USE_XOPEN_EXTENDED
#include <assert.h>
#include <dwarf.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


// Feature test macros might not work since it is possible that the header has
// been included before this one and thus expanded without the macro, so the
// functions must be declared here.

ssize_t readlink(const char *path, char *buf, size_t bufsiz);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);


// number of spaces of indentation per each layer of nesting
#ifndef UPRINTF_INDENT
#define UPRINTF_INDENT 4
#endif


#define INVALID -1UL

#define ERROR(...)                            \
    do {                                      \
        fprintf(stderr, "[ERROR] uprintf: "); \
        fprintf(stderr, __VA_ARGS__);         \
        exit(1);                              \
    } while (0)

#define OUT_OF_MEMORY() ERROR("process ran out of memory.\n")
#define UNREACHABLE() assert(0 && "Unreachable.\n");


#define INITIAL_ARENA_SIZE 4096

typedef struct _upf_arena_region {
    uint8_t *data;
    size_t capacity;
    size_t length;
    struct _upf_arena_region *next;
} _upf_arena_region;

typedef struct {
    _upf_arena_region *tail;
    _upf_arena_region *head;
} _upf_arena;

static void _upf_arena_init(_upf_arena *arena) {
    _upf_arena_region *region = (_upf_arena_region *) malloc(sizeof(*region));
    if (region == NULL) OUT_OF_MEMORY();
    region->capacity = INITIAL_ARENA_SIZE;
    region->data = (uint8_t *) malloc(region->capacity);
    if (region->data == NULL) OUT_OF_MEMORY();
    region->length = 0;
    region->next = NULL;

    arena->head = region;
    arena->tail = region;
}

static uint8_t *_upf_arena_alloc(_upf_arena *arena, size_t size) {
    assert(arena->head != NULL && arena->tail != NULL);

    size_t alignment = (arena->head->length % sizeof(void *));
    if (alignment > 0) alignment = sizeof(void *) - alignment;

    if (alignment + size > arena->head->capacity - arena->head->length) {
        _upf_arena_region *region = (_upf_arena_region *) malloc(sizeof(*region));
        if (region == NULL) OUT_OF_MEMORY();
        region->capacity = arena->head->capacity * 2;
        region->data = (uint8_t *) malloc(region->capacity);
        if (region->data == NULL) OUT_OF_MEMORY();
        region->length = 0;
        region->next = NULL;

        arena->head->next = region;
        arena->head = region;
        alignment = 0;
    }

    uint8_t *memory = arena->head->data + arena->head->length + alignment;
    arena->head->length += alignment + size;
    return memory;
}

static void _upf_arena_free(_upf_arena *arena) {
    assert(arena->head != NULL && arena->tail != NULL);

    _upf_arena_region *region = arena->tail;
    while (region != NULL) {
        _upf_arena_region *next = region->next;

        free(region->data);
        free(region);

        region = next;
    }

    arena->head = NULL;
    arena->tail = NULL;
}

// Copies [begin, end) to arena-allocated string
static char *_upf_arena_string(_upf_arena *arena, const char *begin, const char *end) {
    size_t len = end - begin;

    char *string = (char *) _upf_arena_alloc(arena, len + 2);
    memcpy(string, begin, len);
    string[len] = '\0';

    return string;
}


#define INITIAL_VECTOR_CAPACITY 16

#define VECTOR_CSTR_ARENA(a) \
    {                        \
        .arena = (a),        \
    }

#define VECTOR_TYPEDEF(name, type) \
    typedef struct {               \
        _upf_arena *arena;         \
        size_t capacity;           \
        size_t length;             \
        type *data;                \
    } name

#define VECTOR_PUSH(vec, element)                                                             \
    do {                                                                                      \
        if ((vec)->capacity == 0) {                                                           \
            (vec)->capacity = INITIAL_VECTOR_CAPACITY;                                        \
            size_t size = (vec)->capacity * sizeof(*(vec)->data);                             \
            (vec)->data = (vec)->arena ? _upf_arena_alloc((vec)->arena, size) : malloc(size); \
            if ((vec)->data == NULL) OUT_OF_MEMORY();                                         \
        } else if ((vec)->length == (vec)->capacity) {                                        \
            size_t old_size = (vec)->capacity * sizeof(*(vec)->data);                         \
            (vec)->capacity *= 2;                                                             \
            size_t new_size = (vec)->capacity * sizeof(*(vec)->data);                         \
            if ((vec)->arena == NULL) (vec)->data = realloc((vec)->data, new_size);           \
            else {                                                                            \
                void *new_data = _upf_arena_alloc((vec)->arena, new_size);                    \
                memcpy(new_data, (vec)->data, old_size);                                      \
                (vec)->data = new_data;                                                       \
            }                                                                                 \
            if ((vec)->data == NULL) OUT_OF_MEMORY();                                         \
        }                                                                                     \
        (vec)->data[(vec)->length++] = (element);                                             \
    } while (0)

#define VECTOR_FREE(vec)                             \
    do {                                             \
        if ((vec)->arena == NULL) free((vec)->data); \
        (vec)->length = 0;                           \
        (vec)->capacity = 0;                         \
        (vec)->data = NULL;                          \
    } while (0);


#define bool int8_t
#define true 1
#define false 0


typedef struct {
    uint64_t name;
    uint64_t form;
    int64_t implicit_const;
} _upf_attr;

VECTOR_TYPEDEF(_upf_attr_vec, _upf_attr);

typedef struct {
    uint64_t code;
    uint64_t tag;
    bool has_children;
    _upf_attr_vec attrs;
} _upf_abbrev;

VECTOR_TYPEDEF(_upf_abbrev_vec, _upf_abbrev);

typedef struct {
    const uint8_t *subprogram;  // pointer to DIE of parent subprogram
    const uint8_t *info;
    uint64_t call_origin;
} _upf_call_site;

VECTOR_TYPEDEF(_upf_call_site_vec, _upf_call_site);

#define LOC_FBREG 255
#define LOC_ADDR 254

typedef struct {
    uint8_t reg;
    int64_t offset;
    bool is_param;
    bool is_scoped;
    uint64_t from;
    uint64_t to;
} _upf_loc;

VECTOR_TYPEDEF(_upf_loc_vec, _upf_loc);

// TODO: rename, change types
typedef struct {
    _upf_loc_vec locs;
    const uint8_t *type_die;
} _upf_var_entry;

VECTOR_TYPEDEF(_upf_var_vec, _upf_var_entry);

typedef struct {
    bool is_const;
    // TODO: ^ enum?
    union {
        int64_t constant;
        _upf_loc loc;
    } as;
} _upf_param_value;

struct _upf_field;

VECTOR_TYPEDEF(_upf_field_vec, struct _upf_field);

enum _upf_type_kind {
    _UPF_TK_STRUCT,
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
    enum _upf_type_kind kind;
    _upf_field_vec fields;
    bool is_pointer;
    bool is_const;
} _upf_type;

typedef struct _upf_field {
    const char *name;
    size_t type;
    size_t offset;
} _upf_field;

typedef struct {
    const uint8_t *type_die;
    _upf_type type;
} _upf_type_entry;

VECTOR_TYPEDEF(_upf_type_vec, _upf_type_entry);

typedef struct {
    const uint8_t *type_die;
    const char *name;
} _upf_type2;

VECTOR_TYPEDEF(_upf_type2_vec, _upf_type2);

typedef struct {
    const char *file_path;
    const uint8_t *base;
    _upf_abbrev_vec abbrevs;
    _upf_type2_vec types;
    uint64_t str_offsets_base;
    uint64_t addr_base;
    uint64_t low_pc;
    uint64_t high_pc;
} _upf_cu;

VECTOR_TYPEDEF(_upf_cu_vec, _upf_cu);

struct _upf_dwarf {
    void *this_file;

    uint8_t *file;
    off_t file_size;

    bool is64bit;
    uint8_t offset_size;
    uint8_t address_size;

    const uint8_t *info;
    size_t info_size;
    const uint8_t *abbrev;
    const char *str;
    const char *line_str;
    const uint8_t *loclists;
    const uint8_t *loclist_offsets;
    const uint8_t *str_offsets;
    const uint8_t *addr;
};


static _upf_arena _upf_vec_arena = {0};
static struct _upf_dwarf _upf_dwarf = {0};
static _upf_type_vec _upf_type_map = VECTOR_CSTR_ARENA(&_upf_vec_arena);  // TODO: rename
static _upf_cu_vec _upf_cus = VECTOR_CSTR_ARENA(&_upf_vec_arena);         // TODO: rename


// Converts unsigned LEB128 to uint64_t and returns the size of LEB in bytes
static size_t _upf_uLEB_to_uint64(const uint8_t *leb, uint64_t *result) {
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

static uint64_t _upf_get_offset(const uint8_t *info) {
    uint64_t offset = 0;
    memcpy(&offset, info, _upf_dwarf.offset_size);
    return offset;
}

static uint64_t _upf_get_address(const uint8_t *info) {
    uint64_t address = 0;
    memcpy(&address, info, _upf_dwarf.address_size);
    return address;
}

static _upf_abbrev_vec _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _upf_abbrev_vec abbrevs = VECTOR_CSTR_ARENA(&_upf_vec_arena);

    while (1) {
        _upf_abbrev abbrev = {
            .attrs = VECTOR_CSTR_ARENA(&_upf_vec_arena),
        };
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.code);
        if (abbrev.code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_children = *abbrev_table;
        abbrev_table += sizeof(uint8_t);

        while (1) {
            _upf_attr attr = {0};
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.name);
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.form);
            if (attr.form == DW_FORM_implicit_const) abbrev_table += _upf_LEB_to_int64(abbrev_table, &attr.implicit_const);
            if (attr.name == 0 && attr.form == 0) break;
            VECTOR_PUSH(&abbrev.attrs, attr);
        }

        VECTOR_PUSH(&abbrevs, abbrev);
    }

    return abbrevs;
}

static _upf_param_value _upf_eval_dwarf_expr(const _upf_cu *cu, const uint8_t *info) {
    uint64_t len;
    info += _upf_uLEB_to_uint64(info, &len);

    uint8_t opcode = *info++;
    len--;

    _upf_param_value param = {0};
    if (DW_OP_lit0 <= opcode && opcode <= DW_OP_lit31) {
        if (len != 0) goto skip;
        param.is_const = true;
        param.as.constant = opcode - DW_OP_lit0;
    } else if (opcode == DW_OP_const1u) {
        if (len != 1) goto skip;
        param.is_const = true;
        param.as.constant = *info;
    } else if (opcode == DW_OP_const1s) {
        if (len != 1) goto skip;
        param.is_const = true;
        int8_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const2u) {
        if (len != 2) goto skip;
        param.is_const = true;
        uint16_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const2s) {
        if (len != 2) goto skip;
        param.is_const = true;
        int16_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const4u) {
        if (len != 4) goto skip;
        param.is_const = true;
        uint32_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const4s) {
        if (len != 4) goto skip;
        param.is_const = true;
        int32_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const8u) {
        if (len != 8) goto skip;
        param.is_const = true;
        uint64_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_const8s) {
        if (len != 8) goto skip;
        param.is_const = true;
        int64_t temp = 0;
        memcpy(&temp, info, sizeof(temp));
        memcpy(&param.as.constant, &temp, sizeof(temp));
    } else if (opcode == DW_OP_constu) {
        param.is_const = true;
        uint64_t temp;
        size_t leb_size = _upf_uLEB_to_uint64(info, &temp);
        param.as.constant = temp;
        if (len != leb_size) goto skip;
    } else if (opcode == DW_OP_consts) {
        param.is_const = true;
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.constant);
        if (len != leb_size) goto skip;
    } else if (opcode == DW_OP_fbreg) {
        param.as.loc.reg = LOC_FBREG;
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) goto skip;
    } else if (DW_OP_breg0 <= opcode && opcode <= DW_OP_breg31) {
        param.as.loc.reg = opcode - DW_OP_breg0;
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) goto skip;
    } else if (DW_OP_reg0 <= opcode && opcode <= DW_OP_reg31) {
        param.as.loc.reg = opcode - DW_OP_reg0;
        if (len != 0) goto skip;
    } else if (opcode == DW_OP_entry_value) {
        uint64_t subexpr_len;
        size_t leb_len = _upf_uLEB_to_uint64(info, &subexpr_len);

        param = _upf_eval_dwarf_expr(cu, info);
        if (param.is_const == 2) return param;
        assert(!param.is_const);
        param.as.loc.is_param = true;

        if (len - subexpr_len - leb_len == 1) {
            uint8_t next_opcode = *(info + leb_len + subexpr_len);
            if (next_opcode != DW_OP_stack_value) goto skip;
        }
    } else if (opcode == DW_OP_addr) {
        param.as.loc.reg = LOC_ADDR;
        param.as.loc.offset = _upf_get_address(info);
        if (len != _upf_dwarf.address_size) goto skip;
    } else if (opcode == DW_OP_addrx) {
        assert(_upf_dwarf.addr != NULL && cu->addr_base != INVALID);

        uint64_t addr_offset;
        size_t leb_len = _upf_uLEB_to_uint64(info, &addr_offset);

        param.as.loc.reg = LOC_ADDR;
        param.as.loc.offset = _upf_get_address(_upf_dwarf.addr + cu->addr_base + addr_offset * _upf_dwarf.address_size);

        if (len != leb_len) goto skip;
    } else if (opcode == DW_OP_implicit_pointer) {
        fprintf(stderr, "[WARN] skipping DW_OP_implicit_pointer\n");
        param.is_const = 2;
        return param;
    } else if (opcode == DW_OP_piece) {
        fprintf(stderr, "[WARN] skipping DW_OP_piece\n");
        param.is_const = 2;
        return param;
    } else {
        fprintf(stderr, "[WARN] skipping unkown DWARF operation with opcode 0x%02x\n", opcode);
        param.is_const = 2;
        return param;
    }

    return param;
skip:
    fprintf(stderr, "[WARN] skipping dwarf expression: stack is not implemented\n");
    param.is_const = 2;
    return param;
}

static size_t _upf_get_attr_size(const uint8_t *info, uint64_t form) {
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
            return _upf_LEB_to_int64(info, &result);
        } break;
        case DW_FORM_loclistx:
        case DW_FORM_rnglistx:
        case DW_FORM_addrx:
        case DW_FORM_strx:
        case DW_FORM_ref_udata:
        case DW_FORM_udata: {
            uint64_t result;
            return _upf_uLEB_to_uint64(info, &result);
        } break;
        case DW_FORM_string:
            return strlen((const char *) info) + 1;
        case DW_FORM_exprloc:
        case DW_FORM_block: {
            uint64_t length;
            size_t leb_size = _upf_uLEB_to_uint64(info, &length);
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
            size_t offset = _upf_uLEB_to_uint64(info, &form);
            return _upf_get_attr_size(info + offset, form);
        } break;
        case DW_FORM_flag_present:
        case DW_FORM_implicit_const:
            return 0;
        default:
            UNREACHABLE();
    }
}

static uint64_t _upf_get_x_offset(const uint8_t *info, uint64_t form) {
    uint64_t offset = 0;
    switch (form) {
        case DW_FORM_strx1:
        case DW_FORM_addrx1:
            memcpy(&offset, info, 1);
            return offset;
        case DW_FORM_strx2:
        case DW_FORM_addrx2:
            memcpy(&offset, info, 2);
            return offset;
        case DW_FORM_strx3:
        case DW_FORM_addrx3:
            memcpy(&offset, info, 3);
            return offset;
        case DW_FORM_strx4:
        case DW_FORM_addrx4:
            memcpy(&offset, info, 4);
            return offset;
        case DW_FORM_addrx:
        case DW_FORM_strx:
            _upf_uLEB_to_uint64(info, &offset);
            return offset;
        default:
            UNREACHABLE();
    }
}

static const char *_upf_get_string(const _upf_cu *cu, const uint8_t *info, uint64_t form) {
    switch (form) {
        case DW_FORM_strp:
            return _upf_dwarf.str + _upf_get_offset(info);
        case DW_FORM_line_strp:
            assert(_upf_dwarf.line_str != NULL);
            return _upf_dwarf.line_str + _upf_get_offset(info);
        case DW_FORM_string:
            return (const char *) info;
        case DW_FORM_strx:
        case DW_FORM_strx1:
        case DW_FORM_strx2:
        case DW_FORM_strx3:
        case DW_FORM_strx4: {
            assert(_upf_dwarf.str_offsets != NULL && cu->str_offsets_base != INVALID);
            uint64_t offset = cu->str_offsets_base + _upf_get_x_offset(info, form) * _upf_dwarf.offset_size;
            return _upf_dwarf.str + _upf_get_offset(_upf_dwarf.str_offsets + offset);
        }
        default:
            UNREACHABLE();
    }
}

static uint64_t _upf_get_ref(const uint8_t *info, uint64_t form) {
    uint64_t ref = 0;
    switch (form) {
        case DW_FORM_ref1:
        case DW_FORM_ref2:
        case DW_FORM_ref4:
        case DW_FORM_ref8:
            memcpy(&ref, info, _upf_get_attr_size(info, form));
            break;
        case DW_FORM_ref_udata:
            _upf_uLEB_to_uint64(info, &ref);
            break;
        default:
            assert(0 && "TODO: handle other ref types");  // TODO:
    }
    return ref;
}

static int64_t _upf_get_data(const uint8_t *info, _upf_attr attr) {
    switch (attr.form) {
        case DW_FORM_data1:
        case DW_FORM_data2:
        case DW_FORM_data4:
        case DW_FORM_data8: {
            int64_t data = 0;
            memcpy(&data, info, _upf_get_attr_size(info, attr.form));
            return data;
        } break;
        case DW_FORM_implicit_const:
            return attr.implicit_const;
        default:
            assert(0 && "TODO: handle other data types");  // TODO:
    }
}

static uint64_t _upf_get_addr(const _upf_cu *cu, const uint8_t *info, uint64_t form) {
    switch (form) {
        case DW_FORM_addr:
            return _upf_get_address(info);
        case DW_FORM_addrx: {
            assert(_upf_dwarf.addr != NULL && cu->addr_base != INVALID);
            uint64_t offset = cu->addr_base + _upf_get_x_offset(info, form) * _upf_dwarf.address_size;
            return _upf_get_address(_upf_dwarf.addr + offset);
        }
        default:
            assert(0 && "TODO: handle other address types");  // TODO:
    }
}

static const uint8_t *_upf_skip_die(const uint8_t *info, const _upf_abbrev *abbrev) {
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        info += _upf_get_attr_size(info, abbrev->attrs.data[i].form);
    }

    return info;
}

static enum _upf_type_kind _upf_get_type_kind(int64_t encoding, int64_t size) {
    switch (encoding) {
        case DW_ATE_boolean:
            if (size == 1) return _UPF_TK_BOOL;
            fprintf(stderr, "[WARN] uprintf: expected boolean to be 1 byte long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_address:
            fprintf(stderr,
                    "[WARN] uprintf: segmented addresses aren't supported since x86_64/amd64 (the only supported architecture) doesn't "
                    "have them. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed:
            if (size == 1) return _UPF_TK_S1;
            if (size == 2) return _UPF_TK_S2;
            if (size == 4) return _UPF_TK_S4;
            if (size == 8) return _UPF_TK_S8;
            if (size == 16) {
                uint32_t x = 1;
                assert(*((uint8_t *) (&x)) == 1 && "uprintf only support x86_64/amd64 which is little-endian");

                fprintf(stderr, "[WARN] uprintf: 16 byte integers aren't supported. Truncating to 8 bytes.\n");
                return _UPF_TK_S8;
            }
            fprintf(stderr, "[WARN] uprintf: expected signed integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed_char:
            if (size == 1) return _UPF_TK_SCHAR;
            fprintf(stderr, "[WARN] uprintf: expected char to be 1 byte long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_unsigned:
            if (size == 1) return _UPF_TK_U1;
            if (size == 2) return _UPF_TK_U2;
            if (size == 4) return _UPF_TK_U4;
            if (size == 8) return _UPF_TK_U8;
            if (size == 16) {
                uint32_t x = 1;
                assert(*((uint8_t *) (&x)) == 1 && "uprintf only support x86_64/amd64 which is little-endian");

                fprintf(stderr, "[WARN] uprintf: 16 byte unsigned integers aren't supported. Truncating to 8 bytes.\n");
                return _UPF_TK_U8;
            }
            fprintf(stderr, "[WARN] uprintf: expected unsigned integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_unsigned_char:
            if (size == 1) return _UPF_TK_UCHAR;
            fprintf(stderr, "[WARN] uprintf: expected char to be 1 byte long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_ASCII:
        case DW_ATE_UCS:
        case DW_ATE_UTF:
            fprintf(stderr, "[WARN] uprintf: C shouldn't have character encodings besides DW_ATE_(un)signed_char. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed_fixed:
        case DW_ATE_unsigned_fixed:
            fprintf(stderr, "[WARN] uprintf: C shouldn't have fixed-point decimals. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_float:
            if (size == 4) return _UPF_TK_F4;
            if (size == 8) return _UPF_TK_F8;
            if (size == 16) {
                fprintf(stderr, "[WARN] uprintf: 16 byte floats aren't supported. Ignoring this type.\n");
                return _UPF_TK_UNKNOWN;
            }
            fprintf(stderr, "[WARN] uprintf: expected floats to be 4, 8 or 16 bytes long. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_complex_float:
            // TODO:
            fprintf(stderr, "[WARN] uprintf: complex floats aren't supported. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_imaginary_float:
            fprintf(stderr, "[WARN] uprintf: imaginary floats aren't supported. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_decimal_float:
            // TODO:
            fprintf(stderr, "[WARN] uprintf: decimal floats aren't supported. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_packed_decimal:
            fprintf(stderr, "[WARN] uprintf: C shouldn't have packed decimals. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_numeric_string:
            fprintf(stderr, "[WARN] uprintf: C shouldn't have numeric strings. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_edited:
            fprintf(stderr, "[WARN] uprintf: C shouldn't have edited strings. Ignoring this type.\n");
            return _UPF_TK_UNKNOWN;
        default:
            fprintf(stderr, "[WARN] uprintf: skipping unkown DWARF type encoding (0x%02lx)\n", encoding);
            return _UPF_TK_UNKNOWN;
    }
}

static size_t _upf_get_type(const _upf_cu *cu, const uint8_t *info) {
    // TODO: try to reduce size, esp of const/pointer/typedef, since they don't change much
    for (size_t i = 0; i < _upf_type_map.length; i++) {
        if (_upf_type_map.data[i].type_die == info) {
            return i;
        }
    }

    const uint8_t *base = info;

    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= cu->abbrevs.length);
    const _upf_abbrev *abbrev = &cu->abbrevs.data[code - 1];

    switch (abbrev->tag) {
        case DW_TAG_typedef: {
            const char *name = NULL;
            const uint8_t *next_info = NULL;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_type) next_info = cu->base + _upf_get_ref(info, attr.form);
                else if (attr.name == DW_AT_name) name = _upf_get_string(cu, info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(next_info != NULL && name != NULL);

            size_t type_idx = _upf_get_type(cu, next_info);
            _upf_type_entry entry = _upf_type_map.data[type_idx];
            entry.type.name = name;
            entry.type_die = base;

            VECTOR_PUSH(&_upf_type_map, entry);

            return _upf_type_map.length - 1;
        }
        case DW_TAG_union_type:
            assert(0 && "TODO: unions");  // TODO:
        case DW_TAG_structure_type: {
            _upf_type type = {
                .name = NULL,
                .kind = _UPF_TK_STRUCT,
                .fields = VECTOR_CSTR_ARENA(&_upf_vec_arena),
                .is_pointer = false,
                .is_const = false,
            };

            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) type.name = _upf_get_string(cu, info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }

            while (1) {
                info += _upf_uLEB_to_uint64(info, &code);
                if (code == 0) break;

                assert(code <= cu->abbrevs.length);
                abbrev = &cu->abbrevs.data[code - 1];
                assert(abbrev->tag == DW_TAG_member);

                _upf_field field = {
                    .name = NULL,
                    .type = INVALID,
                    .offset = INVALID,
                };
                for (size_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];

                    if (attr.name == DW_AT_name) {
                        field.name = _upf_get_string(cu, info, attr.form);
                    } else if (attr.name == DW_AT_type) {
                        const uint8_t *type_die = cu->base + _upf_get_ref(info, attr.form);
                        field.type = _upf_get_type(cu, type_die);
                    } else if (attr.name == DW_AT_data_member_location) {
                        field.offset = _upf_get_data(info, attr);
                    }

                    info += _upf_get_attr_size(info, attr.form);
                }
                assert(field.name != NULL && field.type != INVALID && field.offset != INVALID);

                VECTOR_PUSH(&type.fields, field);
            }

            _upf_type_entry entry = {
                .type_die = base,
                .type = type,
            };
            VECTOR_PUSH(&_upf_type_map, entry);

            return _upf_type_map.length - 1;
        }
        case DW_TAG_base_type: {
            _upf_type type = {
                .name = NULL,
                .kind = _UPF_TK_UNKNOWN,
                .fields = {0},
                .is_pointer = false,
                .is_const = false,
            };
            uint64_t size = INVALID;
            uint64_t encoding = INVALID;
            // TODO: rename _OFFSET
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_byte_size) size = _upf_get_data(info, attr);  // TODO: may be exprloc or ref
                else if (attr.name == DW_AT_encoding) encoding = _upf_get_data(info, attr);
                else if (attr.name == DW_AT_name) type.name = _upf_get_string(cu, info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(size != INVALID && encoding != INVALID && type.name != NULL);

            type.kind = _upf_get_type_kind(encoding, size);

            // TODO: don't add base types to the map?
            // TODO: use real hash map?
            _upf_type_entry entry = {
                .type_die = base,
                .type = type,
            };
            VECTOR_PUSH(&_upf_type_map, entry);

            return _upf_type_map.length - 1;
        }
        case DW_TAG_pointer_type: {
            uint64_t offset = INVALID;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_type) offset = _upf_get_ref(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }

            if (offset == INVALID) {
                // void*'s pointer DIEs do not contain DW_AT_type, hence offset is invalid
                _upf_type_entry entry = {
                    .type_die = base,
                    .type = {
                        .name = "void",
                        .kind = _UPF_TK_VOID,
                        .fields = {0},
                        .is_pointer = true,
                        .is_const = false,
                    },
                };
                VECTOR_PUSH(&_upf_type_map, entry);

                return _upf_type_map.length - 1;
            }

            size_t type_idx = _upf_get_type(cu, cu->base + offset);
            _upf_type_entry entry = _upf_type_map.data[type_idx];
            entry.type_die = base;
            entry.type.is_pointer = true;
            VECTOR_PUSH(&_upf_type_map, entry);

            return _upf_type_map.length - 1;
        }
        case DW_TAG_const_type: {
            // TODO: display const?
            uint64_t offset = INVALID;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_type) offset = _upf_get_ref(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(offset != INVALID);

            size_t type_idx = _upf_get_type(cu, cu->base + offset);
            _upf_type_entry entry = _upf_type_map.data[type_idx];
            entry.type_die = base;
            entry.type.is_const = true;
            VECTOR_PUSH(&_upf_type_map, entry);

            return _upf_type_map.length - 1;
        }
        default:
            UNREACHABLE();
    }
}

static bool _upf_parse_loclist(const _upf_cu *cu, const uint8_t *loclist, _upf_loc_vec *locs) {
    uint64_t base = cu->low_pc == INVALID ? 0 : cu->low_pc;
    bool result = true;
    while (*loclist != DW_LLE_end_of_list) {
        switch (*loclist++) {
            case DW_LLE_base_address:
                base = _upf_get_address(loclist);
                loclist += _upf_dwarf.address_size;
                break;
            case DW_LLE_offset_pair: {
                uint64_t from, to;
                loclist += _upf_uLEB_to_uint64(loclist, &from);
                loclist += _upf_uLEB_to_uint64(loclist, &to);

                _upf_param_value param = _upf_eval_dwarf_expr(cu, loclist);
                if (param.is_const != 2) {
                    assert(!param.is_const);

                    param.as.loc.is_scoped = true;
                    param.as.loc.from = base + from;
                    param.as.loc.to = base + to;

                    VECTOR_PUSH(locs, param.as.loc);
                } else {
                    result = false;
                }

                // TODO: move this inside _upf_eval_dwarf_expr?
                uint64_t expr_len;
                loclist += _upf_uLEB_to_uint64(loclist, &expr_len);
                loclist += expr_len;
            } break;
            case DW_LLE_start_length: {
                uint64_t from = _upf_get_address(loclist);
                loclist += _upf_dwarf.address_size;

                uint64_t len;
                loclist += _upf_uLEB_to_uint64(loclist, &len);
                uint64_t to = from + len;

                _upf_param_value param = _upf_eval_dwarf_expr(cu, loclist);
                if (param.is_const != 2) {
                    assert(!param.is_const);

                    param.as.loc.is_scoped = true;
                    param.as.loc.from = from;
                    param.as.loc.to = to;

                    VECTOR_PUSH(locs, param.as.loc);
                } else {
                    result = false;
                }

                // TODO: move this inside _upf_eval_dwarf_expr?
                uint64_t expr_len;
                loclist += _upf_uLEB_to_uint64(loclist, &expr_len);
                loclist += expr_len;
            } break;
            default:
                assert(0 && "TODO: handle other loclist types");  // TODO:
        }
    }
    return result;
}

// TODO: is there any point in returning bool, or partial result + handle them?
static bool _upf_get_locations(const _upf_cu *cu, const uint8_t *info, uint64_t form, _upf_loc_vec *locs) {
    assert(locs->length == 0);
    switch (form) {
        case DW_FORM_exprloc: {
            _upf_param_value param = _upf_eval_dwarf_expr(cu, info);
            if (param.is_const == 2) return false;
            assert(!param.is_const);
            VECTOR_PUSH(locs, param.as.loc);
            return true;
        }
        case DW_FORM_sec_offset: {
            assert(_upf_dwarf.loclists != NULL);
            const uint8_t *loclist = _upf_dwarf.loclists + _upf_get_offset(info);
            return _upf_parse_loclist(cu, loclist, locs);
        }
        case DW_FORM_loclistx: {
            assert(_upf_dwarf.loclist_offsets != NULL);

            uint64_t index;
            _upf_uLEB_to_uint64(info, &index);

            uint64_t loclist_offset = _upf_get_offset(_upf_dwarf.loclist_offsets + index * _upf_dwarf.offset_size);
            const uint8_t *loclist = _upf_dwarf.loclist_offsets + loclist_offset;

            return _upf_parse_loclist(cu, loclist, locs);
        }
        default:
            UNREACHABLE();
    }
}

// TODO: rename to include parameter?
static _upf_var_entry _upf_parse_variable(const _upf_cu *cu, const uint8_t *info, const _upf_abbrev *abbrev) {
    _upf_var_entry var = {
        .locs = VECTOR_CSTR_ARENA(&_upf_vec_arena),
    };

    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_location) {
            if (!_upf_get_locations(cu, info, attr.form, &var.locs)) return var;  // failure
        } else if (attr.name == DW_AT_type) {
            var.type_die = cu->base + _upf_get_ref(info, attr.form);
        } else if (attr.name == DW_AT_abstract_origin) {
            const uint8_t *new_info = cu->base + _upf_get_ref(info, attr.form);

            uint64_t code;
            new_info += _upf_uLEB_to_uint64(new_info, &code);
            assert(code > 0 && code <= cu->abbrevs.length);
            _upf_var_entry new_var = _upf_parse_variable(cu, new_info, &cu->abbrevs.data[code - 1]);
            var.type_die = new_var.type_die;
        }

        info += _upf_get_attr_size(info, attr.form);
    }

    return var;
}

static bool _upf_is_form_addr(uint64_t form) {
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

static const char *_upf_get_type_name(const _upf_cu *cu, const uint8_t *info) {
    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);

    assert(code > 0 && code <= cu->abbrevs.length);
    const _upf_abbrev *abbrev = &cu->abbrevs.data[code - 1];
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type) {
            if (attr.name == DW_AT_type) return _upf_get_type_name(cu, cu->base + _upf_get_ref(info, attr.form));
        } else if (abbrev->tag == DW_TAG_structure_type || abbrev->tag == DW_TAG_typedef || abbrev->tag == DW_TAG_base_type) {
            if (attr.name == DW_AT_name) return _upf_get_string(cu, info, attr.form);
        }

        info += _upf_get_attr_size(info, attr.form);
    }
    if (abbrev->tag == DW_TAG_pointer_type) return "void";  // TODO: void*

    return NULL;
}

// TODO: handle all of these (check whether they can be generated in C with gcc/clang):
// #define DW_TAG_array_type               0x01
// #define DW_TAG_class_type               0x02
// #define DW_TAG_enumeration_type         0x04
// #define DW_TAG_pointer_type             0x0f
// #define DW_TAG_reference_type           0x10
// #define DW_TAG_string_type              0x12
// #define DW_TAG_structure_type           0x13
// #define DW_TAG_subroutine_type          0x15
// #define DW_TAG_typedef                  0x16
// #define DW_TAG_union_type               0x17
// #define DW_TAG_ptr_to_member_type       0x1f
// #define DW_TAG_set_type                 0x20
// #define DW_TAG_subrange_type            0x21
// #define DW_TAG_base_type                0x24
// #define DW_TAG_const_type               0x26
// #define DW_TAG_file_type                0x29
// #define DW_TAG_packed_type              0x2d
// #define DW_TAG_volatile_type            0x35
// #define DW_TAG_restrict_type            0x37  /* DWARF3 */
// #define DW_TAG_interface_type           0x38  /* DWARF3 */
// #define DW_TAG_unspecified_type         0x3b  /* DWARF3 */
// #define DW_TAG_shared_type              0x40  /* DWARF3f */
// #define DW_TAG_type_unit                0x41  /* DWARF4 */
// #define DW_TAG_coarray_type             0x44  /* DWARF5 */
// #define DW_TAG_dynamic_type             0x46  /* DWARF5 */
// #define DW_TAG_atomic_type              0x47  /* DWARF5 */
// #define DW_TAG_immutable_type           0x4b  /* DWARF5 */

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *info, const uint8_t *info_end, const uint8_t *abbrev_table) {
    static const char *UPRINTF_FUNCTION_NAME = "_upf_uprintf";

    _upf_cu cu = {
        .file_path = NULL,
        .base = cu_base,
        .abbrevs = _upf_parse_abbrevs(abbrev_table),
        .types = VECTOR_CSTR_ARENA(&_upf_vec_arena),
        .str_offsets_base = INVALID,
        .addr_base = 0,
        .low_pc = INVALID,
        .high_pc = INVALID,
    };

    uint64_t uprintf_offset = INVALID;
    _upf_call_site_vec call_sites = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    _upf_var_vec vars = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    const uint8_t *subprogram = NULL;
    int depth = 0;
    while (info < info_end) {
        const uint8_t *die_base = info;

        uint64_t code;
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) {
            depth--;
            continue;
        }

        assert(code <= cu.abbrevs.length);
        const _upf_abbrev *abbrev = &cu.abbrevs.data[code - 1];
        if (abbrev->has_children) depth++;

        if (abbrev->tag == DW_TAG_subprogram) subprogram = die_base;

        if (abbrev->tag == DW_TAG_compile_unit) {
            // save to parse after the loop because they require str_offsets and addr bases
            const uint8_t *name_info = NULL;
            uint64_t name_form = INVALID;
            const uint8_t *low_pc_info = NULL;
            uint64_t low_pc_form = INVALID;
            const uint8_t *high_pc_info = NULL;
            _upf_attr high_pc_attr;

            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    name_info = info;
                    name_form = attr.form;
                } else if (attr.name == DW_AT_str_offsets_base) {
                    cu.str_offsets_base = _upf_get_offset(info);
                } else if (attr.name == DW_AT_addr_base) {
                    cu.addr_base = _upf_get_offset(info);
                } else if (attr.name == DW_AT_low_pc) {
                    low_pc_info = info;
                    low_pc_form = attr.form;
                } else if (attr.name == DW_AT_high_pc) {
                    high_pc_info = info;
                    high_pc_attr = attr;
                }

                info += _upf_get_attr_size(info, attr.form);
            }

            assert(name_info != NULL && name_form != INVALID);
            cu.file_path = _upf_get_string(&cu, name_info, name_form);

            if (low_pc_info != NULL && low_pc_form != INVALID) cu.low_pc = _upf_get_addr(&cu, low_pc_info, low_pc_form);
            if (high_pc_info != NULL && high_pc_attr.form != INVALID) {
                if (_upf_is_form_addr(high_pc_attr.form)) {
                    cu.high_pc = _upf_get_addr(&cu, high_pc_info, high_pc_attr.form);
                } else {
                    assert(cu.low_pc != INVALID);
                    cu.high_pc = cu.low_pc + _upf_get_data(high_pc_info, high_pc_attr);
                }
            }
        } else if (abbrev->tag == DW_TAG_subprogram && uprintf_offset == INVALID) {
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    const char *function_name = _upf_get_string(&cu, info, attr.form);
                    if (strcmp(function_name, UPRINTF_FUNCTION_NAME) == 0) uprintf_offset = die_base - cu.base;
                }

                info += _upf_get_attr_size(info, attr.form);
            }
        } else if (abbrev->tag == DW_TAG_call_site) {
            uint64_t call_origin = INVALID;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_call_origin) call_origin = _upf_get_ref(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(call_origin != INVALID);

            _upf_call_site call_site = {
                .subprogram = subprogram,
                .info = die_base,
                .call_origin = call_origin,
            };
            VECTOR_PUSH(&call_sites, call_site);
        } else if (abbrev->tag == DW_TAG_variable && depth == 1) {
            _upf_var_entry var = _upf_parse_variable(&cu, info, abbrev);
            if (var.locs.length > 0) VECTOR_PUSH(&vars, var);
            info = _upf_skip_die(info, abbrev);
        } else if (abbrev->tag == DW_TAG_typedef || abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type
                   || abbrev->tag == DW_TAG_structure_type || abbrev->tag == DW_TAG_base_type) {
            const char *name = _upf_get_type_name(&cu, die_base);
            if (name != NULL) {
                _upf_type2 type = {
                    .type_die = die_base,
                    .name = name,
                };
                VECTOR_PUSH(&cu.types, type);
            }
            info = _upf_skip_die(info, abbrev);
        } else {
            info = _upf_skip_die(info, abbrev);
        }
    }
    assert(cu.file_path != NULL && "compilation unit must have a path.");
    VECTOR_PUSH(&_upf_cus, cu);

    if (uprintf_offset == INVALID) return;

    for (size_t i = 0; i < call_sites.length; i++) {
        _upf_call_site *call_site = &call_sites.data[i];
        if (call_site->call_origin == uprintf_offset) {
            size_t vars_length = vars.length;
            // _upf_parse_uprintf_call_site(cu, &uprintf.params, call_site, &vars);
            vars.length = vars_length;  // clear variables local to parsed subprogram
        }
    }
}

static void _upf_parse_dwarf(void) {
    const uint8_t *info = _upf_dwarf.info;
    const uint8_t *info_end = info + _upf_dwarf.info_size;
    while (info < info_end) {
        const uint8_t *cu_base = info;

        uint64_t length = 0;
        memcpy(&length, info, sizeof(uint32_t));
        info += sizeof(uint32_t);

        _upf_dwarf.is64bit = false;
        if (length == 0xffffffffU) {
            memcpy(&length, info, sizeof(uint64_t));
            info += sizeof(uint64_t);
            _upf_dwarf.is64bit = true;
        }

        _upf_dwarf.offset_size = _upf_dwarf.is64bit ? 8 : 4;

        const uint8_t *next = info + length;

        uint16_t version = 0;
        memcpy(&version, info, sizeof(version));
        info += sizeof(version);
        if (version != 5) ERROR("uprintf only supports DWARF version 5.\n");

        uint8_t type = *info;
        info += sizeof(type);
        assert(type == DW_UT_compile);

        uint8_t address_size = *info;
        assert(_upf_dwarf.address_size == 0 || _upf_dwarf.address_size == address_size);
        _upf_dwarf.address_size = address_size;
        info += sizeof(address_size);

        uint64_t abbrev_offset = _upf_get_offset(info);
        info += _upf_dwarf.offset_size;

        _upf_parse_cu(cu_base, info, next, _upf_dwarf.abbrev + abbrev_offset);

        info = next;
    }

    for (size_t i = 0; i < _upf_cus.length; i++) {
        _upf_cu *cu = &_upf_cus.data[i];
        if (cu->low_pc == INVALID) {
            if (i == 0) cu->low_pc = 0;
            else cu->low_pc = _upf_cus.data[i - 1].high_pc + 1;
        }

        if (cu->high_pc == INVALID) {
            if (i + 1 == _upf_cus.length) cu->high_pc = UINT64_MAX;
            else {
                uint64_t next_low_pc = _upf_cus.data[i + 1].low_pc;
                assert(next_low_pc != INVALID);
                cu->high_pc = next_low_pc - 1;
            }
        }
    }
}

static void _upf_parse_elf(void) {
    struct stat file_info;
    if (stat("/proc/self/exe", &file_info) == -1) ERROR("Unable to stat \"/proc/self/exe\": %s.\n", strerror(errno));
    size_t size = file_info.st_size;
    _upf_dwarf.file_size = size;

    int fd = open("/proc/self/exe", O_RDONLY);
    assert(fd != -1);

    uint8_t *file = (uint8_t *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    assert(file != MAP_FAILED);
    _upf_dwarf.file = file;

    close(fd);

    const Elf64_Ehdr *header = (Elf64_Ehdr *) file;

    assert(memcmp(header->e_ident, ELFMAG, SELFMAG) == 0);
    assert(header->e_ident[EI_CLASS] == ELFCLASS64);
    assert(header->e_ident[EI_VERSION] == 1);
    assert(header->e_machine == EM_X86_64);
    assert(header->e_version == 1);
    assert(header->e_shentsize == sizeof(Elf64_Shdr));

    const Elf64_Shdr *string_section = (Elf64_Shdr *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);

    const Elf64_Shdr *section = (Elf64_Shdr *) (file + header->e_shoff);
    for (size_t i = 0; i < header->e_shnum; i++) {
        const char *name = string_table + section->sh_name;

        if (strcmp(name, ".debug_info") == 0) {
            _upf_dwarf.info = file + section->sh_offset;
            _upf_dwarf.info_size = section->sh_size;
        } else if (strcmp(name, ".debug_abbrev") == 0) {
            _upf_dwarf.abbrev = file + section->sh_offset;
        } else if (strcmp(name, ".debug_str") == 0) {
            _upf_dwarf.str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_line_str") == 0) {
            _upf_dwarf.line_str = (const char *) (file + section->sh_offset);
        } else if (strcmp(name, ".debug_loclists") == 0) {
            const uint8_t *temp = file + section->sh_offset;
            _upf_dwarf.loclists = temp;

            uint32_t dwarf64_length = 0xffffffffU;
            if (memcmp(temp, &dwarf64_length, sizeof(dwarf64_length)) == 0) temp += sizeof(uint64_t);
            temp += sizeof(uint32_t);

            uint16_t version = 0;
            memcpy(&version, temp, sizeof(version));
            assert(version == 5);
            temp += sizeof(uint16_t);

            uint8_t address_size = *temp;
            assert(_upf_dwarf.address_size == 0 || _upf_dwarf.address_size == address_size);
            _upf_dwarf.address_size = address_size;
            temp += sizeof(uint8_t);

            uint8_t segment_selector_size = *temp;
            assert(segment_selector_size == 0
                   && "Segmented addresses aren't supported since x86_64/amd64 (the only supported architecture) doesn't have them");
            temp += sizeof(uint8_t);

            uint32_t offset_count = 0;
            memcpy(&offset_count, temp, sizeof(offset_count));
            temp += sizeof(uint32_t);

            if (offset_count > 0) _upf_dwarf.loclist_offsets = temp;
        } else if (strcmp(name, ".debug_str_offsets") == 0) {
            const uint8_t *temp = file + section->sh_offset;
            _upf_dwarf.str_offsets = temp;

            uint32_t dwarf64_length = 0xffffffffU;
            if (memcmp(temp, &dwarf64_length, sizeof(dwarf64_length)) == 0) temp += sizeof(uint64_t);
            temp += sizeof(uint32_t);

            uint16_t version = 0;
            memcpy(&version, temp, sizeof(version));
            assert(version == 5);
            temp += sizeof(uint16_t);

            uint16_t reserved = 0;
            memcpy(&reserved, temp, sizeof(reserved));
            assert(reserved == 0);
            temp += sizeof(uint16_t);
        } else if (strcmp(name, ".debug_addr") == 0) {
            const uint8_t *temp = file + section->sh_offset;
            _upf_dwarf.addr = temp;

            uint32_t dwarf64_length = 0xffffffffU;
            if (memcmp(temp, &dwarf64_length, sizeof(dwarf64_length)) == 0) temp += sizeof(uint64_t);
            temp += sizeof(uint32_t);

            uint16_t version = 0;
            memcpy(&version, temp, sizeof(version));
            assert(version == 5);
            temp += sizeof(uint16_t);

            uint8_t address_size = *temp;
            assert(_upf_dwarf.address_size == 0 || _upf_dwarf.address_size == address_size);
            _upf_dwarf.address_size = address_size;
            temp += sizeof(uint8_t);

            uint8_t segment_selector_size = *temp;
            assert(segment_selector_size == 0
                   && "Segmented addresses aren't supported since x86_64/amd64 (the only supported architecture) doesn't have them");
            temp += sizeof(uint8_t);
        }

        section++;
    }
    assert(_upf_dwarf.info != NULL && _upf_dwarf.abbrev != NULL && _upf_dwarf.str != NULL);
}

static const char *_upf_escape_char(char ch) {
    switch (ch) {
        case '\a':
            return "\\a";
        case '\b':
            return "\\b";
        case '\f':
            return "\\f";
        case '\n':
            return "\\n";
        case '\r':
            return "\\r";
        case '\t':
            return "\\t";
        case '\v':
            return "\\v";
        case '\\':
            return "\\\\";
        default: {
            static char str[2] = {0};
            str[0] = ch;
            return str;
        }
    }
}

static bool _upf_is_printable(char c) { return ' ' <= c && c <= '~'; }

static char *_upf_print_type(char *p, const uint8_t *data, const _upf_type *type, int depth) {
    // TODO: if circular or self-referential, numerate fields/structs (only referred-to?) to
    // TODO: make them pointer to each other (or just include addresses?)
    // TODO: limit depth
    // TODO: add option to align by equals
    // TODO: specify floating point precision
    // TODO: snprintf

    if (type->is_pointer) {
        // TODO: check that machine address size == DWARF.type.byte_size
        void *ptr = *((void **) data);
        if (ptr == NULL) {
            p += sprintf(p, "NULL");
            return p;
        }

        switch (type->kind) {
            case _UPF_TK_STRUCT:
                // TODO: check that the pointer is not to already seen struct
                break;
            case _UPF_TK_SCHAR: {
                // TODO: what if it is a pointer to a char (or even int8_t) instead of c-style string?
                // TODO: it might not have a \0
                char *str = *((char **) data);
                p += sprintf(p, "%p (\"", (void *) str);
                while (*str != '\0') p += sprintf(p, "%s", _upf_escape_char(*str++));
                p += sprintf(p, "\")");
                return p;
            }
            case _UPF_TK_VOID:
                p += sprintf(p, "%p", *((void **) data));
                return p;
            default: {
                // TODO: print address and value in parentheses
                void *ptr = *((void **) data);
                p += sprintf(p, "%p (", ptr);
                data = ptr;
            } break;
        }
    }

    switch (type->kind) {
        case _UPF_TK_STRUCT:
            p += sprintf(p, "%s {\n", type->name);
            for (size_t i = 0; i < type->fields.length; i++) {
                _upf_field *field = &type->fields.data[i];
                _upf_type type = _upf_type_map.data[field->type].type;

                int indent = UPRINTF_INDENT * (depth + 1);
                memset(p, ' ', indent);
                p += indent;

                p += sprintf(p, "%s%s %s%s = ", type.is_const ? "const " : "", type.name, type.is_pointer ? "*" : "", field->name);
                p = _upf_print_type(p, data + field->offset, &type, depth + 1);
                *p++ = '\n';
            }
            p += sprintf(p, "};");
            break;
        case _UPF_TK_U1:
            p += sprintf(p, "%hhu", *data);
            break;
        case _UPF_TK_U2:
            p += sprintf(p, "%hu", *((uint16_t *) data));
            break;
        case _UPF_TK_U4:
            p += sprintf(p, "%u", *((uint32_t *) data));
            break;
        case _UPF_TK_U8:
            p += sprintf(p, "%lu", *((uint64_t *) data));
            break;
        case _UPF_TK_S1:
            p += sprintf(p, "%hhd", *((int8_t *) data));
            break;
        case _UPF_TK_S2:
            p += sprintf(p, "%hd", *((int16_t *) data));
            break;
        case _UPF_TK_S4:
            p += sprintf(p, "%d", *((int32_t *) data));
            break;
        case _UPF_TK_S8:
            p += sprintf(p, "%ld", *((int64_t *) data));
            break;
        case _UPF_TK_F4:
            p += sprintf(p, "%f", *((float *) data));
            break;
        case _UPF_TK_F8:
            p += sprintf(p, "%lf", *((double *) data));
            break;
        case _UPF_TK_UCHAR: {
            char ch = *((char *) data);
            p += sprintf(p, "%hhu", ch);
            if (_upf_is_printable(ch)) p += sprintf(p, " ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_SCHAR: {
            char ch = *((char *) data);
            p += sprintf(p, "%hhd", ch);
            if (_upf_is_printable(ch)) p += sprintf(p, " ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_VOID:
            ERROR("`void` type isn't valid if it isn't a pointer.\n");
        case _UPF_TK_UNKNOWN:
            p += sprintf(p, "(unkown)");
            break;
        default:
            UNREACHABLE();
    }

    if (type->is_pointer) *p++ = ')';

    return p;
}

// TODO: assert everywhere where address_size is set?
// Function returns the address to which this executable is mapped to.
// It is retrieved by reading `/proc/self/maps` (see `man proc_pid_maps`).
// It is used to convert between DWARF addresses and runtime/real ones: DWARF
// addresses are relative to the beginning of the file, thus real = base + dwarf.
static void *_upf_get_this_file_address(void) {
    static const size_t PATH_BUFFER_SIZE = 1024;

    char this_path[PATH_BUFFER_SIZE];
    ssize_t read = readlink("/proc/self/exe", this_path, PATH_BUFFER_SIZE);
    if (read == -1) ERROR("Unable to readlink \"/proc/self/exe\": %s.\n", strerror(errno));
    if (read == PATH_BUFFER_SIZE) ERROR("Unable to readlink \"/proc/self/exe\": path is too long.\n");
    this_path[read] = '\0';

    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) ERROR("Unable to open \"/proc/self/maps\": %s.\n", strerror(errno));

    uint64_t address = INVALID;
    size_t length = 0;
    char *line = NULL;
    while ((read = getline(&line, &length, file)) != -1) {
        if (read == 0) continue;
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        int path_offset;
        if (sscanf(line, "%lx-%*x %*s %*x %*x:%*x %*u %n", &address, &path_offset) != 1)
            ERROR("Unable to parse \"/proc/self/maps\": invalid format.\n");

        if (strcmp(this_path, line + path_offset) == 0) break;
    }
    if (line) free(line);
    fclose(file);

    assert(address != INVALID);
    return (void *) address;
}


__attribute__((constructor)) void _upf_init(void) {
    if (access("/proc/self/exe", R_OK) != 0) ERROR("uprintf only supports Linux: expected \"/proc/self/exe\" to be a valid path.\n");
    if (access("/proc/self/maps", R_OK) != 0) ERROR("uprintf only supports Linux: expected \"/proc/self/maps\" to be a valid path.\n");

    _upf_arena_init(&_upf_vec_arena);

    _upf_dwarf.this_file = _upf_get_this_file_address();

    _upf_parse_elf();
    _upf_parse_dwarf();
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    munmap(_upf_dwarf.file, _upf_dwarf.file_size);
    _upf_arena_free(&_upf_vec_arena);
}

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
    _UPF_TOK_PTR_MEMBER,
    _UPF_TOK_NUMBER,
    _UPF_TOK_ID,
    _UPF_TOK_STRING,
    _UPF_TOK_KEYWORD
};

typedef struct {
    enum _upf_token_kind kind;
    union {
        const char *string;
        long num;
    } as;
} _upf_token;

VECTOR_TYPEDEF(_upf_token_vec, _upf_token);

typedef struct {
    _upf_token_vec tokens;
    size_t idx;
} _upf_tokenizer;

static _upf_tokenizer _upf_tokenize(const char *string) {
    static const _upf_token signs[] = {{_UPF_TOK_OPEN_PAREN, {"("}},    {_UPF_TOK_CLOSE_PAREN, {")"}}, {_UPF_TOK_OPEN_BRACKET, {"["}},
                                       {_UPF_TOK_CLOSE_BRACKET, {"]"}}, {_UPF_TOK_STAR, {"*"}},        {_UPF_TOK_AMPERSAND, {"&"}},
                                       {_UPF_TOK_COMMA, {","}},         {_UPF_TOK_DOT, {"."}},         {_UPF_TOK_PTR_MEMBER, {"->"}}};
    static const char *keywords[] = {"struct", "union", "enum"};

    _upf_token_vec tokens = VECTOR_CSTR_ARENA(&_upf_vec_arena);

    const char *ch = string;
    while (*ch != '\0') {
        if (*ch == ' ') {
            ch++;
            continue;
        }

        bool found = false;
        for (size_t i = 0; i < sizeof(signs) / sizeof(*signs) && !found; i++) {
            size_t len = strlen(signs[i].as.string);
            if (strncmp(ch, signs[i].as.string, len) == 0) {
                VECTOR_PUSH(&tokens, signs[i]);
                ch += len;
                found = true;
            }
        }
        if (found) continue;

        if ('0' <= *ch && *ch <= '9') {
            errno = 0;
            char *end = NULL;
            long num = strtol(ch, &end, 0);
            assert(errno == 0 && end != NULL);

            _upf_token token = {
                .kind = _UPF_TOK_NUMBER,
                .as = { .num = num, },
            };
            VECTOR_PUSH(&tokens, token);

            ch = end;
            continue;
        }

        if (('a' <= *ch && *ch <= 'z') || ('A' <= *ch && *ch <= 'Z') || *ch == '_') {
            const char *end = ch;
            while (('a' <= *end && *end <= 'z') || ('A' <= *end && *end <= 'Z') || ('0' <= *end && *end <= '9') || *end == '_') end++;

            const char *string = _upf_arena_string(&_upf_vec_arena, ch, end);

            enum _upf_token_kind kind = _UPF_TOK_ID;
            for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
                if (strcmp(string, keywords[i]) == 0) {
                    kind = _UPF_TOK_KEYWORD;
                    break;
                }
            }

            _upf_token token = {
                .kind = kind,
                .as = { .string = string, },
            };
            VECTOR_PUSH(&tokens, token);

            ch = end;
            continue;
        }

        if (*ch == '"') {
            ch++;

            const char *end = ch;
            while (*end != '"' && *end != '\0') end++;
            assert(*end != '\0');  // C compiler won't allow this, so it must be a bug within the code

            _upf_token token = {
                .kind = _UPF_TOK_STRING,
                .as = { .string = _upf_arena_string(&_upf_vec_arena, ch, end), },
            };
            VECTOR_PUSH(&tokens, token);

            ch = end + 1;
            continue;
        }

        ERROR("Unkown character when parsing arguments '%c'.\n", *ch);  // TODO: don't crash?
    }

    _upf_tokenizer tokenizer = {
        .tokens = tokens,
        .idx = 0,
    };
    return tokenizer;
}

static bool _upf_can_consume(const _upf_tokenizer *t) { return t->idx < t->tokens.length; }

static void _upf_skip(_upf_tokenizer *t) {
    if (!_upf_can_consume(t)) ERROR("todo1");

    t->idx++;
}

static _upf_token _upf_consume(_upf_tokenizer *t, enum _upf_token_kind kind) {
    if (!_upf_can_consume(t)) ERROR("todo2");

    _upf_token token = t->tokens.data[t->idx++];
    if (token.kind != kind) ERROR("expected %d\n", kind);

    return token;
}

static bool _upf_try_consume(_upf_tokenizer *t, enum _upf_token_kind kind) {
    if (!_upf_can_consume(t)) return false;

    _upf_token token = t->tokens.data[t->idx];
    if (token.kind != kind) return false;

    t->idx++;
    return true;
}

static _upf_token _upf_consume_any2(_upf_tokenizer *t, ...) {
    va_list va_args;
    va_start(va_args, t);

    if (!_upf_can_consume(t)) ERROR("TODO");
    _upf_token token = t->tokens.data[t->idx++];

    while (1) {
        enum _upf_token_kind kind = va_arg(va_args, enum _upf_token_kind);
        if (kind == _UPF_TOK_NONE) break;

        if (token.kind == kind) {
            va_end(va_args);
            return token;
        }
    }

    ERROR("TODO");
}

#define _upf_consume_any(t, ...) _upf_consume_any2(t, __VA_ARGS__, _UPF_TOK_NONE)

VECTOR_TYPEDEF(_upf_idx_vec, size_t);
VECTOR_TYPEDEF(_upf_cstr_vec, const char *);

static _upf_idx_vec _upf_parse_args(uint64_t pc, const char *args) {
    _upf_idx_vec types = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    _upf_tokenizer t = _upf_tokenize(args);

    _upf_cstr_vec vars = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    while (_upf_can_consume(&t)) {
        vars.length = 0;
        bool is_casted = false;
        const char *type_name = NULL;
        const char *var = NULL;
        int deref = 0;

        if (_upf_try_consume(&t, _UPF_TOK_OPEN_PAREN)) {
            _upf_try_consume(&t, _UPF_TOK_KEYWORD);

            type_name = _upf_consume(&t, _UPF_TOK_ID).as.string;
            is_casted = true;

            _upf_consume(&t, _UPF_TOK_STAR);
            _upf_consume(&t, _UPF_TOK_CLOSE_PAREN);

            while (_upf_can_consume(&t) && !_upf_try_consume(&t, _UPF_TOK_COMMA)) _upf_skip(&t);
        } else {
            if (_upf_try_consume(&t, _UPF_TOK_AMPERSAND)) deref--;
            while (_upf_can_consume(&t) && _upf_try_consume(&t, _UPF_TOK_STAR)) deref++;

            VECTOR_PUSH(&vars, _upf_consume(&t, _UPF_TOK_ID).as.string);
            while (_upf_can_consume(&t) && !_upf_try_consume(&t, _UPF_TOK_COMMA)) {
                if (_upf_try_consume(&t, _UPF_TOK_OPEN_BRACKET)) {
                    _upf_consume(&t, _UPF_TOK_NUMBER);
                    _upf_consume(&t, _UPF_TOK_CLOSE_BRACKET);
                    if (!_upf_can_consume(&t) || _upf_try_consume(&t, _UPF_TOK_COMMA)) break;
                }

                _upf_consume_any(&t, _UPF_TOK_DOT, _UPF_TOK_PTR_MEMBER);
                VECTOR_PUSH(&vars, _upf_consume(&t, _UPF_TOK_ID).as.string);
            }
        }

        if (is_casted) {
            bool found = false;
            for (size_t i = 0; i < _upf_cus.length && !found; i++) {
                // TODO: is it inclusive on both ends?
                _upf_cu cu = _upf_cus.data[i];  // TODO: make this regular cu to pass it down?
                if (cu.low_pc <= pc && pc <= cu.high_pc) {
                    for (size_t j = 0; j < cu.types.length && !found; j++) {
                        _upf_type2 type = cu.types.data[j];
                        if (strcmp(type.name, type_name) == 0) {
                            size_t type_idx = _upf_get_type(&cu, type.type_die);
                            VECTOR_PUSH(&types, type_idx);
                            found = true;
                        }
                    }
                }
            }
            assert(found);  // TODO: is it even possible?
        } else {
            // TODO: find variables and then each next one must be its member -> get types recursively
        }
    }

    return types;
}



__attribute__((noinline)) void _upf_uprintf(const char *file, uint64_t line, const char *fmt, const char *args, ...) {
    static char buffer[65536];  // TODO: dynamically resize and/or check size
    char *p = buffer;

    void *return_pc = __builtin_extract_return_addr(__builtin_return_address(0));
    uint64_t pc = return_pc - _upf_dwarf.this_file;

    _upf_idx_vec types = _upf_parse_args(pc, args);
    size_t arg_idx = 0;

    va_list va_args;
    va_start(va_args, args);
    for (const char *ch = fmt; *ch != '\0'; ch++) {
        if (*ch != '%') {
            *p++ = *ch;
            continue;
        }

        char next = *(ch + 1);
        if (next == '%') {
            *p++ = '%';
            ch++;
        } else if (next == 'S') {
            if (arg_idx >= types.length) ERROR("there are more conversion specifiers than arguments provided at %s:%lu\n", file, line);

            void *ptr = va_arg(va_args, void *);
            _upf_type *type = &_upf_type_map.data[types.data[arg_idx++]].type;
            p = _upf_print_type(p, ptr, type, 0);

            ch++;
        } else if (next == '\0' || next == '\n') {
            ERROR("unfinished format at the end of line at %s:%lu\n", file, line);
        } else {
            ERROR("unkown format '%%%c' at %s:%lu\n", next, file, line);
        }
    }
    va_end(va_args);

    assert(arg_idx == types.length);  // TODO: warning/error not enough arguments

    *p = '\0';
    printf("%s", buffer);
}


#undef LOC_ADDR
#undef LOC_FBREG
#undef false
#undef true
#undef bool
#undef VECTOR_FREE
#undef VECTOR_PUSH
#undef VECTOR_TYPEDEF
#undef VECTOR_CSTR_ARENA
#undef INITIAL_VECTOR_CAPACITY
#undef OUT_OF_MEMORY
#undef ERROR
#undef INVALID

#endif  // UPRINTF_IMPLEMENTATION
