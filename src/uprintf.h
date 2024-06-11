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


#define ERROR(...)                            \
    do {                                      \
        fprintf(stderr, "[ERROR] uprintf: "); \
        fprintf(stderr, __VA_ARGS__);         \
        exit(1);                              \
    } while (0)

#define OUT_OF_MEMORY() ERROR("process ran out of memory.\n");


#define INITIAL_VECTOR_CAPACITY 16

#define VECTOR_TYPEDEF(name, type) \
    typedef struct {               \
        size_t capacity;           \
        size_t length;             \
        type *data;                \
    } name

#define VECTOR_PUSH(vec, element)                                                       \
    do {                                                                                \
        if ((vec)->capacity == 0) {                                                     \
            (vec)->capacity = INITIAL_VECTOR_CAPACITY;                                  \
            (vec)->data = malloc((vec)->capacity * sizeof(*(vec)->data));               \
            if ((vec)->data == NULL) OUT_OF_MEMORY();                                   \
        } else if ((vec)->length == (vec)->capacity) {                                  \
            (vec)->capacity *= 2;                                                       \
            (vec)->data = realloc((vec)->data, (vec)->capacity * sizeof(*(vec)->data)); \
            if ((vec)->data == NULL) OUT_OF_MEMORY();                                   \
        }                                                                               \
        (vec)->data[(vec)->length++] = (element);                                       \
    } while (0)

#define VECTOR_RESET(vec) (vec)->length = 0;

#define VECTOR_FREE(vec)     \
    do {                     \
        free((vec)->data);   \
        (vec)->length = 0;   \
        (vec)->capacity = 0; \
        (vec)->data = NULL;  \
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
} _upf_var_loc;

VECTOR_TYPEDEF(_upf_var_loc_vec, _upf_var_loc);

// TODO: rename, change types
typedef struct {
    _upf_var_loc_vec locs;
    const uint8_t *type_die;
} _upf_var_entry;

VECTOR_TYPEDEF(_upf_var_vec, _upf_var_entry);

