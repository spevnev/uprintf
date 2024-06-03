// For usage and readme see https://github.com/spevnev/uprintf

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

#ifndef __COUNTER__
#error ERROR: uprintf requires compiler/preprocessor to support "__COUNTER__" predefined macro
#endif


#ifndef UPRINTF_H
#define UPRINTF_H

#define uprintf(...) _upf_uprintf(__FILE__, __LINE__, __COUNTER__, __VA_ARGS__)

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


struct _upf_attr {
    uint64_t name;
    uint64_t form;
    int64_t implicit_const;
};

VECTOR_TYPEDEF(_upf_attrs, struct _upf_attr);

struct _upf_abbrev {
    uint64_t code;
    uint64_t tag;
    uint8_t has_children;
    _upf_attrs attrs;
};

VECTOR_TYPEDEF(_upf_abbrevs, struct _upf_abbrev);

struct _upf_call_site {
    const uint8_t *subprogram;  // pointer to DIE of parent subprogram
    const uint8_t *info;
    uint64_t call_origin;
};

VECTOR_TYPEDEF(_upf_call_sites, struct _upf_call_site);

struct _upf_dwarf {
    uint8_t *file;
    off_t file_size;

    uint8_t is_64bit;
    uint8_t offset_size;
    uint8_t address_size;

    const Elf64_Shdr *info;
    const Elf64_Shdr *abbrev;
    const Elf64_Shdr *str;
    const Elf64_Shdr *line_str;
    const Elf64_Shdr *loclists;
};

struct _upf_var_loc {
    // TODO: how to type reg? enum?
    int reg;
    int64_t offset;
    uint8_t is_scoped;
    uint64_t from;
    uint64_t to;
    uint8_t is_param;
};

// TODO: rename, union, change types
struct _upf_param_value {
    uint8_t is_const;
    union {
        int64_t constant;
        struct _upf_var_loc loc;
    } as;
};

VECTOR_TYPEDEF(_upf_var_loc_vec, struct _upf_var_loc);

// TODO: rename, change types
struct _upf_var_entry {
    _upf_var_loc_vec locs;
    const uint8_t *type_die;
};

VECTOR_TYPEDEF(_upf_var_vec, struct _upf_var_entry);

VECTOR_TYPEDEF(_upf_uint32_vec, uint32_t);

struct _upf_arg_entry {
    const char *file;
    int64_t line;
    int64_t counter;
    _upf_uint32_vec arg_types;
};

VECTOR_TYPEDEF(_upf_arg_vec, struct _upf_arg_entry);

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
    _UPF_TT_CH
};

struct _upf_type {
    const char *name;
    enum _upf_type_type type;
    _upf_field_vec fields;
    uint8_t is_pointer;
};

struct _upf_field {
    const char *name;
    struct _upf_type type;
    int offset;
};

struct _upf_type_entry {
    const uint8_t *type_die;
    struct _upf_type type;
};

VECTOR_TYPEDEF(_upf_types_vec, struct _upf_type_entry);

static struct _upf_dwarf _upf_dwarf = {0};
static _upf_arg_vec _upf_args_map = {0};    // TODO: rename
static _upf_types_vec _upf_type_map = {0};  // TODO: rename


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

