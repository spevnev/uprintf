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

#ifndef __COUNTER__
#error ERROR: uprintf requires compiler/preprocessor to support "__COUNTER__" predefined macro
#endif


#ifndef UPRINTF_H
#define UPRINTF_H

#define uprintf(fmt, ...) _upf_uprintf(__FILE__, __LINE__, __COUNTER__, fmt, __VA_ARGS__)

void _upf_uprintf(const char *file, int line, int counter, const char *fmt, ...);

#endif  // UPRINTF_H


#ifdef UPRINTF_IMPLEMENTATION

#include <assert.h>
#include <dwarf.h>
#include <elf.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


// number of spaces of indentation per each layer of nesting
#ifndef UPRINTF_INDENT
#define UPRINTF_INDENT 4
#endif


#define INVALID_OFFSET -1UL

#define ERROR(...)                            \
    do {                                      \
        fprintf(stderr, "[ERROR] uprintf: "); \
        fprintf(stderr, __VA_ARGS__);         \
        exit(1);                              \
    } while (0)

#define OUT_OF_MEMORY() ERROR("process ran out of memory.\n");


#define INITIAL_ARENA_SIZE 4096

typedef struct _upf_arena_region {
    char *data;
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
    region->data = (char *) malloc(region->capacity);
    if (region->data == NULL) OUT_OF_MEMORY();
    region->length = 0;
    region->next = NULL;

    arena->head = region;
    arena->tail = region;
}