typedef struct {
    bool is_const;
    // TODO: ^ enum?
    union {
        int64_t constant;
        _upf_var_loc loc;
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

struct _upf_dwarf {
    uint8_t *file;
    off_t file_size;

    bool is_64bit;
    uint8_t offset_size;
    uint8_t address_size;

    const uint8_t *info;
    size_t info_size;
    const uint8_t *abbrev;
    const uint8_t *str;
    const uint8_t *line_str;
    const uint8_t *loclists;
};


struct _upf_dwarf _upf_dwarf = {0};
static _upf_arg_vec _upf_args_map = {0};   // TODO: rename
static _upf_type_vec _upf_type_map = {0};  // TODO: rename


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
static uint64_t _upf_get_offset(const uint8_t *info) { return _upf_dwarf.is_64bit ? *((uint64_t *) info) : *((uint32_t *) info); }

static _upf_abbrev_vec _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _upf_abbrev_vec abbrevs = {0};

    while (1) {
        _upf_abbrev abbrev = {0};
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

static _upf_param_value _upf_eval_dwarf_expr(const uint8_t *info) {
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
    } else if (opcode == DW_OP_implicit_pointer) {
        printf("[WARN] skipping DW_OP_implicit_pointer\n");
        param.is_const = 2;
        return param;
    } else if (opcode == DW_OP_entry_value) {
        uint64_t subexpr_len;
        size_t leb_len = _upf_uLEB_to_uint64(info, &subexpr_len);

        param = _upf_eval_dwarf_expr(info);
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
    } else {
        printf("0x%x\n", opcode);
        assert(0 && "TODO: handle other dwarf expressions");  // TODO:
    }

    return param;
skip:
    printf("[WARN] skipping dwarf expression: stack is not implemented\n");
    param.is_const = 2;
    return param;
}

static const char *_upf_get_string(const uint8_t *info, uint64_t form) {
    if (form == DW_FORM_strp || form == DW_FORM_line_strp) {
        const char *section = (char *) (form == DW_FORM_strp ? _upf_dwarf.str : _upf_dwarf.line_str);
        assert(section != NULL);
        uint64_t string_offset = _upf_dwarf.is_64bit ? (*((uint64_t *) info)) : ((uint64_t) (*((uint32_t *) info)));
        return section + string_offset;
    } else if (form == DW_FORM_string) {
        return (const char *) info;
    } else {
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

static uint64_t _upf_get_address(const uint8_t *info, uint64_t form) {
    if (form == DW_FORM_addr) {
        return _upf_get_offset(info);
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
            assert(0 && "Unreachable");
    }
}

static const uint8_t *_upf_skip_die(const uint8_t *info, const _upf_abbrev *abbrev) {
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        info += _upf_get_attr_size(info, abbrev->attrs.data[i].form);
    }

    return info;
}

static const uint8_t *_upf_get_param_value(const uint8_t *info, const _upf_abbrev *abbrev, _upf_param_value *value) {
    bool found = false;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_value) {
            assert(attr.form == DW_FORM_exprloc);
            *value = _upf_eval_dwarf_expr(info);
            found = true;
        }

        info += _upf_get_attr_size(info, attr.form);
    }
    if (!found) ERROR("unable to get call_site_parameter value.\n");

    return info;
}

// TODO: fully parse type, push it to _upf_type_map and return pointer into the map
static size_t _upf_get_type_helper(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrev_vec *abbrevs, _upf_type *type) {
    // TODO: FULLY rework caching

    const uint8_t *base = info;

    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    const _upf_abbrev *abbrev = &abbrevs->data[code - 1];

    if (abbrev->tag == DW_TAG_structure_type) {
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(type->name != NULL);  // TODO:

        while (1) {
            info += _upf_uLEB_to_uint64(info, &code);
            if (code == 0) break;

            assert(code <= abbrevs->length);
            abbrev = &abbrevs->data[code - 1];
            assert(abbrev->tag == DW_TAG_member);

            _upf_field field = {0};
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    field.name = _upf_get_string(info, attr.form);
                } else if (attr.name == DW_AT_type) {
                    _upf_get_type_helper(cu_base, cu_base + _upf_get_ref(info, attr.form), abbrevs, &field.type);
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
            else if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(info, attr.form);

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

            if (attr.name == DW_AT_type) next_info = cu_base + _upf_get_ref(info, attr.form);
            else if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(next_info != NULL);  // TODO:

        _upf_get_type_helper(cu_base, next_info, abbrevs, type);

        _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    if (abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type) {
        uint64_t offset = -1UL;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_type) offset = _upf_get_ref(info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }

        if (offset == -1UL) {
            assert(abbrev->tag == DW_TAG_pointer_type);
            type->type = _UPF_TT_VOID;
            type->name = "void";
        } else {
            _upf_get_type_helper(cu_base, cu_base + offset, abbrevs, type);
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

static void _upf_free_type(_upf_type type) {
    for (size_t i = 0; i < type.fields.length; i++) {
        _upf_free_type(type.fields.data[i].type);
    }
    VECTOR_FREE(&type.fields);
}

static size_t _upf_get_type(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrev_vec *abbrevs) {
    _upf_type type = {0};
    size_t type_idx = _upf_get_type_helper(cu_base, info, abbrevs, &type);
    if (type_idx == -1U) _upf_free_type(type);

    return type_idx;
}

// TODO: rename to include parameter?
static _upf_var_entry _upf_parse_variable(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrev *abbrev, uint64_t low_pc) {
    static const uint64_t INVALID_OFFSET = -1UL;

    _upf_var_entry var = {0};

    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_location) {
            if (attr.form == DW_FORM_exprloc) {
                _upf_param_value param = _upf_eval_dwarf_expr(info);
                if (param.is_const == 2) {
                    VECTOR_FREE(&var.locs);
                    return var;
                }
                assert(!param.is_const);
                VECTOR_PUSH(&var.locs, param.as.loc);
            } else if (attr.form == DW_FORM_sec_offset) {
                assert(_upf_dwarf.loclists != NULL);
                const uint8_t *loclist = _upf_dwarf.loclists + _upf_get_offset(info);

                uint8_t loclist_type = *loclist++;
                if (loclist_type == DW_LLE_base_address || loclist_type == DW_LLE_offset_pair) {
                    uint64_t base;
                    if (loclist_type == DW_LLE_base_address) {
                        base = _upf_get_offset(loclist);
                        loclist += _upf_dwarf.address_size;
                    } else {
                        loclist--;
                        assert(low_pc != INVALID_OFFSET);
                        base = low_pc;
                    }

                    uint64_t from, to;
                    while (*loclist != DW_LLE_end_of_list) {
                        assert(*loclist == DW_LLE_offset_pair);

                        loclist++;
                        loclist += _upf_uLEB_to_uint64(loclist, &from);
                        loclist += _upf_uLEB_to_uint64(loclist, &to);

                        _upf_param_value param = _upf_eval_dwarf_expr(loclist);
                        if (param.is_const != 2) {
                            assert(!param.is_const);

                            param.as.loc.is_scoped = true;
                            param.as.loc.from = base + from;
                            param.as.loc.to = base + to;

                            VECTOR_PUSH(&var.locs, param.as.loc);
                        }

                        // TODO: move this inside _upf_eval_dwarf_expr?
                        uint64_t expr_len;
                        loclist += _upf_uLEB_to_uint64(loclist, &expr_len);
                        loclist += expr_len;
                    }
                } else if (loclist_type == DW_LLE_start_length) {
                    uint64_t from = _upf_get_offset(loclist);
                    loclist += _upf_dwarf.address_size;

                    uint64_t len;
                    loclist += _upf_uLEB_to_uint64(loclist, &len);
                    uint64_t to = from + len;

                    _upf_param_value param = _upf_eval_dwarf_expr(loclist);
                    if (param.is_const == 2) {
                        VECTOR_FREE(&var.locs);
                        return var;
                    }
                    assert(!param.is_const);

                    param.as.loc.is_scoped = true;
                    param.as.loc.from = from;
                    param.as.loc.to = to;

                    VECTOR_PUSH(&var.locs, param.as.loc);
                } else {
                    printf("0x%x\n", loclist_type);
                    assert(0 && "TODO: handle other loclist types");  // TODO:
                }
            } else {
                assert(0 && "TODO: handle other forms of DW_AT_location in variable/parameter");
            }
        } else if (attr.name == DW_AT_type) {
            var.type_die = cu_base + _upf_get_ref(info, attr.form);
        }

        info += _upf_get_attr_size(info, attr.form);
    }

    return var;
}

static void _upf_parse_uprintf_call_site(const uint8_t *cu_base, const _upf_abbrev_vec *abbrevs, const _upf_call_site *call_site,
                                         const char *file_path, _upf_var_vec *vars) {
    static const uint64_t INVALID_OFFSET = -1UL;

    uint64_t code;
    _upf_param_value param;
    const _upf_abbrev *abbrev;

    const uint8_t *subprogram = call_site->subprogram;
    uint64_t low_pc = INVALID_OFFSET;
    while (1) {
        subprogram += _upf_uLEB_to_uint64(subprogram, &code);
        if (subprogram >= call_site->info) break;
        if (code == 0) continue;

        assert(code <= abbrevs->length);
        abbrev = &abbrevs->data[code - 1];

        if (abbrev->tag == DW_TAG_subprogram) {
            const uint8_t *info = subprogram;  // TODO: rename
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_low_pc) {
                    low_pc = _upf_get_address(info, attr.form);
                    break;
                }

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(low_pc != INVALID_OFFSET);
        } else if (abbrev->tag == DW_TAG_formal_parameter || abbrev->tag == DW_TAG_variable) {
            _upf_var_entry var = _upf_parse_variable(cu_base, subprogram, abbrev, low_pc);
            if (var.locs.length > 0) VECTOR_PUSH(vars, var);
        }

        subprogram = _upf_skip_die(subprogram, abbrev);
    }
    // TODO: save/cache results when there are multiple _upf_uprintf calls in the same function

    const uint8_t *info = call_site->info;

    // call site
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    abbrev = &abbrevs->data[code - 1];
    uint64_t return_pc = -1UL;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_return_pc) return_pc = _upf_get_address(info, attr.form);

        info += _upf_get_attr_size(info, attr.form);
    }
    assert(return_pc != -1UL);

    // file
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    info = _upf_skip_die(info, &abbrevs->data[code - 1]);

    // line
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    info = _upf_get_param_value(info, &abbrevs->data[code - 1], &param);
    assert(param.is_const);
    int64_t line = param.as.constant;
    assert(line >= 0 && "line number mustn't be negative");

    // counter
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    info = _upf_get_param_value(info, &abbrevs->data[code - 1], &param);
    assert(param.is_const);
    int64_t counter = param.as.constant;
    assert(counter >= 0 && "__COUNTER__ shouldn't be negative");

    // fmt
    info += _upf_uLEB_to_uint64(info, &code);
    assert(code > 0 && code <= abbrevs->length);
    info = _upf_skip_die(info, &abbrevs->data[code - 1]);

    // variadic arguments
    _upf_size_vec arg_types = {0};
    while (1) {
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) break;

        assert(code <= abbrevs->length);
        abbrev = &abbrevs->data[code - 1];
        assert(abbrev->tag == DW_TAG_call_site_parameter);

        info = _upf_get_param_value(info, abbrev, &param);
        if (param.is_const == 2) {
            fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%ld\n", file_path, line);
            VECTOR_FREE(&arg_types);
            return;
        }
        assert(!param.is_const);

        bool found = false;
        for (size_t i = 0; i < vars->length; i++) {
            _upf_var_entry var = vars->data[i];
            for (size_t j = 0; j < var.locs.length; j++) {
                _upf_var_loc loc = var.locs.data[j];
                if (loc.is_param == param.as.loc.is_param && loc.reg == param.as.loc.reg && loc.offset == param.as.loc.offset) {
                    if (loc.is_scoped) {
                        // TODO: return_pc isn't the variable we should be using although is close enough
                        assert(return_pc != INVALID_OFFSET);
                        if (!(loc.from < return_pc && return_pc <= loc.to)) continue;
                    }

                    size_t type_idx = _upf_get_type(cu_base, var.type_die, abbrevs);
                    if (type_idx != -1U) {
                        VECTOR_PUSH(&arg_types, type_idx);
                        found = true;
                    }
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%ld\n", file_path, line);
            VECTOR_FREE(&arg_types);
            return;
        }
    }

    _upf_arg_entry entry = {
        .file = file_path,
        .line = line,
        .counter = counter,
        .arg_types = arg_types,
    };

    VECTOR_PUSH(&_upf_args_map, entry);
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *info, const uint8_t *info_end, const uint8_t *abbrev_table) {
    static const uint64_t INVALID_OFFSET = -1UL;
    static const char *UPRINTF_FUNCTION_NAME = "_upf_uprintf";

    _upf_abbrev_vec abbrevs = _upf_parse_abbrevs(abbrev_table);
    _upf_call_site_vec call_sites = {0};
    _upf_var_vec vars = {0};
    uint64_t uprintf_offset = INVALID_OFFSET;
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

        if (abbrev->tag == DW_TAG_variable && depth == 1) {
            _upf_var_entry var = _upf_parse_variable(cu_base, info, abbrev, -1UL);
            if (var.locs.length > 0) VECTOR_PUSH(&vars, var);
        }

        if (abbrev->tag == DW_TAG_compile_unit) {
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) file_path = _upf_get_string(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
        } else if (abbrev->tag == DW_TAG_subprogram && uprintf_offset == INVALID_OFFSET) {
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) {
                    const char *function_name = _upf_get_string(info, attr.form);
                    if (strcmp(function_name, UPRINTF_FUNCTION_NAME) == 0) uprintf_offset = die_base - cu_base;
                }

                info += _upf_get_attr_size(info, attr.form);
            }
        } else if (abbrev->tag == DW_TAG_call_site) {
            uint64_t call_origin = -1UL;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_call_origin) call_origin = _upf_get_ref(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(call_origin != -1UL);

            _upf_call_site call_site = {
                .subprogram = subprogram,
                .info = die_base,
                .call_origin = call_origin,
            };
            VECTOR_PUSH(&call_sites, call_site);
        } else {
            info = _upf_skip_die(info, abbrev);
        }
    }
    assert(file_path != NULL && "compilation unit must have a path.");
    if (uprintf_offset == INVALID_OFFSET) goto exit;

    for (size_t i = 0; i < call_sites.length; i++) {
        _upf_call_site *call_site = &call_sites.data[i];
        if (call_site->call_origin == uprintf_offset) {
            size_t vars_length = vars.length;
            _upf_parse_uprintf_call_site(cu_base, &abbrevs, call_site, file_path, &vars);
            vars.length = vars_length;  // clear variables local to parsed subprogram
        }
    }