static _upf_abbrevs _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _upf_abbrevs abbrevs = {0};

    while (1) {
        struct _upf_abbrev abbrev = {0};
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.code);
        if (abbrev.code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_children = *abbrev_table;
        abbrev_table += sizeof(uint8_t);

        while (1) {
            struct _upf_attr attr = {0};
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

static struct _upf_param_value _upf_eval_dwarf_expr(const uint8_t *info) {
    // TODO: rename struct, and variable to result
    struct _upf_param_value param = {0};

    uint64_t len;
    info += _upf_uLEB_to_uint64(info, &len);

    uint8_t opcode = *info++;
    len--;

    if (DW_OP_lit0 <= opcode && opcode <= DW_OP_lit31) {
        if (len != 0) goto skip;
        param.is_const = 1;
        param.as.constant = opcode - DW_OP_lit0;
    } else if (opcode == DW_OP_const1u) {
        if (len != 1) goto skip;
        param.is_const = 1;
        param.as.constant = *((uint8_t *) info);
    } else if (opcode == DW_OP_const1s) {
        if (len != 1) goto skip;
        param.is_const = 1;
        param.as.constant = *((int8_t *) info);
    } else if (opcode == DW_OP_const2u) {
        if (len != 2) goto skip;
        param.is_const = 1;
        param.as.constant = *((uint16_t *) info);
    } else if (opcode == DW_OP_const2s) {
        if (len != 2) goto skip;
        param.is_const = 1;
        param.as.constant = *((int16_t *) info);
    } else if (opcode == DW_OP_const4u) {
        if (len != 4) goto skip;
        param.is_const = 1;
        param.as.constant = *((uint32_t *) info);
    } else if (opcode == DW_OP_const4s) {
        if (len != 4) goto skip;
        param.is_const = 1;
        param.as.constant = *((int32_t *) info);
    } else if (opcode == DW_OP_const8u) {
        if (len != 8) goto skip;
        param.is_const = 1;
        param.as.constant = *((uint64_t *) info);
    } else if (opcode == DW_OP_const8s) {
        if (len != 8) goto skip;
        param.is_const = 1;
        param.as.constant = *((int64_t *) info);
    } else if (opcode == DW_OP_constu) {
        param.is_const = 1;
        size_t leb_size = _upf_uLEB_to_uint64(info, (uint64_t *) &param.as.constant);
        if (len != leb_size) goto skip;
    } else if (opcode == DW_OP_consts) {
        param.is_const = 1;
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.constant);
        if (len != leb_size) goto skip;
    } else if (opcode == DW_OP_fbreg) {
        param.as.loc.reg = -1;  // TODO:
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) goto skip;
    } else if (DW_OP_breg0 <= opcode && opcode <= DW_OP_breg31) {
        param.as.loc.reg = opcode - DW_OP_breg0;  // TODO:
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) goto skip;
    } else if (DW_OP_reg0 <= opcode && opcode <= DW_OP_reg31) {
        param.as.loc.reg = opcode - DW_OP_reg0;  // TODO:
        if (len != 0) goto skip;
    } else if (opcode == DW_OP_implicit_pointer) {
        printf("[WARN] skipping DW_OP_implicit_pointer\n");
        param.is_const = 2;
        return param;
    } else if (opcode == DW_OP_entry_value) {
        param = _upf_eval_dwarf_expr(info);
        if (param.is_const == 2) return param;
        assert(param.is_const == 0 && "TODO");  // TODO:
        param.as.loc.is_param = 1;
        // TODO: check that this is the last expression
    } else if (opcode == DW_OP_addr) {
        param.as.loc.reg = -2;  // TODO:
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
    uint64_t string_offset = _upf_dwarf.is_64bit ? (*((uint64_t *) info)) : ((uint64_t) (*((uint32_t *) info)));
    const char *base = (char *) _upf_dwarf.file;

    if (form == DW_FORM_strp || form == DW_FORM_line_strp) {
        uint64_t section_offset = form == DW_FORM_strp ? _upf_dwarf.str->sh_offset : _upf_dwarf.line_str->sh_offset;
        return base + section_offset + string_offset;
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

static int64_t _upf_get_data(const uint8_t *info, struct _upf_attr attr) {
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
        printf("0x%02x\n", attr.form);
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

static const uint8_t *_upf_skip_die(const uint8_t *info, const struct _upf_abbrev *abbrev) {
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        info += _upf_get_attr_size(info, abbrev->attrs.data[i].form);
    }

    return info;
}

static const uint8_t *_upf_parse_subprogram(const uint8_t *info, const struct _upf_abbrev *abbrev, uint8_t *is_uprintf) {
    static const char *UPRINTF_FUNCTION_NAME = "_upf_uprintf";


    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_name) {
            const char *function_name = _upf_get_string(info, attr.form);
            if (strcmp(UPRINTF_FUNCTION_NAME, function_name) == 0) *is_uprintf = 1;
        }

        info += _upf_get_attr_size(info, attr.form);
    }

    return info;
}

static const uint8_t *_upf_parse_call_site(const uint8_t *info, const struct _upf_abbrev *abbrev, uint64_t *offset) {
    static const uint64_t INVALID_OFFSET = -1U;


    *offset = INVALID_OFFSET;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_origin) *offset = _upf_get_ref(info, attr.form);

        info += _upf_get_attr_size(info, attr.form);
    }
    if (*offset == INVALID_OFFSET) ERROR("unable to find call_origin of call_site.\n");

    return info;
}

static const uint8_t *_upf_get_param_value(const uint8_t *info, const struct _upf_abbrev *abbrev, struct _upf_param_value *value) {
    uint8_t found = 0;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_value) {
            assert(attr.form == DW_FORM_exprloc);
            *value = _upf_eval_dwarf_expr(info);
            found = 1;
        }

        info += _upf_get_attr_size(info, attr.form);
    }
    if (!found) ERROR("unable to get call_site_parameter value.\n");

    return info;
}