static char *_upf_arena_alloc(_upf_arena *arena, size_t size) {
    assert(arena->head != NULL && arena->tail != NULL);

    if (size > arena->head->capacity - arena->head->length) {
        _upf_arena_region *region = (_upf_arena_region *) malloc(sizeof(*region));
        if (region == NULL) OUT_OF_MEMORY();
        region->capacity = arena->head->capacity * 2;
        region->data = (char *) malloc(region->capacity);
        if (region->data == NULL) OUT_OF_MEMORY();
        region->length = 0;
        region->next = NULL;

        arena->head->next = region;
        arena->head = region;
    }

    char *memory = arena->head->data + arena->head->length;
    arena->head->length += size;
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
            (vec)->capacity *= 2;                                                             \
            size_t size = (vec)->capacity * sizeof(*(vec)->data);                             \
            if ((vec)->arena == NULL) (vec)->data = realloc((vec)->data, size);               \
            else {                                                                            \
                void *new_data = _upf_arena_alloc((vec)->arena, size);                        \
                assert(size % 2 == 0);                                                        \
                memcpy(new_data, (vec)->data, size / 2);                                      \
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

enum _upf_type_type {
    _UPF_TT_STRUCT,
    _UPF_TT_U1,
    _UPF_TT_U2,
    _UPF_TT_U4,
    _UPF_TT_U8,
    _UPF_TT_S1,
    _UPF_TT_S2,
    _UPF_TT_S4,
    _UPF_TT_S8,
    _UPF_TT_F4,
    _UPF_TT_F8,
    _UPF_TT_CH,
    _UPF_TT_UCH,
    _UPF_TT_VOID
};

typedef struct {
    const char *name;
    enum _upf_type_type type;
    _upf_field_vec fields;
    bool is_pointer;
    bool is_const;
} _upf_type;

typedef struct _upf_field {
    const char *name;
    _upf_type type;
    int offset;
} _upf_field;

typedef struct {
    const uint8_t *type_die;
    _upf_type type;
} _upf_type_entry;

VECTOR_TYPEDEF(_upf_type_vec, _upf_type_entry);

VECTOR_TYPEDEF(_upf_size_vec, size_t);

typedef struct {
    const char *file;
    int64_t line;
    int64_t counter;
    _upf_size_vec arg_types;
} _upf_arg_entry;

VECTOR_TYPEDEF(_upf_arg_vec, _upf_arg_entry);

enum _upf_param_kind { _UPF_PK_FILE, _UPF_PK_LINE, _UPF_PK_COUNTER, _UPF_PK_FMT, _UPF_PK_VA_ARG };

typedef struct {
    _upf_loc loc;
    enum _upf_param_kind kind;
} _upf_param;

VECTOR_TYPEDEF(_upf_param_vec, _upf_param);

typedef struct {
    uint64_t offset;
    _upf_param_vec params;
} _upf_uprintf_info;

typedef struct {
    const uint8_t *base;
    const _upf_abbrev_vec *abbrevs;
    uint64_t str_offsets_base;
    uint64_t addr_base;
} _upf_cu_info;

struct _upf_dwarf {
    uint8_t *file;
    off_t file_size;

    bool is_64bit;
    uint8_t offset_size;
    uint8_t address_size;

    const uint8_t *info;
    size_t info_size;
    const uint8_t *abbrev;
    const char *str;
    const char *line_str;
    const uint8_t *loclists;
    const uint8_t *str_offsets;
    const uint8_t *addr;
};


static _upf_arena _upf_vec_arena = {0};
static struct _upf_dwarf _upf_dwarf = {0};
static _upf_arg_vec _upf_args_map = VECTOR_CSTR_ARENA(&_upf_vec_arena);   // TODO: rename
static _upf_type_vec _upf_type_map = VECTOR_CSTR_ARENA(&_upf_vec_arena);  // TODO: rename


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

// TODO: rename
static uint64_t _upf_get_offset(const uint8_t *info) { return _upf_dwarf.offset_size == 8 ? *((uint64_t *) info) : *((uint32_t *) info); }

static uint64_t _upf_get_address2(const uint8_t *info) {
    return _upf_dwarf.address_size == 8 ? *((uint64_t *) info) : *((uint32_t *) info);
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

static _upf_param_value _upf_eval_dwarf_expr(const _upf_cu_info *cu, const uint8_t *info) {
    // TODO: rename struct, and variable to result
    _upf_param_value param = {0};

    uint64_t len;
    info += _upf_uLEB_to_uint64(info, &len);

    uint8_t opcode = *info++;
    len--;

    if (DW_OP_lit0 <= opcode && opcode <= DW_OP_lit31) {
        if (len != 0) goto skip;
        param.is_const = true;
        param.as.constant = opcode - DW_OP_lit0;
    } else if (opcode == DW_OP_const1u) {
        if (len != 1) goto skip;
        param.is_const = true;
        param.as.constant = *((uint8_t *) info);
    } else if (opcode == DW_OP_const1s) {
        if (len != 1) goto skip;
        param.is_const = true;
        param.as.constant = *((int8_t *) info);
    } else if (opcode == DW_OP_const2u) {
        if (len != 2) goto skip;
        param.is_const = true;
        param.as.constant = *((uint16_t *) info);
    } else if (opcode == DW_OP_const2s) {
        if (len != 2) goto skip;
        param.is_const = true;
        param.as.constant = *((int16_t *) info);
    } else if (opcode == DW_OP_const4u) {
        if (len != 4) goto skip;
        param.is_const = true;
        param.as.constant = *((uint32_t *) info);
    } else if (opcode == DW_OP_const4s) {
        if (len != 4) goto skip;
        param.is_const = true;
        param.as.constant = *((int32_t *) info);
    } else if (opcode == DW_OP_const8u) {
        if (len != 8) goto skip;
        param.is_const = true;
        param.as.constant = *((uint64_t *) info);
    } else if (opcode == DW_OP_const8s) {
        if (len != 8) goto skip;
        param.is_const = true;
        param.as.constant = *((int64_t *) info);
    } else if (opcode == DW_OP_constu) {
        param.is_const = true;
        size_t leb_size = _upf_uLEB_to_uint64(info, (uint64_t *) &param.as.constant);
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
        param.as.loc.offset = _upf_get_offset(info);
        if (len != _upf_dwarf.address_size) goto skip;
    } else if (opcode == DW_OP_addrx) {
        assert(_upf_dwarf.addr != NULL);

        uint64_t addr_offset;
        size_t leb_len = _upf_uLEB_to_uint64(info, &addr_offset);

        param.as.loc.reg = LOC_ADDR;
        param.as.loc.offset = _upf_get_address2(_upf_dwarf.addr + cu->addr_base + addr_offset * _upf_dwarf.address_size);

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
        printf("0x%x\n", opcode);
        assert(0 && "TODO: handle other dwarf expressions");  // TODO:
    }

    return param;
skip:
    fprintf(stderr, "[WARN] skipping dwarf expression: stack is not implemented\n");
    param.is_const = 2;
    return param;
}

static uint64_t _upf_get_x_offset(const uint8_t *info, uint64_t form) {
    uint64_t offset;
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
            assert(0 && "UNREACHABLE");
    }
}

static const char *_upf_get_string(const _upf_cu_info *cu, const uint8_t *info, uint64_t form) {
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
            assert(_upf_dwarf.str_offsets != NULL && cu->str_offsets_base != INVALID_OFFSET);
            uint64_t offset = cu->str_offsets_base + _upf_get_x_offset(info, form) * _upf_dwarf.offset_size;
            return _upf_dwarf.str + _upf_get_offset(_upf_dwarf.str_offsets + offset);
        }
        default:
            assert(0 && "TODO: handle other string types");  // TODO:
    }
}