exit:
    VECTOR_FREE(&call_sites);
    for (size_t i = 0; i < abbrevs.length; i++) VECTOR_FREE(&abbrevs.data[i].attrs);
    VECTOR_FREE(&abbrevs);
    VECTOR_FREE(&vars);
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

        _upf_dwarf.address_size = *info;
        info += sizeof(_upf_dwarf.address_size);

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
    static const char *THIS_EXECUTABLE_PATH = "/proc/self/exe";


    struct stat file_info;
    if (stat(THIS_EXECUTABLE_PATH, &file_info) == -1) abort();
    size_t size = file_info.st_size;
    _upf_dwarf.file_size = size;

    int fd = open(THIS_EXECUTABLE_PATH, O_RDONLY);
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
            _upf_dwarf.str = file + section->sh_offset;
        } else if (strcmp(name, ".debug_line_str") == 0) {
            _upf_dwarf.line_str = file + section->sh_offset;
        } else if (strcmp(name, ".debug_loclists") == 0) {
            _upf_dwarf.loclists = file + section->sh_offset;
        }

        section++;
    }
    assert(_upf_dwarf.info != NULL && _upf_dwarf.abbrev != NULL);
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
                p += sprintf(p, "%p (\"", str);
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
    _upf_parse_elf();
    _upf_parse_dwarf();
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    munmap(_upf_dwarf.file, _upf_dwarf.file_size);
    VECTOR_FREE(&_upf_args_map);
    VECTOR_FREE(&_upf_type_map);
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
#undef VECTOR_RESET
#undef VECTOR_PUSH
#undef VECTOR_TYPEDEF
#undef INITIAL_VECTOR_CAPACITY
#undef OUT_OF_MEMORY
#undef ERROR

#endif  // UPRINTF_IMPLEMENTATION