// TODO: fully parse type, push it to _upf_type_map and return pointer into the map
static uint32_t _upf_get_type_helper(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrevs *abbrevs, struct _upf_type *type) {
    for (size_t i = 0; i < _upf_type_map.length; i++) {
        if (_upf_type_map.data[i].type_die == info) {
            *type = _upf_type_map.data[i].type;
            return i;
        }
    }

    const uint8_t *base = info;

    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);
    const struct _upf_abbrev *abbrev = &abbrevs->data[code - 1];

    if (abbrev->tag == DW_TAG_structure_type) {
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            struct _upf_attr attr = abbrev->attrs.data[i];

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

            struct _upf_field field = {0};
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                struct _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) field.name = _upf_get_string(info, attr.form);
                else if (attr.name == DW_AT_type)
                    _upf_get_type_helper(cu_base, cu_base + _upf_get_ref(info, attr.form), abbrevs, &field.type);
                else if (attr.name == DW_AT_data_member_location) field.offset = _upf_get_data(info, attr);

                info += _upf_get_attr_size(info, attr.form);
            }
            assert(field.name != NULL);
            VECTOR_PUSH(&type->fields, field);
        }

        struct _upf_type_entry entry = {
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
            struct _upf_attr attr = abbrev->attrs.data[i];

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
        } else {
            printf("0x%02x\n", encoding);
            assert(0 && "TODO: handle other base types");  // TODO:
        }

        // TODO: don't add base types to the map
        struct _upf_type_entry entry = {
            .type_die = base,
            .type = *type,
        };
        VECTOR_PUSH(&_upf_type_map, entry);

        return _upf_type_map.length - 1;
    }

    if (abbrev->tag == DW_TAG_typedef) {
        const uint8_t *next_info = NULL;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            struct _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_type) next_info = cu_base + _upf_get_ref(info, attr.form);
            else if (attr.name == DW_AT_name && type->name == NULL) type->name = _upf_get_string(info, attr.form);

            info += _upf_get_attr_size(info, attr.form);
        }
        assert(next_info != NULL);  // TODO:

        // TODO: only save struct & union types to map
        return _upf_get_type_helper(cu_base, next_info, abbrevs, type);
    }

    if (abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type) {
        // TODO: add this info to type?
        if (abbrev->tag == DW_TAG_pointer_type) type->is_pointer = 1;
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            struct _upf_attr attr = abbrev->attrs.data[i];

            if (attr.name == DW_AT_type) return _upf_get_type_helper(cu_base, cu_base + _upf_get_ref(info, attr.form), abbrevs, type);

            info += _upf_get_attr_size(info, attr.form);
        }
    }

    return -1U;
}

static void _upf_free_type(struct _upf_type type) {
    for (size_t i = 0; i < type.fields.length; i++) {
        _upf_free_type(type.fields.data[i].type);
    }
    VECTOR_FREE(&type.fields);
}

static uint32_t _upf_get_type(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrevs *abbrevs) {
    struct _upf_type type = {0};

    uint32_t type_idx = _upf_get_type_helper(cu_base, info, abbrevs, &type);
    if (type_idx == -1U) _upf_free_type(type);

    return type_idx;
}