static uint64_t _upf_get_ref(const uint8_t *info, uint64_t form) {
    if (form == DW_FORM_ref1) {
        return *info;
    } else if (form == DW_FORM_ref2) {
        return *((uint16_t *) info);
    } else if (form == DW_FORM_ref4) {
        return *((uint32_t *) info);
    } else if (form == DW_FORM_ref8) {
        return *((uint64_t *) info);
    } else if (form == DW_FORM_ref_udata) {
        uint64_t result;
        _upf_uLEB_to_uint64(info, &result);
        return result;
    } else {
        assert(0 && "TODO: handle other ref types");  // TODO:
    }
}

static int64_t _upf_get_data(const uint8_t *info, _upf_attr attr) {
    if (attr.form == DW_FORM_data1) {
        return *info;
    } else if (attr.form == DW_FORM_data2) {
        return *((int16_t *) info);
    } else if (attr.form == DW_FORM_data4) {
        return *((int32_t *) info);
    } else if (attr.form == DW_FORM_data8) {
        return *((int64_t *) info);
    } else if (attr.form == DW_FORM_implicit_const) {
        return attr.implicit_const;
    } else {
        printf("0x%02lx\n", attr.form);
        assert(0 && "TODO: handle other data types");  // TODO:
    }
}

static uint64_t _upf_get_address(const _upf_cu_info *cu, const uint8_t *info, uint64_t form) {
    if (form == DW_FORM_addr) {
        return _upf_get_offset(info);
    } else if (form == DW_FORM_addrx) {
        assert(_upf_dwarf.addr != NULL);
        uint64_t offset = cu->addr_base + _upf_get_x_offset(info, form) * _upf_dwarf.address_size;
        return _upf_get_address2(_upf_dwarf.addr + offset);
    } else {
        assert(0 && "TODO: handle other address types");  // TODO:
    }
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
            assert(0 && "UNREACHABLE");
    }
}

static const uint8_t *_upf_skip_die(const uint8_t *info, const _upf_abbrev *abbrev) {
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        info += _upf_get_attr_size(info, abbrev->attrs.data[i].form);
    }

    return info;
}

