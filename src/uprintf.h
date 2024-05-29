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
    const char *name;
};

VECTOR_TYPEDEF(_upf_var_vec, struct _upf_var_entry);

    // TODO:
    const char *name;
};

};


static struct _upf_dwarf _upf_dwarf = {0};


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
        if (len != 0) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = opcode - DW_OP_lit0;
    } else if (opcode == DW_OP_const1u) {
        if (len != 1) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((uint8_t *) info);
    } else if (opcode == DW_OP_const1s) {
        if (len != 1) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((int8_t *) info);
    } else if (opcode == DW_OP_const2u) {
        if (len != 2) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((uint16_t *) info);
    } else if (opcode == DW_OP_const2s) {
        if (len != 2) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((int16_t *) info);
    } else if (opcode == DW_OP_const4u) {
        if (len != 4) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((uint32_t *) info);
    } else if (opcode == DW_OP_const4s) {
        if (len != 4) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((int32_t *) info);
    } else if (opcode == DW_OP_const8u) {
        if (len != 8) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((uint64_t *) info);
    } else if (opcode == DW_OP_const8s) {
        if (len != 8) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
        param.is_const = 1;
        param.as.constant = *((int64_t *) info);
    } else if (opcode == DW_OP_constu) {
        param.is_const = 1;
        size_t leb_size = _upf_uLEB_to_uint64(info, (uint64_t *) &param.as.constant);
        if (len != leb_size) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
    } else if (opcode == DW_OP_consts) {
        param.is_const = 1;
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.constant);
        if (len != leb_size) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
    } else if (opcode == DW_OP_fbreg) {
        param.as.loc.reg = -1;  // TODO:
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
    } else if (DW_OP_breg0 <= opcode && opcode <= DW_OP_breg31) {
        param.as.loc.reg = opcode - DW_OP_breg0;  // TODO:
        size_t leb_size = _upf_LEB_to_int64(info, &param.as.loc.offset);
        if (len != leb_size) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
    } else if (DW_OP_reg0 <= opcode && opcode <= DW_OP_reg31) {
        param.as.loc.reg = opcode - DW_OP_reg0;  // TODO:
        if (len != 0) {
            printf("[WARN] skipping dwarf expression: stack is not implemented\n");
            param.is_const = 2;
            return param;
        }
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
    } else {
        printf("0x%x\n", opcode);
        assert(0 && "TODO: handle other dwarf expressions");  // TODO:
    }

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

static const char *_upf_get_type_name(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrevs *abbrevs) {
    uint64_t code;
    info += _upf_uLEB_to_uint64(info, &code);

    const struct _upf_abbrev *abbrev = &abbrevs->data[code - 1];
    for (size_t i = 0; i < abbrev->attrs.length; i++) {
        struct _upf_attr attr = abbrev->attrs.data[i];

        if (abbrev->tag == DW_TAG_pointer_type || abbrev->tag == DW_TAG_const_type) {
            if (attr.name == DW_AT_type) return _upf_get_type_name(cu_base, cu_base + _upf_get_ref(info, attr.form), abbrevs);
        } else if (abbrev->tag == DW_TAG_structure_type || abbrev->tag == DW_TAG_typedef || abbrev->tag == DW_TAG_base_type) {
            if (attr.name == DW_AT_name) return _upf_get_string(info, attr.form);
        }

        info += _upf_get_attr_size(info, attr.form);
    }

    return NULL;
}

// TODO: rename to include parameter?
static struct _upf_var_entry _upf_parse_variable(const uint8_t *cu_base, const uint8_t *info, const _upf_abbrevs *abbrevs,
                                                 const struct _upf_abbrev *abbrev, uint64_t low_pc) {
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
        } else if (attr.name == DW_AT_type) {
            // TODO: just save reference to base type
            var.name = _upf_get_type_name(cu_base, cu_base + _upf_get_ref(info, attr.form), abbrevs);
            if (var.name == NULL) {
                VECTOR_FREE(&var.locs);
                return var;
            }
        }

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
            struct _upf_var_entry var = _upf_parse_variable(cu_base, subprogram, abbrevs, abbrev, low_pc);
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

    printf("[INFO] %s:%lld:%lld:\n", file_path, line, counter);

    // variadic arguments
    while (1) {
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) break;

        assert(code <= abbrevs->length);
        abbrev = &abbrevs->data[code - 1];
        assert(abbrev->tag == DW_TAG_call_site_parameter);

        info = _upf_get_param_value(info, abbrev, &param);
        if (param.is_const == 2) continue;
        assert(param.is_const == 0);

        for (size_t i = 0; i < loc_to_type_map.length; i++) {
            uint8_t found = 0;
            struct _upf_var_entry var = loc_to_type_map.data[i];
            for (size_t j = 0; j < var.locs.length; j++) {
                struct _upf_var_loc loc = var.locs.data[j];
                if (loc.is_scoped == 0) {
                    if (loc.is_param == param.as.loc.is_param && loc.reg == param.as.loc.reg && loc.offset == param.as.loc.offset) {
                        printf("[INFO] variadic arg of type \"%s\"\n", var.name);
                        found = 1;
                        break;
                    }
                } else {
                    assert(return_pc != -1UL);
                    // TODO: return_pc isn't the variable we should be using although is close enough
                    if (loc.from < return_pc && return_pc <= loc.to) {
                        if (loc.is_param == param.as.loc.is_param && loc.reg == param.as.loc.reg && loc.offset == param.as.loc.offset) {
                            printf("[INFO] variadic arg of type \"%s\"\n", var.name);
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found) break;
        }
    }
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

    assert(_upf_dwarf.info != NULL && _upf_dwarf.abbrev != NULL && _upf_dwarf.str != NULL);
}


__attribute__((constructor)) void _upf_init(void) {
    _upf_parse_elf();
    _upf_parse_dwarf();
}

__attribute__((destructor)) void _upf_fini(void) {
    // Must be unloaded at the end of the program because many variables point
    // into the _upf_dwarf.file to avoid unnecessarily copying date.
    munmap(_upf_dwarf.file, _upf_dwarf.file_size);
}

void _upf_uprintf(const char *file, int line, int counter, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (const char *ch = fmt; *ch != '\0'; ch++) {
        if (*ch != '%') {
            // TODO: add to the buffer
            continue;
        }

        char next = *(ch + 1);

        if (next == '%') {
            // TODO: add % to the buffer
            ch++;
        } else if (next == 'S') {
            // void *s = va_arg(args, void *);
            // TODO: print struct ^
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