// TODO: rename to include parameter?
static struct _upf_var_entry _upf_parse_variable(const uint8_t *cu_base, const uint8_t *info, const struct _upf_abbrev *abbrev,
                                                 uint64_t low_pc) {
    struct _upf_var_entry var = {0};

    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_location) {
            if (attr.form == DW_FORM_exprloc) {
                struct _upf_param_value param = _upf_eval_dwarf_expr(info);
                if (param.is_const == 2) {
                    VECTOR_FREE(&var.locs);
                    return var;
                }
                assert(param.is_const == 0);
                VECTOR_PUSH(&var.locs, param.as.loc);
            } else if (attr.form == DW_FORM_sec_offset) {
                const uint8_t *loclist = _upf_dwarf.file + _upf_dwarf.loclists->sh_offset + _upf_get_offset(info);

                uint8_t loclist_type = *loclist++;
                if (loclist_type == DW_LLE_base_address || loclist_type == DW_LLE_offset_pair) {
                    uint64_t base;
                    if (loclist_type == DW_LLE_base_address) {
                        base = _upf_get_offset(loclist);
                        loclist += _upf_dwarf.address_size;
                    } else {
                        assert(low_pc != -1UL);
                        base = low_pc;
                    }

                    uint64_t from, to;
                    while (*loclist != DW_LLE_end_of_list) {
                        assert(*loclist == DW_LLE_offset_pair);

                        loclist++;
                        loclist += _upf_uLEB_to_uint64(loclist, &from);
                        loclist += _upf_uLEB_to_uint64(loclist, &to);

                        struct _upf_param_value param = _upf_eval_dwarf_expr(loclist);
                        if (param.is_const != 2) {
                            assert(param.is_const == 0);

                            param.as.loc.is_scoped = 1;
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

                    struct _upf_param_value param = _upf_eval_dwarf_expr(loclist);
                    if (param.is_const == 2) {
                        VECTOR_FREE(&var.locs);
                        return var;
                    }
                    assert(param.is_const == 0);

                    param.as.loc.is_scoped = 1;
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
        } else if (attr.name == DW_AT_type) var.type_die = cu_base + _upf_get_ref(info, attr.form);

        info += _upf_get_attr_size(info, attr.form);
    }

    return var;
}

static void _upf_parse_uprintf_call_site(const uint8_t *cu_base, const _upf_abbrevs *abbrevs, const struct _upf_call_site *call_site,
                                         const char *file_path) {
    uint64_t code;
    struct _upf_param_value param;
    const struct _upf_abbrev *abbrev;

    _upf_var_vec loc_to_type_map = {0};  // TODO: rename
    const uint8_t *subprogram = call_site->subprogram;
    // TODO: parse global variables
    uint64_t low_pc = -1UL;
    while (1) {
        subprogram += _upf_uLEB_to_uint64(subprogram, &code);
        if (subprogram >= call_site->info) break;

        assert(code <= abbrevs->length);
        abbrev = &abbrevs->data[code - 1];

        if (abbrev->tag == DW_TAG_subprogram) {
            const uint8_t *_subprogram = subprogram;
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                struct _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_low_pc) {
                    low_pc = _upf_get_address(_subprogram, attr.form);
                    break;
                }

                _subprogram += _upf_get_attr_size(_subprogram, attr.form);
            }
        } else if (abbrev->tag == DW_TAG_formal_parameter || abbrev->tag == DW_TAG_variable) {
            struct _upf_var_entry var = _upf_parse_variable(cu_base, subprogram, abbrev, low_pc);
            if (var.locs.length > 0) VECTOR_PUSH(&loc_to_type_map, var);
        }

        subprogram = _upf_skip_die(subprogram, abbrev);
    }
    // TODO: save/cache results when there are multiple _upf_uprintf calls in the same function

    const uint8_t *info = call_site->info;

    // call site
    info += _upf_uLEB_to_uint64(info, &code);
    abbrev = &abbrevs->data[code - 1];
    uint64_t return_pc = -1UL;
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (attr.name == DW_AT_call_return_pc) return_pc = _upf_get_address(info, attr.form);

        info += _upf_get_attr_size(info, attr.form);
    }

    // file
    info += _upf_uLEB_to_uint64(info, &code);
    info = _upf_skip_die(info, &abbrevs->data[code - 1]);

    // line
    info += _upf_uLEB_to_uint64(info, &code);
    info = _upf_get_param_value(info, &abbrevs->data[code - 1], &param);
    assert(param.is_const == 1);
    int64_t line = param.as.constant;
    assert(line >= 0 && "line number mustn't be negative");

    // counter
    info += _upf_uLEB_to_uint64(info, &code);
    info = _upf_get_param_value(info, &abbrevs->data[code - 1], &param);
    assert(param.is_const == 1);
    int64_t counter = param.as.constant;
    assert(counter >= 0 && "__COUNTER__ shouldn't be negative");

    // fmt
    info += _upf_uLEB_to_uint64(info, &code);
    info = _upf_skip_die(info, &abbrevs->data[code - 1]);

    // variadic arguments
    _upf_uint32_vec arg_types = {0};
    while (1) {
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) break;

        assert(code <= abbrevs->length);
        abbrev = &abbrevs->data[code - 1];
        assert(abbrev->tag == DW_TAG_call_site_parameter);

        info = _upf_get_param_value(info, abbrev, &param);
        if (param.is_const == 2) {
            fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%lld\n", file_path, line);
            VECTOR_FREE(&arg_types);
            return;
        }
        assert(param.is_const == 0);

        uint8_t found = 0;
        for (size_t i = 0; i < loc_to_type_map.length; i++) {
            struct _upf_var_entry var = loc_to_type_map.data[i];
            for (size_t j = 0; j < var.locs.length; j++) {
                struct _upf_var_loc loc = var.locs.data[j];
                if (loc.is_param == param.as.loc.is_param && loc.reg == param.as.loc.reg && loc.offset == param.as.loc.offset) {
                    if (loc.is_scoped == 1) {
                        // TODO: return_pc isn't the variable we should be using although is close enough
                        assert(return_pc != -1UL);
                        if (!(loc.from < return_pc && return_pc <= loc.to)) continue;
                    }

                    uint32_t type_idx = _upf_get_type(cu_base, var.type_die, abbrevs);
                    if (type_idx != -1U) {
                        VECTOR_PUSH(&arg_types, type_idx);
                        found = 1;
                    }
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            fprintf(stderr, "[ERROR] Can't determine argument types for uprintf call at %s:%lld\n", file_path, line);
            VECTOR_FREE(&arg_types);
            return;
        }
    }

    struct _upf_arg_entry entry = {
        .file = file_path,
        .line = line,
        .counter = counter,
        .arg_types = arg_types,
    };

    VECTOR_PUSH(&_upf_args_map, entry);
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *info, const uint8_t *info_end, const uint8_t *abbrev_table) {
    static const uint64_t INVALID_OFFSET = -1UL;


    _upf_abbrevs abbrevs = _upf_parse_abbrevs(abbrev_table);
    _upf_call_sites call_sites = {0};
    uint64_t uprintf_offset = INVALID_OFFSET;
    const uint8_t *subprogram = NULL;
    const char *file_path = NULL;
    while (info < info_end) {
        const uint8_t *die_base = info;

        uint64_t code;
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) continue;

        assert(code <= abbrevs.length);
        const struct _upf_abbrev *abbrev = &abbrevs.data[code - 1];

        if (abbrev->tag == DW_TAG_subprogram) subprogram = die_base;

        if (abbrev->tag == DW_TAG_compile_unit) {
            for (size_t i = 0; i < abbrev->attrs.length; i++) {
                struct _upf_attr attr = abbrev->attrs.data[i];

                if (attr.name == DW_AT_name) file_path = _upf_get_string(info, attr.form);

                info += _upf_get_attr_size(info, attr.form);
            }
        } else if (abbrev->tag == DW_TAG_subprogram && uprintf_offset == INVALID_OFFSET) {
            uint8_t is_uprintf = 0;
            info = _upf_parse_subprogram(info, abbrev, &is_uprintf);
            if (is_uprintf) uprintf_offset = die_base - cu_base;
        } else if (abbrev->tag == DW_TAG_call_site) {
            uint64_t offset = 0;
            info = _upf_parse_call_site(info, abbrev, &offset);

            struct _upf_call_site call_site = {
                .subprogram = subprogram,
                .info = die_base,
                .call_origin = offset,
            };
            VECTOR_PUSH(&call_sites, call_site);
        } else {
            info = _upf_skip_die(info, abbrev);
        }
    }
    assert(file_path != NULL && "compilation unit must have a path.");
    assert(uprintf_offset != INVALID_OFFSET && "unable to find the offset of uprintf.");

    for (size_t i = 0; i < call_sites.length; i++) {
        struct _upf_call_site *call_site = &call_sites.data[i];
        if (call_site->call_origin == uprintf_offset) _upf_parse_uprintf_call_site(cu_base, &abbrevs, call_site, file_path);
    }

    VECTOR_FREE(&call_sites);
    for (size_t i = 0; i < abbrevs.length; i++) VECTOR_FREE(&abbrevs.data[i].attrs);
    VECTOR_FREE(&abbrevs);
}