// TODO: fully parse type, push it to _upf_type_map and return pointer into the map
static size_t _upf_get_type_helper(const _upf_cu_info *cu, const uint8_t *info, _upf_type *type) {
    // TODO: FULLY rework caching

    const uint8_t *base = info;

    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= cu->abbrevs->length);
    const _upf_abbrev *abbrev = &cu->abbrevs->data[code - 1];

    if (abbrev->tag == DW_TAG_structure_type) {
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(cu, info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(type->name != NULL);  // TODO:

        while (1) {
            info += _upf_uLEB_to_uint64(info, &code);
            if (code == 0) break;

            assert(code <= cu->abbrevs->length);
            abbrev = &cu->abbrevs->data[code - 1];
            assert(abbrev->tag == DW_TAG_member);

            _upf_field field = {0};
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    field.name = _upf_get_string(cu, info, attr.form);
                } else if (attr.name == DW_AT_type) {
                    _upf_get_type_helper(cu, cu->base + _upf_get_ref(info, attr.form), &field.type);
                } else if (attr.name == DW_AT_data_member_location) {
                    field.offset = _upf_get_data(info, attr);
                }

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(field.name != NULL);  // TODO: other fields

            VECTOR_PUSH(&type->fields, field);
        }

        _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    if (abbrev->tag == DW_TAG_base_type) {
        int size = -1;
        int encoding = -1;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_byte_size) size = _upf_get_data(info, attr);
            else if (attr.name == DW_AT_encoding) encoding = _upf_get_data(info, attr);
            else if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(cu, info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(size != -1 && encoding != -1 && type->name != NULL);  // TODO:

        if (encoding == DW_ATE_signed) {
            if (size == 1) type->type = _UPF_TT_S1;
            else if (size == 2) type->type = _UPF_TT_S2;
            else if (size == 4) type->type = _UPF_TT_S4;
            else if (size == 8) type->type = _UPF_TT_S8;
            else assert(0 && "TODO: handle other base types");  // TODO:
        } else if (encoding == DW_ATE_unsigned) {
            if (size == 1) type->type = _UPF_TT_U1;
            else if (size == 2) type->type = _UPF_TT_U2;
            else if (size == 4) type->type = _UPF_TT_U4;
            else if (size == 8) type->type = _UPF_TT_U8;
            else assert(0 && "TODO: handle other base types");  // TODO:
        } else if (encoding == DW_ATE_float) {
            if (size == 4) type->type = _UPF_TT_F4;
            else if (size == 8) type->type = _UPF_TT_F8;
            else assert(0 && "TODO: handle other base types");  // TODO:
        } else if (encoding == DW_ATE_signed_char) {
            assert(size == 1);
            type->type = _UPF_TT_CH;
        } else if (encoding == DW_ATE_unsigned_char) {
            assert(size == 1);
            type->type = _UPF_TT_UCH;
        } else {
            printf("0x%02x\n", encoding);
            assert(0 && "TODO: handle other base types");  // TODO:
        }

        // TODO: don't add base types to the map
        _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    if (abbrev->tag == DW_TAG_typedef) {
        const uint8_t *next_info = NULL;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_type) next_info = cu->base + _upf_get_ref(info, attr.form);
            else if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(cu, info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(next_info != NULL);  // TODO:

        _upf_get_type_helper(cu, next_info, type);

        _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    if (abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type) {
        uint64_t offset = INVALID_OFFSET;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_type) offset = _upf_get_ref(info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }

        if (offset == INVALID_OFFSET) {
            assert(abbrev->tag == DW_TAG_pointer_type);
            type->type = _UPF_TT_VOID;
            type->name = "void";
        } else {
            _upf_get_type_helper(cu, cu->base + offset, type);
            if (abbrev->tag != DW_TAG_pointer_type) {
                assert(abbrev->tag == DW_TAG_const_type);

                type->is_const = true;
                _upf_type_entry entry = {
                    .type_die = base,
                    .type = *type,
                };
                VECTOR_PUSH(&_upf_type_map, entry);
                return _upf_type_map.length - 1;
            }
        }

        type->is_pointer = true;  // TODO: better way to add pointer than current(duplicating the entire type entry)

        _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    return -1U;
}

static size_t _upf_get_type(const _upf_cu_info *cu, const uint8_t *info) {
    _upf_type type = {
        .fields = VECTOR_CSTR_ARENA(&_upf_vec_arena),
    };

    return _upf_get_type_helper(cu, info, &type);
}

// TODO: is there any point in returning bool, or partial result + handle them
static bool _upf_get_locations(const _upf_cu_info *cu, const uint8_t *info, uint64_t form, _upf_loc_vec *locs) {
    assert(locs->length == 0);
    if (form == DW_FORM_exprloc) {
        _upf_param_value param = _upf_eval_dwarf_expr(cu, info);
        if (param.is_const == 2) return false;
        assert(!param.is_const);
        VECTOR_PUSH(locs, param.as.loc);
    } else if (form == DW_FORM_sec_offset) {
        assert(_upf_dwarf.loclists != NULL);
        uint64_t base = INVALID_OFFSET;
        bool result = true;
        uint64_t o = _upf_get_offset(info);
        const uint8_t *loclist = _upf_dwarf.loclists + o;
        while (*loclist != DW_LLE_end_of_list) {
            switch (*loclist++) {
                case DW_LLE_base_address:
                    base = _upf_get_address2(loclist);
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
                    uint64_t from = _upf_get_address2(loclist);
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
    } else {
        assert(0 && "TODO: handle other forms of DW_AT_location in variable/parameter");  // TODO:
    }

    return true;
}

// TODO: rename to include parameter?
static _upf_var_entry _upf_parse_variable(const _upf_cu_info *cu, const uint8_t *info, const _upf_abbrev *abbrev) {
    _upf_var_entry var = {
        .locs = VECTOR_CSTR_ARENA(&_upf_vec_arena),
    };

    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_location) {
            if (!_upf_get_locations(cu, info, attr.form, &var.locs)) return var;
        } else if (attr.name == DW_AT_type) {
            var.type_die = cu->base + _upf_get_ref(info, attr.form);
        }

        info += _upf_get_attr_size(info, attr.form);
    }

    return var;
}

static void _upf_parse_uprintf_call_site(const _upf_cu_info *cu, const _upf_param_vec *params, const _upf_call_site *call_site,
                                         const char *file_path, _upf_var_vec *vars) {
    uint64_t code;
    const _upf_abbrev *abbrev;

    const uint8_t *subprogram = call_site->subprogram;
    uint64_t low_pc = INVALID_OFFSET;
    while (1) {
        subprogram += _upf_uLEB_to_uint64(subprogram, &code);
        if (subprogram >= call_site->info) break;
        if (code == 0) continue;

        assert(code <= cu->abbrevs->length);
        abbrev = &cu->abbrevs->data[code - 1];

        if (abbrev->tag == DW_TAG_subprogram) {
            const uint8_t *info = subprogram;  // TODO: rename
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_low_pc) {
                    low_pc = _upf_get_address(cu, info, attr.form);
                    break;
                }

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(low_pc != INVALID_OFFSET);
        } else if (abbrev->tag == DW_TAG_formal_parameter || abbrev->tag == DW_TAG_variable) {
            _upf_var_entry var = _upf_parse_variable(cu, subprogram, abbrev);
            if (var.locs.length > 0) VECTOR_PUSH(vars, var);
        }

        subprogram = _upf_skip_die(subprogram, abbrev);
    }
    // TODO: save/cache results when there are multiple _upf_uprintf calls in the same function

    const uint8_t *info = call_site->info;

    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= cu->abbrevs->length);
    abbrev = &cu->abbrevs->data[code - 1];
    uint64_t return_pc = INVALID_OFFSET;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_return_pc) return_pc = _upf_get_address(cu, info, attr.form);

        info += _upf_get_attr_size(info, attr.form);
    }
    assert(return_pc != INVALID_OFFSET);

    _upf_loc_vec locs = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    _upf_arg_entry entry = {
        .file = file_path,
        .line = -1,
        .counter = -1,
        .arg_types = VECTOR_CSTR_ARENA(&_upf_vec_arena),
    };
    while (1) {
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) break;

        assert(code <= cu->abbrevs->length);
        abbrev = &cu->abbrevs->data[code - 1];
        assert(abbrev->tag == DW_TAG_call_site_parameter);

        const uint8_t *value_info = NULL;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_location) {
                locs.length = 0;
                bool result = _upf_get_locations(cu, info, attr.form, &locs);
                assert(result);
                assert(locs.length == 1);
            } else if (attr.name == DW_AT_call_value) {
                value_info = info;
                assert(attr.form == DW_FORM_exprloc);
            }

            info += _upf_get_attr_size(info, attr.form);
        }

        enum _upf_param_kind kind = _UPF_PK_VA_ARG;
        for (size_t i = 0; i < params->length; i++) {
            _upf_param param = params->data[i];
            if (param.loc.reg == locs.data[0].reg && param.loc.offset == locs.data[0].offset) {
                kind = param.kind;
                break;
            }
        }

        _upf_param_value param;
        switch (kind) {
            case _UPF_PK_FILE:
            case _UPF_PK_FMT:
                break;
            case _UPF_PK_LINE:
            case _UPF_PK_COUNTER:
                assert(value_info != NULL);
                param = _upf_eval_dwarf_expr(cu, value_info);
                assert(param.is_const);
                if (kind == _UPF_PK_LINE) {
                    entry.line = param.as.constant;
                    assert(entry.line >= 0 && "line number mustn't be negative");
                } else {
                    entry.counter = param.as.constant;
                    assert(entry.counter >= 0 && "__COUNTER__ shouldn't be negative");
                }
                break;
            case _UPF_PK_VA_ARG: {
                assert(value_info != NULL);
                param = _upf_eval_dwarf_expr(cu, value_info);
                if (param.is_const == 2) {
                    // TODO: what if line number is -1/wrong?
                    fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%ld\n", file_path, entry.line);
                    return;
                }
                assert(!param.is_const);

                bool found = false;
                for (size_t i = 0; i < vars->length; i++) {
                    _upf_var_entry var = vars->data[i];
                    for (size_t j = 0; j < var.locs.length; j++) {
                        _upf_loc loc = var.locs.data[j];
                        if (loc.is_param == param.as.loc.is_param && loc.reg == param.as.loc.reg && loc.offset == param.as.loc.offset) {
                            if (loc.is_scoped) {
                                // TODO: return_pc isn't the variable we should be using although is close enough
                                assert(return_pc != INVALID_OFFSET);
                                if (!(loc.from < return_pc && return_pc <= loc.to)) continue;
                            }

                            size_t type_idx = _upf_get_type(cu, var.type_die);
                            if (type_idx != -1U) {
                                VECTOR_PUSH(&entry.arg_types, type_idx);
                                found = true;
                            }
                            break;
                        }
                    }
                    if (found) break;
                }
                if (!found) {
                    // TODO: what if line number is -1/wrong?
                    fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%ld\n", file_path, entry.line);
                    return;
                }
            } break;
            default:
                assert(0 && "UNREACHABLE");
        }
    }

    assert(entry.line != -1 && entry.counter != -1);  // TODO: what if not found? how to identify?
    VECTOR_PUSH(&_upf_args_map, entry);
}