static void _upf_parse_dwarf() {
    const uint8_t *abbrev = _upf_dwarf.file + _upf_dwarf.abbrev->sh_offset;
    const uint8_t *info = _upf_dwarf.file + _upf_dwarf.info->sh_offset;
    const uint8_t *info_end = info + _upf_dwarf.info->sh_size;

    while (info < info_end) {
        const uint8_t *cu_base = info;

        uint64_t length = *((uint32_t *) info);
        info += sizeof(uint32_t);

        _upf_dwarf.is_64bit = 0;
        if (length == 0xffffffffU) {
            length = *((uint64_t *) info);
            info += sizeof(uint64_t);
            _upf_dwarf.is_64bit = 1;
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

        _upf_parse_cu(cu_base, info, next, abbrev + abbrev_offset);

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

        if (strcmp(name, ".debug_info") == 0) _upf_dwarf.info = section;
        else if (strcmp(name, ".debug_abbrev") == 0) _upf_dwarf.abbrev = section;
        else if (strcmp(name, ".debug_str") == 0) _upf_dwarf.str = section;
        else if (strcmp(name, ".debug_line_str") == 0) _upf_dwarf.line_str = section;
        else if (strcmp(name, ".debug_loclists") == 0) _upf_dwarf.loclists = section;

        section++;
    }
    assert(_upf_dwarf.info != NULL && _upf_dwarf.abbrev != NULL && _upf_dwarf.str != NULL && _upf_dwarf.line_str != NULL
           && _upf_dwarf.loclists != NULL);
}

static void _upf_print_struct(void *data, const struct _upf_type type) {
    // TODO: handle pointers
    switch (type.type) {
        case _UPF_TT_STRUCT:
            printf("%s {\n", type.name);
            for (size_t i = 0; i < type.fields.length; i++) {
                struct _upf_field *field = &type.fields.data[i];

                printf("    %s %s=", field->type.name, field->name);
                _upf_print_struct(data + field->offset, field->type);
                printf("\n");
            }
            printf("};\n");
            break;
        case _UPF_TT_U1:
            printf("%u", *((uint8_t *) data));
            break;
        case _UPF_TT_U2:
            printf("%u", *((uint16_t *) data));
            break;
        case _UPF_TT_U4:
            printf("%u", *((uint32_t *) data));
            break;
        case _UPF_TT_U8:
            printf("%lu", *((uint64_t *) data));
            break;
        case _UPF_TT_S1:
            printf("%d", *((int8_t *) data));
            break;
        case _UPF_TT_S2:
            printf("%d", *((int16_t *) data));
            break;
        case _UPF_TT_S4:
            printf("%d", *((int32_t *) data));
            break;
        case _UPF_TT_S8:
            printf("%ld", *((int64_t *) data));
            break;
        case _UPF_TT_F4:
            printf("%f", *((float *) data));
            break;
        case _UPF_TT_F8:
            printf("%lf", *((double *) data));
            break;
        case _UPF_TT_CH:
            if (type.is_pointer) {
                // TODO: don't hardcode uint64 - address may be 32
                printf("\"%s\"", (char *) (*((uint64_t *) data)));
            } else {
                printf("'%c'", *((char *) data));
            }
            break;
    }
}


__attribute__((constructor)) void _upf_init(void) {
    _upf_parse_elf();
    _upf_parse_dwarf();
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    munmap(_upf_dwarf.file, _upf_dwarf.file_size);
    // TODO: cleanup global vectors
}

void _upf_uprintf(const char *file, int line, int counter, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // TODO: move to helper
    uint8_t found = 0;
    _upf_uint32_vec arg_types;
    for (size_t i = 0; i < _upf_args_map.length; i++) {
        struct _upf_arg_entry entry = _upf_args_map.data[i];
        if (entry.counter == counter && entry.line == line && strcmp(entry.file, file) == 0) {
            arg_types = entry.arg_types;
            found = 1;
            break;
        }
    }
    assert(found && "TODO");  // TODO: handle not being able to parse as either an error or warning

    int arg = 0;
    for (const char *ch = fmt; *ch != '\0'; ch++) {
        if (*ch != '%') {
            printf("%c", *ch);
            // TODO: add to the buffer
            continue;
        }

        char next = *(ch + 1);

        if (next == '%') {
            printf("%%");
            // TODO: add % to the buffer
            ch++;
        } else if (next == 'S') {
            if (arg >= arg_types.length) ERROR("more formats specified than arguments provided at %s:%d\n", file, line);
            // TODO: remove _upf_type_map => make arg_types contain pointers
            _upf_print_struct(va_arg(args, void *), _upf_type_map.data[arg_types.data[arg++]].type);
            // TODO: print to the buffer
            ch++;
        } else if (next == '\0' || next == '\n') {
            ERROR("unfinished format at the end of line at %s:%d\n", file, line);
        } else {
            ERROR("unkown format '%%%c' at %s:%d\n", next, file, line);
        }
    }

    va_end(args);
}


#undef VECTOR_FREE
#undef VECTOR_RESET
#undef VECTOR_PUSH
#undef VECTOR_TYPEDEF
#undef INITIAL_VECTOR_CAPACITY
#undef OUT_OF_MEMORY
#undef ERROR

#endif  // UPRINTF_IMPLEMENTATION