static const uint8_t *_upf_parse_uprintf_subprogram(const _upf_cu_info *cu, const uint8_t *info, const _upf_abbrev *abbrev,
                                                    const uint8_t *die_base, _upf_uprintf_info *uprintf) {
    static const char *UPRINTF_FUNCTION_NAME = "_upf_uprintf";

    bool found = false;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            const char *function_name = _upf_get_string(cu, info, attr.form);
            if (strcmp(function_name, UPRINTF_FUNCTION_NAME) == 0) {
                found = true;
                uprintf->offset = die_base - cu->base;
            }
        }

        info += _upf_get_attr_size(info, attr.form);
    }
    if (!found) return info;

    // TODO: check param parsing: print + compare to dwarfdump
    uint64_t code;
    const uint8_t *prev_info = info;
    _upf_loc_vec locs = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    while (1) {
        info += _upf_uLEB_to_uint64(info, &code);
        assert(code > 0 && code <= cu->abbrevs->length);

        abbrev = &cu->abbrevs->data[code - 1];
        if (abbrev->tag != DW_TAG_formal_parameter) return prev_info;

        _upf_param param = {0};
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_name) {
                const char *name = _upf_get_string(cu, info, attr.form);
                if (strcmp(name, "file") == 0) param.kind = _UPF_PK_FILE;
                else if (strcmp(name, "line") == 0) param.kind = _UPF_PK_LINE;
                else if (strcmp(name, "counter") == 0) param.kind = _UPF_PK_COUNTER;
                else if (strcmp(name, "fmt") == 0) param.kind = _UPF_PK_FMT;
                else assert(0 && "UNREACHABLE");
            } else if (attr.name == DW_AT_location) {
                locs.length = 0;
                _upf_get_locations(cu, info, attr.form, &locs);
                assert(locs.length > 0);
                param.loc = locs.data[0];
            }

            info += _upf_get_attr_size(info, attr.form);
        }
        VECTOR_PUSH(&uprintf->params, param);
    }
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *info, const uint8_t *info_end, const uint8_t *abbrev_table) {
    _upf_abbrev_vec abbrevs = _upf_parse_abbrevs(abbrev_table);

    _upf_uprintf_info uprintf = {
        .offset = INVALID_OFFSET,
        .params = VECTOR_CSTR_ARENA(&_upf_vec_arena),
    };
    _upf_cu_info cu = {
        .base = cu_base,
        .abbrevs = &abbrevs,
        .str_offsets_base = INVALID_OFFSET,
        .addr_base = INVALID_OFFSET,
    };
    _upf_call_site_vec call_sites = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    _upf_var_vec vars = VECTOR_CSTR_ARENA(&_upf_vec_arena);
    const uint8_t *subprogram = NULL;
    const char *file_path = NULL;
    int depth = 0;
    while (info < info_end) {
        const uint8_t *die_base = info;

        uint64_t code;
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) {
            depth--;
            continue;
        }

        assert(code <= abbrevs.length);
        const _upf_abbrev *abbrev = &abbrevs.data[code - 1];
        if (abbrev->has_children) depth++;

        if (abbrev->tag == DW_TAG_subprogram) subprogram = die_base;

        if (abbrev->tag == DW_TAG_compile_unit) {
            const uint8_t *name_info = NULL;
            uint64_t name_form = INVALID_OFFSET;

            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    name_info = info;
                    name_form = attr.form;
                } else if (attr.name == DW_AT_str_offsets_base) {
                    cu.str_offsets_base = _upf_get_offset(info);
                } else if (attr.name == DW_AT_addr_base) {
                    cu.addr_base = _upf_get_offset(info);
                }

                info += _upf_get_attr_size(info, attr.form);
            }

            assert(name_info != NULL && name_form != INVALID_OFFSET);
            file_path = _upf_get_string(&cu, name_info, name_form);
        } else if (abbrev->tag == DW_TAG_subprogram && uprintf.offset == INVALID_OFFSET) {
            info = _upf_parse_uprintf_subprogram(&cu, info, abbrev, die_base, &uprintf);
        } else if (abbrev->tag == DW_TAG_call_site) {
            uint64_t call_origin = INVALID_OFFSET;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_call_origin) call_origin = _upf_get_ref(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(call_origin != INVALID_OFFSET);

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
        } else {
            info = _upf_skip_die(info, abbrev);
        }
    }
    assert(file_path != NULL && "compilation unit must have a path.");
    if (uprintf.offset == INVALID_OFFSET) return;

    for (size_t i = 0; i < call_sites.length; i++) {
        _upf_call_site *call_site = &call_sites.data[i];
        if (call_site->call_origin == uprintf.offset) {
            size_t vars_length = vars.length;
            _upf_parse_uprintf_call_site(&cu, &uprintf.params, call_site, file_path, &vars);
            vars.length = vars_length;  // clear variables local to parsed subprogram
        }
    }
}

static void _upf_parse_dwarf() {
    const uint8_t *info = _upf_dwarf.info;
    const uint8_t *info_end = info + _upf_dwarf.info_size;
    while (info < info_end) {
        const uint8_t *cu_base = info;

        uint64_t length = *((uint32_t *) info);
        info += sizeof(uint32_t);

        _upf_dwarf.is_64bit = false;
        if (length == 0xffffffffU) {
            length = *((uint64_t *) info);
            info += sizeof(uint64_t);
            _upf_dwarf.is_64bit = true;
        }
        _upf_dwarf.offset_size = _upf_dwarf.is_64bit ? 8 : 4;

        const uint8_t *next = info + length;

        uint16_t version = *((uint16_t *) info);
        info += sizeof(version);
        assert(version == 5);

        uint8_t type = *info;
        info += sizeof(type);
        assert(type == DW_UT_compile);

        uint8_t address_size = *info;
        assert(_upf_dwarf.address_size == 0 || _upf_dwarf.address_size == address_size);
        _upf_dwarf.address_size = address_size;
        info += sizeof(address_size);

        uint64_t abbrev_offset;
        if (_upf_dwarf.is_64bit) {
            abbrev_offset = *((uint64_t *) info);
            info += sizeof(uint64_t);
        } else {
            abbrev_offset = *((uint32_t *) info);
            info += sizeof(uint32_t);
        }

        _upf_parse_cu(cu_base, info, next, _upf_dwarf.abbrev + abbrev_offset);

        info = next;
    }
}

static void _upf_parse_elf(void) {
    static const char *THIS_FILE_PATH = "/proc/self/exe";

    struct stat file_info;
    if (stat(THIS_FILE_PATH, &file_info) == -1) abort();
    size_t size = file_info.st_size;
    _upf_dwarf.file_size = size;

    int fd = open(THIS_FILE_PATH, O_RDONLY);
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
            _upf_dwarf.loclists = file + section->sh_offset;
        } else if (strcmp(name, ".debug_str_offsets") == 0) {
            const uint8_t *temp = file + section->sh_offset;
            _upf_dwarf.str_offsets = temp;

            // skipping length
            if (*((uint32_t *) temp) == 0xffffffffU) temp += sizeof(uint64_t);
            temp += sizeof(uint32_t);

            uint16_t version = *((uint16_t *) temp);
            assert(version == 5);
            temp += sizeof(uint16_t);

            uint16_t reserved = *((uint16_t *) temp);
            assert(reserved == 0);
            temp += sizeof(uint16_t);
        } else if (strcmp(name, ".debug_addr") == 0) {
            const uint8_t *temp = file + section->sh_offset;
            _upf_dwarf.addr = temp;

            // skipping length
            if (*((uint32_t *) temp) == 0xffffffffU) temp += sizeof(uint64_t);
            temp += sizeof(uint32_t);

            uint16_t version = *((uint16_t *) temp);
            assert(version == 5);
            temp += sizeof(uint16_t);

            uint8_t address_size = *((uint8_t *) temp);
            _upf_dwarf.address_size = address_size;
            temp += sizeof(uint8_t);

            uint8_t segment_selector_size = *((uint8_t *) temp);
            assert(segment_selector_size == 0
                   && "Segmented addresses aren't supported since x86_64/amd64 (the only supported architecture) doesn't have them");
            temp += sizeof(uint8_t);
        }

        section++;
    }
    assert(_upf_dwarf.info != NULL && _upf_dwarf.abbrev != NULL && _upf_dwarf.str != NULL);
}

static char *_upf_print_char(char *p, char ch) {
    switch (ch) {
        case '\a':
            *p++ = '\\';
            *p++ = 'a';
            break;
        case '\b':
            *p++ = '\\';
            *p++ = 'b';
            break;
        case '\f':
            *p++ = '\\';
            *p++ = 'f';
            break;
        case '\n':
            *p++ = '\\';
            *p++ = 'n';
            break;
        case '\r':
            *p++ = '\\';
            *p++ = 'r';
            break;
        case '\t':
            *p++ = '\\';
            *p++ = 't';
            break;
        case '\v':
            *p++ = '\\';
            *p++ = 'v';
            break;
        case '\\':
            *p++ = '\\';
            *p++ = '\\';
            break;
        default:
            *p++ = ch;
            break;
    }
    return p;
}

static bool _upf_is_printable(char c) { return ' ' <= c && c <= '~'; }

static char *_upf_print_type(char *p, const uint8_t *data, const _upf_type *type, int depth) {
    // TODO: snprintf

    if (type->is_pointer) {
        switch (type->type) {
            case _UPF_TT_STRUCT:
                // TODO: check that the pointer is not to already seen struct
                break;
            case _UPF_TT_CH: {
                // TODO: what if it is a pointer to a char (or even int8_t) instead of c-style string?
                char *str = *((char **) data);
                p += sprintf(p, "%p (\"", (void *) str);
                while (*str != '\0') p = _upf_print_char(p, *str++);
                p += sprintf(p, "\")");
            }
                return p;
            case _UPF_TT_VOID:
                p += sprintf(p, "%p", *((void **) data));
                return p;
            default: {
                // TODO: print address and value in parentheses
                void *ptr = *((void **) data);
                if (ptr == NULL) {
                    p += sprintf(p, "NULL");
                    return p;
                }
                p += sprintf(p, "%p (", ptr);
                data = ptr;
            } break;
        }
    }

    switch (type->type) {
        case _UPF_TT_STRUCT:
            p += sprintf(p, "%s {\n", type->name);
            for (size_t i = 0; i < type->fields.length; i++) {
                _upf_field *field = &type->fields.data[i];

                int indent = UPRINTF_INDENT * (depth + 1);
                memset(p, ' ', indent);
                p += indent;

                p += sprintf(p, "%s%s %s%s = ", field->type.is_const ? "const " : "", field->type.name, field->type.is_pointer ? "*" : "",
                             field->name);
                p = _upf_print_type(p, data + field->offset, &field->type, depth + 1);
                *p++ = '\n';
            }
            p += sprintf(p, "};");
            break;
        case _UPF_TT_U1:
            p += sprintf(p, "%hhu", *((uint8_t *) data));
            break;
        case _UPF_TT_U2:
            p += sprintf(p, "%hu", *((uint16_t *) data));
            break;
        case _UPF_TT_U4:
            p += sprintf(p, "%u", *((uint32_t *) data));
            break;
        case _UPF_TT_U8:
            p += sprintf(p, "%lu", *((uint64_t *) data));
            break;
        case _UPF_TT_S1:
            p += sprintf(p, "%hhd", *((int8_t *) data));
            break;
        case _UPF_TT_S2:
            p += sprintf(p, "%hd", *((int16_t *) data));
            break;
        case _UPF_TT_S4:
            p += sprintf(p, "%d", *((int32_t *) data));
            break;
        case _UPF_TT_S8:
            p += sprintf(p, "%ld", *((int64_t *) data));
            break;
        case _UPF_TT_F4:
            p += sprintf(p, "%f", *((float *) data));
            break;
        case _UPF_TT_F8:
            p += sprintf(p, "%lf", *((double *) data));
            break;
        case _UPF_TT_UCH:
        case _UPF_TT_CH: {
            char ch = *((char *) data);
            if (type->type == _UPF_TT_CH) p += sprintf(p, "%hhd", ch);
            else p += sprintf(p, "%hhu", ch);
            if (_upf_is_printable(ch)) {
                // TODO: sprintf instead
                *p++ = ' ';
                *p++ = '(';
                *p++ = '\'';
                p = _upf_print_char(p, ch);
                *p++ = '\'';
                *p++ = ')';
            }
        } break;
        default:
            assert(0 && "UNREACHABLE");
            break;
    }

    if (type->is_pointer) *p++ = ')';

    return p;
}


__attribute__((constructor)) void _upf_init(void) {
    _upf_arena_init(&_upf_vec_arena);

    _upf_parse_elf();
    _upf_parse_dwarf();
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    munmap(_upf_dwarf.file, _upf_dwarf.file_size);
    _upf_arena_free(&_upf_vec_arena);
}

void _upf_uprintf(const char *file, int line, int counter, const char *fmt, ...) {
    static char buffer[16384];  // TODO: dynamically resize and/or check size
    char *p = buffer;

    // TODO: move to helper
    bool found = false;
    _upf_size_vec arg_types;
    for (size_t i = 0; i < _upf_args_map.length; i++) {
        _upf_arg_entry entry = _upf_args_map.data[i];
        if (entry.counter == counter && entry.line == line && strcmp(entry.file, file) == 0) {
            arg_types = entry.arg_types;
            found = true;
            break;
        }
    }
    assert(found && "TODO");  // TODO: handle not being able to parse as either an error or warning

    va_list args;
    va_start(args, fmt);
    size_t arg = 0;
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
            assert(arg < 2 && "For some reason DWARF includes only first 6 call_site_parameters in the function call, so it only accepts 2 structs at a time");
            if (arg >= arg_types.length) ERROR("there are more conversion specifiers than arguments provided at %s:%d\n", file, line);

            // TODO: why pointer?
            p = _upf_print_type(p, va_arg(args, void *), &_upf_type_map.data[arg_types.data[arg++]].type, 0);
            ch++;
        } else if (next == '\0' || next == '\n') {
            ERROR("unfinished format at the end of line at %s:%d\n", file, line);
        } else {
            ERROR("unkown format '%%%c' at %s:%d\n", next, file, line);
        }
    }
    va_end(args);

    assert(arg == arg_types.length);  // TODO: warning/error not enough arguments

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
#undef INVALID_OFFSET

#endif  // UPRINTF_IMPLEMENTATION