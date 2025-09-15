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

#ifndef __linux__
#error [ERROR] uprintf only supports Linux
#endif

// ====================== HEADER ==========================

#ifndef UPRINTF_H
#define UPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

void _upf_uprintf(const char *file, int line, const char *fmt, const char *args, ...);

#ifdef UPRINTF_TEST
extern int _upf_test_status;
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

// If variadic arguments are stringified directly, the macros will stringify to
// the macro name instead of being expanded. Passing them to another macro causes
// them to get expanded before stringification.
#define _upf_stringify_va_args(...) #__VA_ARGS__

// The noop instruction is required to keep the return PC within the scope of the
// caller function. Otherwise, it might be optimized to return outside of it.
#define uprintf(fmt, ...)                                                                        \
    do {                                                                                         \
        _upf_uprintf(__FILE__, __LINE__, fmt, _upf_stringify_va_args(__VA_ARGS__), __VA_ARGS__); \
        __asm__ volatile("nop");                                                                 \
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

#ifndef UPRINTF_MAX_STRING_LENGTH
#define UPRINTF_MAX_STRING_LENGTH 200
#endif

// ===================== INCLUDES =========================

#ifndef __USE_XOPEN_EXTENDED
#define __USE_XOPEN_EXTENDED
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
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

// Feature test macros might not work since the header could have already been
// included and expanded without them, so the functions must be declared here.

ssize_t readlink(const char *path, char *buf, size_t bufsiz);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// Partial redefinition of dl_phdr_info.
typedef struct {
    Elf64_Addr dlpi_addr;
    const char *dlpi_name;
} _upf_dl_phdr_info;

struct dl_phdr_info;
int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data);

// Linker-generated entry to the dynamic section of ELF.
extern Elf64_Dyn _DYNAMIC[];

// ===================== dwarf.h ==========================

// dwarf.h's location is inconsistent and the package containing it may not be
// installed, so the library includes a partial implementation.

#define DW_UT_compile 0x01

#define DW_TAG_array_type 0x01
#define DW_TAG_class_type 0x02
#define DW_TAG_enumeration_type 0x04
#define DW_TAG_formal_parameter 0x05
#define DW_TAG_lexical_block 0x0b
#define DW_TAG_member 0x0d
#define DW_TAG_pointer_type 0x0f
#define DW_TAG_reference_type 0x10
#define DW_TAG_compile_unit 0x11
#define DW_TAG_structure_type 0x13
#define DW_TAG_subroutine_type 0x15
#define DW_TAG_typedef 0x16
#define DW_TAG_union_type 0x17
#define DW_TAG_unspecified_parameters 0x18
#define DW_TAG_inheritance 0x1c
#define DW_TAG_inlined_subroutine 0x1d
#define DW_TAG_ptr_to_member_type 0x1f
#define DW_TAG_subrange_type 0x21
#define DW_TAG_base_type 0x24
#define DW_TAG_const_type 0x26
#define DW_TAG_enumerator 0x28
#define DW_TAG_subprogram 0x2e
#define DW_TAG_variable 0x34
#define DW_TAG_volatile_type 0x35
#define DW_TAG_restrict_type 0x37
#define DW_TAG_namespace 0x39
#define DW_TAG_imported_module 0x3a
#define DW_TAG_rvalue_reference_type 0x42
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
#define DW_AT_import 0x18
#define DW_AT_const_value 0x1c
#define DW_AT_upper_bound 0x2f
#define DW_AT_abstract_origin 0x31
#define DW_AT_count 0x37
#define DW_AT_data_member_location 0x38
#define DW_AT_declaration 0x3c
#define DW_AT_encoding 0x3e
#define DW_AT_external 0x3f
#define DW_AT_specification 0x47
#define DW_AT_type 0x49
#define DW_AT_ranges 0x55
#define DW_AT_data_bit_offset 0x6b
#define DW_AT_str_offsets_base 0x72
#define DW_AT_addr_base 0x73
#define DW_AT_rnglists_base 0x74
#define DW_AT_export_symbols 0x89

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
#define DW_LANG_C23 0x003e
#define DW_LANG_C_plus_plus 0x0004
#define DW_LANG_C_plus_plus_03 0x0019
#define DW_LANG_C_plus_plus_11 0x001a
#define DW_LANG_C_plus_plus_14 0x0021
#define DW_LANG_C_plus_plus_17 0x002a
#define DW_LANG_C_plus_plus_20 0x002b
#define DW_LANG_C_plus_plus_23 0x003a

// ===================== TESTING ==========================

#ifdef UPRINTF_TEST

#ifdef __cplusplus
extern "C" {
#endif

int _upf_test_status = EXIT_SUCCESS;

#ifdef __cplusplus
}  // extern "C"
#endif

#define _UPF_SET_TEST_STATUS(status) _upf_test_status = status

#else  // UPRINTF_TEST
#define _UPF_SET_TEST_STATUS(status) (void) status
#endif  // UPRINTF_TEST

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

#define _UPF_ERROR(...)                                  \
    do {                                                 \
        _UPF_LOG("ERROR", __VA_ARGS__);                  \
        _UPF_SET_TEST_STATUS(EXIT_FAILURE);              \
        longjmp(_upf_state.error_jmp_buf, EXIT_FAILURE); \
    } while (0)

#define _UPF_ASSERT(condition)                                                                        \
    do {                                                                                              \
        if (!(condition)) _UPF_ERROR("Assert (%s) failed at %s:%d.", #condition, __FILE__, __LINE__); \
    } while (0)

#define _UPF_OUT_OF_MEMORY() _UPF_ERROR("Process ran out of memory.")

#define _UPF_NO_DEBUG_INFO_ERROR "Ensure that the executable contains debugging information of at least 2nd level (-g2 or -g3)."

// ====================== VECTOR ==========================

#define _UPF_INITIAL_VECTOR_CAPACITY 4

#define _UPF_VECTOR_TYPEDEF(name, type) \
    typedef struct {                    \
        uint32_t capacity;              \
        uint32_t length;                \
        type *data;                     \
    } name

#define _UPF_VECTOR_PUSH(vec, element)                                  \
    do {                                                                \
        if ((vec)->capacity == 0) {                                     \
            (vec)->capacity = _UPF_INITIAL_VECTOR_CAPACITY;             \
            uint32_t size = (vec)->capacity * sizeof(*(vec)->data);     \
            *((void **) &(vec)->data) = _upf_alloc(size);               \
        } else if ((vec)->capacity == (vec)->length) {                  \
            uint32_t old_size = (vec)->capacity * sizeof(*(vec)->data); \
            (vec)->capacity *= 2;                                       \
            void *new_data = _upf_alloc(old_size * 2);                  \
            memcpy(new_data, (vec)->data, old_size);                    \
            *((void **) &(vec)->data) = new_data;                       \
        }                                                               \
        (vec)->data[(vec)->length++] = (element);                       \
    } while (0)

#define _UPF_VECTOR_TOP(vec) (vec)->data[(vec)->length - 1]

#define _UPF_VECTOR_UNORDERED_REMOVE(vec, index)   \
    do {                                           \
        _UPF_ASSERT(index < (vec)->length);        \
        (vec)->data[index] = _UPF_VECTOR_TOP(vec); \
        (vec)->length--;                           \
    } while (0)

// ================ C++ compatibility =====================

#ifdef __cplusplus
// clang-format off
#define _UPF_ZERO_INIT {}
// clang-format on
#else
#define _UPF_ZERO_INIT {0}
#endif

#ifdef __cplusplus
namespace uprintf {
#endif

// ====================== TYPES ===========================

_UPF_VECTOR_TYPEDEF(_upf_size_t_vec, size_t);
_UPF_VECTOR_TYPEDEF(_upf_cstr_vec, const char *);

typedef struct _upf_memory_region {
    struct _upf_memory_region *prev;
    size_t capacity;
    size_t length;
    uint8_t *data;
} _upf_memory_region;

typedef struct {
    uint64_t name;
    uint64_t form;
    int64_t implicit_const;
} _upf_attr;

_UPF_VECTOR_TYPEDEF(_upf_attr_vec, _upf_attr);

typedef struct {
    uint64_t tag;
    bool has_children;
    _upf_attr_vec attrs;
} _upf_abbrev;

_UPF_VECTOR_TYPEDEF(_upf_abbrev_vec, _upf_abbrev);

typedef struct _upf_type _upf_type;
_UPF_VECTOR_TYPEDEF(_upf_type_ptr_vec, struct _upf_type *);

typedef enum {
    _UPF_TK_STRUCT,
    _UPF_TK_UNION,
    _UPF_TK_ENUM,
    _UPF_TK_ARRAY,
    _UPF_TK_POINTER,
    _UPF_TK_REFERENCE,
    _UPF_TK_MEMBER_POINTER,
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
} _upf_type_kind;

typedef struct {
    const char *name;
    _upf_type *type;
    size_t offset;
    int bit_size;  // non-zero means bit field
} _upf_member;

_UPF_VECTOR_TYPEDEF(_upf_member_vec, _upf_member);

typedef struct {
    const uint8_t *die;
    const char *name;
} _upf_named_type;

_UPF_VECTOR_TYPEDEF(_upf_named_type_vec, _upf_named_type);

typedef struct {
    const char *name;
    const uint8_t *return_type_die;
    const uint8_t *specification_die;
    _upf_named_type_vec args;
    bool is_variadic;
    bool is_external;
    uint64_t pc;
} _upf_function;

_UPF_VECTOR_TYPEDEF(_upf_function_vec, _upf_function);

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
    _upf_type_kind kind;
    int modifiers;
    size_t size;
    union {
        struct {
            bool is_declaration;
            _upf_member_vec members;
            _upf_function_vec methods;
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
            _upf_type *type;
            bool is_rvalue;
        } reference;
        struct {
            _upf_type *return_type;
            _upf_type_ptr_vec arg_types;
            bool is_variadic;
            const _upf_function *function_ptr;
        } function;
    } as;
};

_UPF_VECTOR_TYPEDEF(_upf_type_vec, _upf_type *);

typedef struct {
    uint64_t start;
    uint64_t end;
} _upf_range;

_UPF_VECTOR_TYPEDEF(_upf_range_vec, _upf_range);

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

struct _upf_ns;
_UPF_VECTOR_TYPEDEF(_upf_ns_vec, struct _upf_ns *);

typedef struct {
    const char *name;
    struct _upf_ns *ns;
} _upf_named_ns;

_UPF_VECTOR_TYPEDEF(_upf_named_ns_vec, _upf_named_ns);

typedef struct _upf_ns {
    bool is_visited;
    _upf_named_type_vec vars;
    _upf_named_type_vec types;
    _upf_function_vec functions;
    _upf_ns_vec imported_nss;
    _upf_named_ns_vec sub_nss;
} _upf_ns;

struct _upf_scope;
_UPF_VECTOR_TYPEDEF(_upf_scope_vec, struct _upf_scope *);

typedef struct {
    const uint8_t *die;
    _upf_ns *ns;
} _upf_ns_entry;

_UPF_VECTOR_TYPEDEF(_upf_ns_map, _upf_ns_entry);

typedef struct _upf_scope {
    _upf_range_vec ranges;
    _upf_scope_vec scopes;
    _upf_named_type_vec vars;
    _upf_named_type_vec types;
    _upf_ns_vec nss;
} _upf_scope;

typedef struct {
    const uint8_t *die;
    _upf_type *type_ptr;
} _upf_type_map_entry;

_UPF_VECTOR_TYPEDEF(_upf_type_map_vec, _upf_type_map_entry);

typedef struct {
    const uint8_t *die;
    _upf_ns_vec *nss;
} _upf_ns_import;

_UPF_VECTOR_TYPEDEF(_upf_ns_import_vec, _upf_ns_import);

typedef struct {
    const uint8_t *die;
    _upf_ns_vec nss;
} _upf_declaration;

_UPF_VECTOR_TYPEDEF(_upf_declaration_vec, _upf_declaration);

typedef struct {
    const uint8_t *die;
    _upf_scope *scope;
} _upf_specification;

_UPF_VECTOR_TYPEDEF(_upf_specification_vec, _upf_specification);

typedef struct {
    const uint8_t *base;
    uint64_t base_address;
    uint64_t addr_base;
    uint64_t str_offsets_base;
    uint64_t rnglists_base;
    _upf_abbrev_vec abbrevs;
    _upf_scope scope;
    _upf_ns_import_vec ns_imports;
    _upf_ns_map nss;                     // TODO: rename/remove
    _upf_function_vec extern_functions;  // TODO: rename/explain/remove
    _upf_declaration_vec declared_functions;
    _upf_specification_vec specified_functions;
} _upf_cu;

_UPF_VECTOR_TYPEDEF(_upf_cu_vec, _upf_cu);

typedef enum {
    _UPF_TT_UNKNOWN,
    _UPF_TT_NUMBER,
    _UPF_TT_STRING,
    _UPF_TT_IDENTIFIER,
    _UPF_TT_STRUCT,
    _UPF_TT_ENUM,
    _UPF_TT_TYPE_SPECIFIER,
    _UPF_TT_TYPE_QUALIFIER,
    _UPF_TT_ATOMIC,
    _UPF_TT_GENERIC,
    _UPF_TT_SIZEOF,
    _UPF_TT_ALIGNOF,
    _UPF_TT_OPEN_PAREN,
    _UPF_TT_CLOSE_PAREN,
    _UPF_TT_OPEN_BRACKET,
    _UPF_TT_CLOSE_BRACKET,
    _UPF_TT_OPEN_BRACE,
    _UPF_TT_CLOSE_BRACE,
    _UPF_TT_COMMA,
    _UPF_TT_DOT,
    _UPF_TT_DOT_STAR,
    _UPF_TT_ARROW,
    _UPF_TT_ARROW_STAR,
    _UPF_TT_UNARY,
    _UPF_TT_INCREMENT,
    _UPF_TT_PLUS,
    _UPF_TT_MINUS,
    _UPF_TT_STAR,
    _UPF_TT_LESS_THAN,
    _UPF_TT_GREATER_THAN,
    _UPF_TT_LOGICAL,
    _UPF_TT_FACTOR,
    _UPF_TT_AMPERSAND,
    _UPF_TT_DOUBLE_AMPERSAND,
    _UPF_TT_QUESTION,
    _UPF_TT_COLON,
    _UPF_TT_ASSIGNMENT,
    _UPF_TT_CXX_CAST,
    _UPF_TT_CXX_NEW,
    _UPF_TT_CXX_SCOPE,

    _UPF_TT_COUNT
} _upf_token_type;

typedef struct {
    _upf_token_type type;
    const char *string;
} _upf_token;

_UPF_VECTOR_TYPEDEF(_upf_token_vec, _upf_token);

// https://en.cppreference.com/w/c/language/operator_precedence.html
typedef enum {
    _UPF_PREC_NONE = 0,
    _UPF_PREC_ASSIGNMENT,
    _UPF_PREC_TERNARY,
    _UPF_PREC_LOGICAL,
    _UPF_PREC_TERM,
    _UPF_PREC_FACTOR,
    _UPF_PREC_UNARY,
    _UPF_PREC_POSTFIX
} _upf_parse_precedence;

typedef struct {
    _upf_type *(*prefix)(void);
    _upf_type *(*infix)(_upf_type *);
    _upf_parse_precedence precedence;
} _upf_parse_rule;

typedef struct {
    int pointers;
    bool reference;
    bool failed;
    _upf_type *function_type;
    _upf_type **function_return_type;
} _upf_abstract_declarator;

// =================== GLOBAL STATE =======================

struct _upf_state {
    _upf_memory_region *allocator;
    // has _upf_init finished
    bool is_init;
    // file loaded by dynamic linker (without debug info)
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
    _upf_type_vec types;
    _upf_cu_vec cus;
    _upf_extern_function_vec extern_functions;
    // sequential id for handling circular structs
    int circular_id;
    // valid memory regions
    _upf_range_vec addresses;
    // bprintf
    char *buffer;
    char *ptr;
    uint32_t size;
    uint32_t free;
    // error handling
    jmp_buf error_jmp_buf;
    const char *file_path;
    int line;
    // tokenizer
    _upf_token_vec tokens;
    uint32_t tokens_idx;
    // parser
    _upf_cu *current_cu;
    _upf_scope_vec current_scopes;
};

static struct _upf_state _upf_state = _UPF_ZERO_INIT;

// ==================== ALLOCATOR =========================

#define _UPF_INITIAL_REGION_SIZE 65535  // 64kb

static _upf_memory_region *_upf_allocator_new_region(size_t capacity, _upf_memory_region *prev) {
    _upf_memory_region *region = (_upf_memory_region *) malloc(sizeof(*region) + capacity * sizeof(*region->data));
    if (region == NULL) _UPF_OUT_OF_MEMORY();
    region->capacity = capacity;
    region->length = 0;
    region->prev = prev;
    region->data = (uint8_t *) region + sizeof(*region);
    return region;
}

static void *_upf_alloc(size_t size) {
    if (_upf_state.allocator == NULL) {
        size_t capacity = _UPF_INITIAL_REGION_SIZE;
        if (size > capacity) capacity = size;
        _upf_state.allocator = _upf_allocator_new_region(capacity, NULL);
    }

    int alignment = _upf_state.allocator->length % sizeof(void *);
    if (alignment > 0) alignment = sizeof(void *) - alignment;

    if (alignment + size > _upf_state.allocator->capacity - _upf_state.allocator->length) {
        size_t new_capacity = _upf_state.allocator->capacity * 2;
        if (size > new_capacity) new_capacity = size;
        _upf_state.allocator = _upf_allocator_new_region(new_capacity, _upf_state.allocator);
        alignment = 0;
    }

    void *memory = _upf_state.allocator->data + _upf_state.allocator->length + alignment;
    _upf_state.allocator->length += alignment + size;
    return memory;
}

static void _upf_free_allocator(void) {
    _upf_memory_region *current = _upf_state.allocator;
    while (current != NULL) {
        _upf_memory_region *prev = current->prev;
        free(current);
        current = prev;
    }
}

static char *_upf_new_string(const char *begin, const char *end) {
    _UPF_ASSERT(begin != NULL && end != NULL && end > begin);

    size_t len = end - begin;
    char *string = (char *) _upf_alloc(len + 1);
    memcpy(string, begin, len);
    string[len] = '\0';

    return string;
}

// ====================== DWARF ===========================

// Converts unsigned LEB128 to uint64_t and returns the size of LEB in bytes.
static size_t _upf_uLEB_to_uint64(const uint8_t *leb, uint64_t *result) {
    _UPF_ASSERT(leb != NULL && result != NULL);

    static const uint8_t BITS_MASK = 0x7f;      // 01111111
    static const uint8_t CONTINUE_MASK = 0x80;  // 10000000

    int i = 0;
    uint8_t b = 0;
    int shift = 0;
    *result = 0;
    do {
        b = leb[i++];
        _UPF_ASSERT(shift <= 56 || (shift == 63 && b == 1));
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        shift += 7;
    } while (b & CONTINUE_MASK);
    return i;
}

// Converts signed LEB128 to int64_t and returns the size of LEB in bytes.
static size_t _upf_LEB_to_int64(const uint8_t *leb, int64_t *result) {
    _UPF_ASSERT(leb != NULL && result != NULL);

    static const uint8_t BITS_MASK = 0x7f;      // 01111111
    static const uint8_t CONTINUE_MASK = 0x80;  // 10000000
    static const uint8_t SIGN_MASK = 0x40;      // 01000000

    int i = 0;
    uint8_t b = 0;
    int shift = 0;
    *result = 0;
    do {
        b = leb[i++];
        _UPF_ASSERT(shift <= 56 || (shift == 63 && (b == 1 || b == BITS_MASK)));
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        shift += 7;
    } while (b & CONTINUE_MASK);
    if ((shift < 64) && (b & SIGN_MASK)) *result |= -(1LL << shift);
    return i;
}

static bool _upf_is_primitive(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);

    switch (type->kind) {
        case _UPF_TK_STRUCT:
        case _UPF_TK_UNION:
        case _UPF_TK_ARRAY:          return false;
        case _UPF_TK_FUNCTION:
        case _UPF_TK_ENUM:
        case _UPF_TK_POINTER:
        case _UPF_TK_REFERENCE:
        case _UPF_TK_MEMBER_POINTER:
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
        case _UPF_TK_UNKNOWN:        return true;
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
        case DW_FORM_addr:     return _upf_state.address_size;
        case DW_FORM_strx1:
        case DW_FORM_addrx1:
        case DW_FORM_flag:
        case DW_FORM_ref1:
        case DW_FORM_data1:    return 1;
        case DW_FORM_strx2:
        case DW_FORM_addrx2:
        case DW_FORM_ref2:
        case DW_FORM_data2:    return 2;
        case DW_FORM_strx3:
        case DW_FORM_addrx3:   return 3;
        case DW_FORM_ref_sup4:
        case DW_FORM_strx4:
        case DW_FORM_addrx4:
        case DW_FORM_ref4:
        case DW_FORM_data4:    return 4;
        case DW_FORM_ref_sig8:
        case DW_FORM_ref_sup8:
        case DW_FORM_ref8:
        case DW_FORM_data8:    return 8;
        case DW_FORM_data16:   return 16;
        case DW_FORM_block1:   {
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
        case DW_FORM_udata:     {
            uint64_t result;
            return _upf_uLEB_to_uint64(die, &result);
        } break;
        case DW_FORM_string:  return strlen((const char *) die) + 1;
        case DW_FORM_exprloc:
        case DW_FORM_block:   {
            uint64_t length;
            size_t leb_size = _upf_uLEB_to_uint64(die, &length);
            return leb_size + length;
        } break;
        case DW_FORM_line_strp:
        case DW_FORM_strp_sup:
        case DW_FORM_sec_offset:
        case DW_FORM_ref_addr:
        case DW_FORM_strp:       return _upf_state.offset_size;
        case DW_FORM_indirect:   {
            uint64_t form;
            size_t offset = _upf_uLEB_to_uint64(die, &form);
            return _upf_get_attr_size(die + offset, form);
        } break;
        case DW_FORM_flag_present:
        case DW_FORM_implicit_const: return 0;
    }
    _UPF_ERROR("Invalid DWARF attribute type(form): %lu.", form);
}

static uint64_t _upf_get_x_offset(const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(die != NULL);

    uint64_t offset = 0;
    switch (form) {
        case DW_FORM_strx1:
        case DW_FORM_addrx1: memcpy(&offset, die, 1); return offset;
        case DW_FORM_strx2:
        case DW_FORM_addrx2: memcpy(&offset, die, 2); return offset;
        case DW_FORM_strx3:
        case DW_FORM_addrx3: memcpy(&offset, die, 3); return offset;
        case DW_FORM_strx4:
        case DW_FORM_addrx4: memcpy(&offset, die, 4); return offset;
        case DW_FORM_addrx:
        case DW_FORM_strx:   _upf_uLEB_to_uint64(die, &offset); return offset;
    }
    _UPF_ERROR("Invalid DWARF x-* type(form): %lu.", form);
}

static const char *_upf_get_str(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    switch (form) {
        case DW_FORM_strp:      return _upf_state.str + _upf_offset_cast(die);
        case DW_FORM_line_strp: _UPF_ASSERT(_upf_state.line_str != NULL); return _upf_state.line_str + _upf_offset_cast(die);
        case DW_FORM_string:    return (const char *) die;
        case DW_FORM_strx:
        case DW_FORM_strx1:
        case DW_FORM_strx2:
        case DW_FORM_strx3:
        case DW_FORM_strx4:     {
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
        case DW_FORM_ref8:      memcpy(&ref, die, _upf_get_attr_size(die, form)); return ref;
        case DW_FORM_ref_udata: _upf_uLEB_to_uint64(die, &ref); return ref;
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
        case DW_FORM_udata:          return true;
        default:                     return false;
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
        case DW_FORM_data16:         _UPF_ERROR("16 byte data blocks aren't supported.");
        case DW_FORM_implicit_const: return attr.implicit_const;
        case DW_FORM_sdata:          {
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
        case DW_FORM_addrx4: return true;
        default:             return false;
    }
}

static uint64_t _upf_get_addr(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    switch (form) {
        case DW_FORM_addr:   return _upf_address_cast(die);
        case DW_FORM_addrx1:
        case DW_FORM_addrx2:
        case DW_FORM_addrx3:
        case DW_FORM_addrx4:
        case DW_FORM_addrx:  {
            _UPF_ASSERT(_upf_state.addr != NULL);
            uint64_t offset = cu->addr_base + _upf_get_x_offset(die, form) * _upf_state.address_size;
            return _upf_address_cast(_upf_state.addr + offset);
        }
    }
    _UPF_ERROR("Invalid DWARF address type(form): %lu.", form);
}

static bool _upf_get_flag(const uint8_t *die, uint64_t form) {
    if (form == DW_FORM_flag_present) return true;
    if (form == DW_FORM_flag) return *die;
    _UPF_ERROR("Invalid DWARF flag type(form): %lu.", form);
}

static size_t _upf_get_abbrev(const _upf_abbrev **abbrev, const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(abbrev != NULL && cu != NULL);

    uint64_t code;
    size_t offset = _upf_uLEB_to_uint64(die, &code);
    *abbrev = code == 0 ? NULL : &cu->abbrevs.data[code - 1];
    return offset;
}

static const uint8_t *_upf_skip_attrs(const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(die != NULL && abbrev != NULL);

    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        die += _upf_get_attr_size(die, abbrev->attrs.data[i].form);
    }

    return die;
}

static const uint8_t *_upf_skip_die(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    die = _upf_skip_attrs(die, abbrev);
    if (!abbrev->has_children) return die;

    int depth = 1;
    while (depth > 0) {
        die += _upf_get_abbrev(&abbrev, cu, die);
        if (abbrev == NULL) {
            depth--;
        } else {
            if (abbrev->has_children) depth++;
            die = _upf_skip_attrs(die, abbrev);
        }
    }
    return die;
}

static _upf_abbrev_vec _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _UPF_ASSERT(abbrev_table != NULL);

    uint64_t code;
    _upf_abbrev_vec abbrevs = _UPF_ZERO_INIT;
    while (true) {
        _upf_abbrev abbrev = _UPF_ZERO_INIT;
        abbrev.tag = UINT64_MAX;

        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &code);
        if (code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_children = *abbrev_table;
        abbrev_table += sizeof(abbrev.has_children);

        while (true) {
            _upf_attr attr = _UPF_ZERO_INIT;
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.name);
            abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &attr.form);
            if (attr.form == DW_FORM_implicit_const) abbrev_table += _upf_LEB_to_int64(abbrev_table, &attr.implicit_const);
            if (attr.name == 0 && attr.form == 0) break;
            _UPF_VECTOR_PUSH(&abbrev.attrs, attr);
        }

        _UPF_VECTOR_PUSH(&abbrevs, abbrev);
        _UPF_ASSERT(code == abbrevs.length);
    }

    return abbrevs;
}

static _upf_type_kind _upf_get_type_kind(int64_t encoding, int64_t size) {
    switch (encoding) {
        case DW_ATE_boolean:
            if (size == 1) return _UPF_TK_BOOL;
            _UPF_WARN("Expected boolean to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_address: _UPF_WARN("Segmented addresses aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_signed:
            switch (size) {
                case 1:  return _UPF_TK_S1;
                case 2:  return _UPF_TK_S2;
                case 4:  return _UPF_TK_S4;
                case 8:  return _UPF_TK_S8;
                case 16: _UPF_WARN("16 byte integers aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
                default: _UPF_WARN("Expected integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type."); return _UPF_TK_UNKNOWN;
            }
        case DW_ATE_signed_char:
            if (size == 1) return _UPF_TK_SCHAR;
            _UPF_WARN("Expected char to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_unsigned:
            switch (size) {
                case 1:  return _UPF_TK_U1;
                case 2:  return _UPF_TK_U2;
                case 4:  return _UPF_TK_U4;
                case 8:  return _UPF_TK_U8;
                case 16: _UPF_WARN("16 byte unsigned integers aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
                default: _UPF_WARN("Expected integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type."); return _UPF_TK_UNKNOWN;
            }
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
        case DW_ATE_unsigned_fixed: _UPF_WARN("C shouldn't have fixed-point decimals. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_float:
            switch (size) {
                case 4:  return _UPF_TK_F4;
                case 8:  return _UPF_TK_F8;
                case 16: _UPF_WARN("16 byte floats aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
                default: _UPF_WARN("Expected floats to be 4, 8 or 16 bytes long. Ignoring this type."); return _UPF_TK_UNKNOWN;
            }
        case DW_ATE_complex_float:   _UPF_WARN("Complex floats aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_imaginary_float: _UPF_WARN("Imaginary floats aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_decimal_float:   _UPF_WARN("Decimal floats aren't supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_packed_decimal:  _UPF_WARN("C shouldn't have packed decimals. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_numeric_string:  _UPF_WARN("C shouldn't have numeric strings. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_edited:          _UPF_WARN("C shouldn't have edited strings. Ignoring this type."); return _UPF_TK_UNKNOWN;
        default:                     _UPF_WARN("Skipping unknown DWARF type encoding (0x%02lx)", encoding); return _UPF_TK_UNKNOWN;
    }
}

static int _upf_get_type_modifier(uint64_t tag) {
    switch (tag) {
        case DW_TAG_const_type:    return _UPF_MOD_CONST;
        case DW_TAG_volatile_type: return _UPF_MOD_VOLATILE;
        case DW_TAG_restrict_type: return _UPF_MOD_RESTRICT;
        case DW_TAG_atomic_type:   return _UPF_MOD_ATOMIC;
    }
    _UPF_ERROR("Invalid DWARF type modifier: %lu.", tag);
}

static _upf_type *_upf_new_type(_upf_type type) {
    _upf_type *type_ptr = (_upf_type *) _upf_alloc(sizeof(type));
    memcpy(type_ptr, &type, sizeof(type));
    _UPF_VECTOR_PUSH(&_upf_state.types, type_ptr);
    return type_ptr;
}

static _upf_type *_upf_new_type2(const uint8_t *type_die, _upf_type type) {
    _UPF_ASSERT(type_die != NULL);

    for (uint32_t i = 0; i < _upf_state.type_map.length; i++) {
        _upf_type_map_entry entry = _upf_state.type_map.data[i];
        if (entry.die == type_die) return entry.type_ptr;
    }

    _upf_type *type_ptr = (_upf_type *) _upf_alloc(sizeof(type));
    memcpy(type_ptr, &type, sizeof(type));

    _upf_type_map_entry entry = _UPF_ZERO_INIT;
    entry.die = type_die;
    entry.type_ptr = type_ptr;

    _UPF_VECTOR_PUSH(&_upf_state.type_map, entry);

    return type_ptr;
}

static _upf_type _upf_get_subarray(const _upf_type *array, int count) {
    _UPF_ASSERT(array != NULL);

    _upf_type subarray = *array;
    if (array->name != NULL) subarray.name = _upf_new_string(array->name, array->name + strlen(array->name) - 2 * count);
    subarray.as.array.lengths.length -= count;
    subarray.as.array.lengths.data += count;

    return subarray;
}

static _upf_type *_upf_get_pointer_to_type(_upf_type *type_ptr) {
    _UPF_ASSERT(type_ptr != NULL);
    _upf_type type = _UPF_ZERO_INIT;
    type.kind = _UPF_TK_POINTER;
    type.size = sizeof(void *);
    type.as.pointer.type = type_ptr;
    return _upf_new_type(type);
}

static _upf_type *_upf_get_reference_to_type(_upf_type *type_ptr) {
    _upf_type type = _UPF_ZERO_INIT;
    type.kind = _UPF_TK_REFERENCE;
    type.as.reference.type = type_ptr;
    return _upf_new_type(type);
}

static bool _upf_is_pointer(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);
    if (type->kind == _UPF_TK_POINTER || type->kind == _UPF_TK_ARRAY) return true;
    return false;
}

static _upf_type *_upf_dereference_type(_upf_type *type_ptr) {
    _UPF_ASSERT(_upf_is_pointer(type_ptr));
    if (type_ptr->kind == _UPF_TK_POINTER) return type_ptr->as.pointer.type;
    if (type_ptr->as.array.lengths.length <= 1) return type_ptr->as.array.element_type;
    return _upf_new_type(_upf_get_subarray(type_ptr, 1));
}

static _upf_type *_upf_get_void_type(void) {
    static _upf_type *type_ptr = NULL;
    if (type_ptr == NULL) {
        _upf_type type = _UPF_ZERO_INIT;
        type.name = "void";
        type.kind = _UPF_TK_VOID;
        type_ptr = _upf_new_type(type);
    }
    return type_ptr;
}

static _upf_type *_upf_get_bool_type(void) {
    static _upf_type *type_ptr = NULL;
    if (type_ptr == NULL) {
        _upf_type type = _UPF_ZERO_INIT;
        type.name = "bool";
        type.kind = _UPF_TK_BOOL;
        type.size = sizeof(bool);
        type_ptr = _upf_new_type(type);
    }
    return type_ptr;
}

static _upf_type *_upf_get_cstr_type(void) {
    static _upf_type *type_ptr = NULL;
    if (type_ptr == NULL) {
        _upf_type type = _UPF_ZERO_INIT;
        type.name = "char";
        type.kind = _UPF_TK_SCHAR;
        type.modifiers = _UPF_MOD_CONST;
        type.size = sizeof(char);
        type_ptr = _upf_get_pointer_to_type(_upf_new_type(type));
    }
    return type_ptr;
}

static _upf_type *_upf_get_number_type(void) {
    static _upf_type *type_ptr = NULL;
    if (type_ptr == NULL) {
        _upf_type type = _UPF_ZERO_INIT;
        type.name = "int";
        type.kind = _UPF_TK_S4;
        type.size = sizeof(int);
        type_ptr = _upf_new_type(type);
    }
    return type_ptr;
}

static bool _upf_is_in_range(uint64_t addr, _upf_range_vec ranges) {
    for (uint32_t i = 0; i < ranges.length; i++) {
        if (ranges.data[i].start <= addr && addr < ranges.data[i].end) return true;
    }
    return false;
}

static _upf_range_vec _upf_get_ranges(const _upf_cu *cu, const uint8_t *die, uint64_t form) {
    _UPF_ASSERT(cu != NULL && die != NULL && _upf_state.rnglists != NULL);

    const uint8_t *rnglist = NULL;
    if (form == DW_FORM_sec_offset) {
        rnglist = _upf_state.rnglists + _upf_offset_cast(die);
    } else {
        _UPF_ASSERT(form == DW_FORM_rnglistx && cu->rnglists_base != UINT64_MAX);
        uint64_t index;
        _upf_uLEB_to_uint64(die, &index);
        uint64_t rnglist_offset = _upf_offset_cast(_upf_state.rnglists + cu->rnglists_base + index * _upf_state.offset_size);
        rnglist = _upf_state.rnglists + cu->rnglists_base + rnglist_offset;
    }
    _UPF_ASSERT(rnglist != NULL);

    uint64_t base = cu->base_address;
    _upf_range_vec ranges = _UPF_ZERO_INIT;
    while (*rnglist != DW_RLE_end_of_list) {
        switch (*rnglist++) {
            case DW_RLE_base_addressx:
                base = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                break;
            case DW_RLE_startx_endx: {
                _upf_range range = _UPF_ZERO_INIT;

                range.start = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                range.end = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_startx_length: {
                uint64_t address = _upf_get_addr(cu, rnglist, DW_FORM_addrx);
                rnglist += _upf_get_attr_size(rnglist, DW_FORM_addrx);
                uint64_t length;
                rnglist += _upf_uLEB_to_uint64(rnglist, &length);

                _upf_range range = _UPF_ZERO_INIT;
                range.start = address;
                range.end = address + length;
                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_offset_pair: {
                uint64_t start, end;
                rnglist += _upf_uLEB_to_uint64(rnglist, &start);
                rnglist += _upf_uLEB_to_uint64(rnglist, &end);

                _upf_range range = _UPF_ZERO_INIT;
                range.start = base + start;
                range.end = base + end;
                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_base_address:
                base = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                break;
            case DW_RLE_start_end: {
                _upf_range range = _UPF_ZERO_INIT;

                range.start = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                range.end = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;

                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            case DW_RLE_start_length: {
                uint64_t address = _upf_address_cast(rnglist);
                rnglist += _upf_state.address_size;
                uint64_t length;
                rnglist += _upf_uLEB_to_uint64(rnglist, &length);

                _upf_range range = _UPF_ZERO_INIT;
                range.start = address;
                range.end = address + length;
                _UPF_VECTOR_PUSH(&ranges, range);
            } break;
            default: _UPF_WARN("Found unsupported range list entry kind (0x%x). Skipping it.", *(rnglist - 1)); return ranges;
        }
    }
    return ranges;
}

static _upf_range_vec _upf_get_die_ranges(const _upf_cu *cu, const uint8_t *low_pc_die, _upf_attr low_pc_attr, const uint8_t *high_pc_die,
                                          _upf_attr high_pc_attr, const uint8_t *ranges_die, _upf_attr ranges_attr) {
    _UPF_ASSERT(cu != NULL);

    if (ranges_die != NULL) return _upf_get_ranges(cu, ranges_die, ranges_attr.form);

    _upf_range_vec ranges = _UPF_ZERO_INIT;
    if (low_pc_die != NULL && high_pc_die != NULL) {
        _upf_range range = _UPF_ZERO_INIT;
        range.start = _upf_get_addr(cu, low_pc_die, low_pc_attr.form);
        if (_upf_is_addr(high_pc_attr.form)) {
            range.end = _upf_get_addr(cu, high_pc_die, high_pc_attr.form);
        } else {
            range.end = range.start + _upf_get_data(high_pc_die, high_pc_attr);
        }

        _UPF_VECTOR_PUSH(&ranges, range);
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
        case DW_LANG_C23:
        case DW_LANG_C_plus_plus:
        case DW_LANG_C_plus_plus_03:
        case DW_LANG_C_plus_plus_11:
        case DW_LANG_C_plus_plus_14:
        case DW_LANG_C_plus_plus_17:
        case DW_LANG_C_plus_plus_20:
        case DW_LANG_C_plus_plus_23: return true;
        default:                     return false;
    }
}

static _upf_named_type _upf_parse_variable(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL);

    _upf_named_type var = _UPF_ZERO_INIT;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:            var.name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_type:            var.die = cu->base + _upf_get_ref(die, attr.form); break;
            case DW_AT_abstract_origin: {
                const uint8_t *new_die = cu->base + _upf_get_ref(die, attr.form);
                const _upf_abbrev *new_abbrev;
                new_die += _upf_get_abbrev(&new_abbrev, cu, new_die);

                return _upf_parse_variable(cu, new_die, new_abbrev);
            }
        }
        die += _upf_get_attr_size(die, attr.form);
    }
    return var;
}

static const char *_upf_get_type_name(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        if (attr.name == DW_AT_name) return _upf_get_str(cu, die, attr.form);
        die += _upf_get_attr_size(die, attr.form);
    }
    return NULL;
}

static _upf_function _upf_parse_subprogram(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    _upf_function function = _UPF_ZERO_INIT;
    function.pc = UINT64_MAX;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:   function.name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_type:   function.return_type_die = cu->base + _upf_get_ref(die, attr.form); break;
            case DW_AT_low_pc: function.pc = _upf_get_addr(cu, die, attr.form); break;
            case DW_AT_ranges: {
                _upf_range_vec ranges = _upf_get_ranges(cu, die, attr.form);
                _UPF_ASSERT(ranges.length > 0);
                function.pc = ranges.data[0].start;
            } break;
            case DW_AT_abstract_origin: {
                const uint8_t *new_die = cu->base + _upf_get_ref(die, attr.form);
                const _upf_abbrev *new_abbrev;
                new_die += _upf_get_abbrev(&new_abbrev, cu, new_die);

                _upf_function origin_function = _upf_parse_subprogram(cu, new_die, new_abbrev);
                origin_function.pc = function.pc;
                function = origin_function;
            } break;
            case DW_AT_specification: function.specification_die = cu->base + _upf_get_ref(die, attr.form); break;
            case DW_AT_external:      function.is_external = _upf_get_flag(die, attr.form); break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    if (abbrev->has_children && function.args.length == 0) {
        while (true) {
            die += _upf_get_abbrev(&abbrev, cu, die);
            if (abbrev == NULL) break;
            if (abbrev->tag == DW_TAG_unspecified_parameters) {
                function.is_variadic = true;
                break;
            }
            if (abbrev->tag != DW_TAG_formal_parameter) break;

            _upf_named_type arg = _UPF_ZERO_INIT;
            for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                _upf_attr attr = abbrev->attrs.data[i];
                switch (attr.name) {
                    case DW_AT_name: arg.name = _upf_get_str(cu, die, attr.form); break;
                    case DW_AT_type: arg.die = cu->base + _upf_get_ref(die, attr.form); break;
                }
                die += _upf_get_attr_size(die, attr.form);
            }
            _UPF_VECTOR_PUSH(&function.args, arg);
        }
    }

    return function;
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
    bool is_declaration = false;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:        name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_type:        subtype_offset = _upf_get_ref(die, attr.form); break;
            case DW_AT_encoding:    encoding = _upf_get_data(die, attr); break;
            case DW_AT_declaration: is_declaration = _upf_get_flag(die, attr.form); break;
            case DW_AT_byte_size:
                if (_upf_is_data(attr.form)) {
                    size = _upf_get_data(die, attr);
                } else {
                    _UPF_WARN("Non-constant type sizes aren't supported. Marking size as unknown.");
                }
                break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    switch (abbrev->tag) {
        case DW_TAG_array_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);

            _upf_type *element_type = _upf_parse_type(cu, cu->base + subtype_offset);
            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _UPF_TK_ARRAY;
            type.size = size;
            type.as.array.element_type = element_type;

            bool generate_name = element_type->name != NULL && type.name == NULL;
            int brackets = 0;
            bool is_static = true;
            size_t array_size = element_type->size;
            if (!abbrev->has_children) return _upf_new_type2(die_base, type);
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
                    switch (attr.name) {
                        case DW_AT_count:
                        case DW_AT_upper_bound:
                            if (_upf_is_data(attr.form)) {
                                length = _upf_get_data(die, attr);
                                if (attr.name == DW_AT_upper_bound) length++;
                            } else {
                                _UPF_WARN("Non-constant array lengths aren't supported. Marking it unknown.");
                            }
                            break;
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

                if (generate_name) brackets++;
            }

            if (element_type->size != UINT64_MAX && type.size == UINT64_MAX) {
                type.size = array_size;
            }

            if (generate_name && brackets > 0) {
                size_t element_name_len = strlen(element_type->name);
                char *type_name = (char *) _upf_alloc(element_name_len + 2 * brackets + 1);
                memcpy(type_name, element_type->name, element_name_len);

                char *c = type_name + element_name_len;
                while (brackets-- > 0) {
                    *c++ = '[';
                    *c++ = ']';
                }
                *c = '\0';

                type.name = type_name;
            }

            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_enumeration_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);

            _upf_type type = _UPF_ZERO_INIT;
            type.name = name ? name : "enum";
            type.kind = _UPF_TK_ENUM;
            type.as.cenum.underlying_type = _upf_parse_type(cu, cu->base + subtype_offset);
            type.size = size == UINT64_MAX ? type.as.cenum.underlying_type->size : size;

            if (!abbrev->has_children) return _upf_new_type2(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                _UPF_ASSERT(abbrev->tag == DW_TAG_enumerator);

                bool found_value = false;
                _upf_enum cenum = _UPF_ZERO_INIT;
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];
                    switch (attr.name) {
                        case DW_AT_name: cenum.name = _upf_get_str(cu, die, attr.form); break;
                        case DW_AT_const_value:
                            if (!_upf_is_data(attr.form)) {
                                _UPF_WARN("Non-constant enum values aren't supported. Ignoring this type.");
                                goto unknown_type;
                            }
                            cenum.value = _upf_get_data(die, attr);
                            found_value = true;
                            break;
                    }
                    die += _upf_get_attr_size(die, attr.form);
                }
                _UPF_ASSERT(cenum.name != NULL && found_value);

                _UPF_VECTOR_PUSH(&type.as.cenum.enums, cenum);
            }

            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_pointer_type: {
            _UPF_ASSERT(size == UINT64_MAX || size == sizeof(void *));

            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _UPF_TK_POINTER;
            type.size = sizeof(void *);

            // Pointers must be added before parsing their data to prevent
            // self-referential structs from infinite recursion.
            _upf_type *type_ptr = _upf_new_type2(die_base, type);

            if (subtype_offset == UINT64_MAX) {
                type_ptr->as.pointer.type = _upf_get_void_type();
            } else {
                type_ptr->as.pointer.type = _upf_parse_type(cu, cu->base + subtype_offset);
            }
            type_ptr->name = type_ptr->as.pointer.type->name;

            return type_ptr;
        }
        case DW_TAG_reference_type:
        case DW_TAG_rvalue_reference_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);

            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _UPF_TK_REFERENCE;
            type.size = 0;
            type.as.reference.type = _upf_parse_type(cu, cu->base + subtype_offset);
            type.as.reference.is_rvalue = abbrev->tag == DW_TAG_rvalue_reference_type;

            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_structure_type:
        case DW_TAG_class_type:     {
            _upf_type type = _UPF_ZERO_INIT;
            type.name = name ? name : "struct";
            type.kind = _UPF_TK_STRUCT;
            type.size = size;
            type.as.cstruct.is_declaration = is_declaration;

            if (!abbrev->has_children) return _upf_new_type2(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;

                switch (abbrev->tag) {
                    case DW_TAG_member: {
                        _upf_member member = _UPF_ZERO_INIT;
                        member.offset = UINT64_MAX;

                        bool add_member = true;
                        for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                            _upf_attr attr = abbrev->attrs.data[i];
                            switch (attr.name) {
                                case DW_AT_name:            member.name = _upf_get_str(cu, die, attr.form); break;
                                case DW_AT_type:            member.type = _upf_parse_type(cu, cu->base + _upf_get_ref(die, attr.form)); break;
                                case DW_AT_data_bit_offset: member.offset = _upf_get_data(die, attr); break;
                                case DW_AT_data_member_location:
                                    if (_upf_is_data(attr.form)) {
                                        member.offset = _upf_get_data(die, attr);
                                    } else {
                                        _UPF_WARN("Non-constant member offsets aren't supported. Skipping this field.");
                                        add_member = false;
                                    }
                                    break;
                                case DW_AT_bit_offset:
                                    _UPF_WARN("DW_AT_bit_offset is deprecated in DWARF v5. Skipping this field.");
                                    add_member = false;
                                    break;
                                case DW_AT_bit_size:
                                    if (_upf_is_data(attr.form)) {
                                        member.bit_size = _upf_get_data(die, attr);
                                    } else {
                                        _UPF_WARN("Non-constant bit field sizes aren't supported. Skipping this field.");
                                        add_member = false;
                                    }
                                    break;
                            }
                            die += _upf_get_attr_size(die, attr.form);
                        }
                        if (add_member) {
                            _UPF_ASSERT(member.type != NULL && member.offset != UINT64_MAX);
                            if (member.name == NULL) member.name = "<anonymous>";
                            _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
                        }
                    } break;
                    case DW_TAG_subprogram:
                        _UPF_VECTOR_PUSH(&type.as.cstruct.methods, _upf_parse_subprogram(cu, die, abbrev));
                        die = _upf_skip_die(cu, die, abbrev);
                        break;
                    case DW_TAG_inheritance: {
                        _upf_type *parent_type = NULL;
                        int64_t offset = -1;
                        bool add_members = true;
                        for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                            _upf_attr attr = abbrev->attrs.data[i];
                            switch (attr.name) {
                                case DW_AT_type: parent_type = _upf_parse_type(cu, cu->base + _upf_get_ref(die, attr.form)); break;
                                case DW_AT_data_member_location:
                                    if (_upf_is_data(attr.form)) {
                                        offset = _upf_get_data(die, attr);
                                    } else {
                                        _UPF_WARN("Non-constant member offsets aren't supported. Skipping this field.");
                                        add_members = false;
                                    }
                                    break;
                            }
                            die += _upf_get_attr_size(die, attr.form);
                        }
                        if (add_members) {
                            _UPF_ASSERT(parent_type != NULL && parent_type->kind == _UPF_TK_STRUCT && offset != -1);
                            for (uint32_t i = 0; i < parent_type->as.cstruct.members.length; i++) {
                                _upf_member member = parent_type->as.cstruct.members.data[i];
                                member.offset += offset;
                                _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
                            }
                        }
                    } break;
                    default: die = _upf_skip_die(cu, die, abbrev); break;
                }
            }
            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_union_type: {
            _upf_type type = _UPF_ZERO_INIT;
            type.name = name ? name : "union";
            type.kind = _UPF_TK_UNION;
            type.size = size;
            type.as.cstruct.is_declaration = is_declaration;

            if (!abbrev->has_children) return _upf_new_type2(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                if (abbrev->tag != DW_TAG_member) {
                    die = _upf_skip_die(cu, die, abbrev);
                    continue;
                }

                _upf_member member = _UPF_ZERO_INIT;
                bool add_member = true;
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];
                    switch (attr.name) {
                        case DW_AT_name: member.name = _upf_get_str(cu, die, attr.form); break;
                        case DW_AT_type: member.type = _upf_parse_type(cu, cu->base + _upf_get_ref(die, attr.form)); break;
                        case DW_AT_bit_size:
                            if (_upf_is_data(attr.form)) {
                                member.bit_size = _upf_get_data(die, attr);
                            } else {
                                _UPF_WARN("Non-constant bit field sizes aren't supported. Skipping this field.");
                                add_member = false;
                            }
                            break;
                    }
                    die += _upf_get_attr_size(die, attr.form);
                }
                if (add_member) {
                    _UPF_ASSERT(member.type != NULL);
                    if (member.name == NULL) member.name = "<anonymous>";
                    _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
                }
            }
            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_subroutine_type: {
            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _UPF_TK_FUNCTION;
            type.size = size;

            if (subtype_offset == UINT64_MAX) {
                type.as.function.return_type = _upf_get_void_type();
            } else {
                type.as.function.return_type = _upf_parse_type(cu, cu->base + subtype_offset);
            }

            if (!abbrev->has_children) return _upf_new_type2(die_base, type);
            while (true) {
                die += _upf_get_abbrev(&abbrev, cu, die);
                if (abbrev == NULL) break;
                if (abbrev->tag == DW_TAG_unspecified_parameters) {
                    type.as.function.is_variadic = true;
                    break;
                }
                if (abbrev->tag != DW_TAG_formal_parameter) {
                    die = _upf_skip_die(cu, die, abbrev);
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
            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_ptr_to_member_type: {
            _UPF_ASSERT(subtype_offset != UINT64_MAX);
            _upf_type type = _UPF_ZERO_INIT;
            type.kind = _UPF_TK_MEMBER_POINTER;
            type.as.pointer.type = _upf_parse_type(cu, cu->base + subtype_offset);
            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_typedef: {
            _UPF_ASSERT(name != NULL);

            if (subtype_offset == UINT64_MAX) {
                _upf_type type = _UPF_ZERO_INIT;
                type.name = name;
                type.kind = _UPF_TK_VOID;
                return _upf_new_type2(die_base, type);
            }

            _upf_type type = *_upf_parse_type(cu, cu->base + subtype_offset);
            type.name = name;

            if (type.kind == _UPF_TK_SCHAR && strcmp(name, "int8_t") == 0) type.kind = _UPF_TK_S1;
            else if (type.kind == _UPF_TK_UCHAR && strcmp(name, "uint8_t") == 0) type.kind = _UPF_TK_U1;

            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_base_type: {
            _UPF_ASSERT(name != NULL && size != UINT64_MAX && encoding != 0);

            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _upf_get_type_kind(encoding, size);
            type.size = size;

            return _upf_new_type2(die_base, type);
        }
        case DW_TAG_const_type:
        case DW_TAG_volatile_type:
        case DW_TAG_restrict_type:
        case DW_TAG_atomic_type:   {
            if (subtype_offset == UINT64_MAX) {
                _upf_type type = _UPF_ZERO_INIT;
                type.name = "void";
                type.kind = _UPF_TK_VOID;
                type.modifiers = _upf_get_type_modifier(abbrev->tag);
                type.size = sizeof(void *);

                return _upf_new_type2(die_base, type);
            } else {
                _upf_type type = *_upf_parse_type(cu, cu->base + subtype_offset);
                type.modifiers |= _upf_get_type_modifier(abbrev->tag);

                return _upf_new_type2(die_base, type);
            }
        }
        default: _UPF_WARN("Found unsupported type (0x%lx). Ignoring it.", abbrev->tag); break;
    }

    _upf_type type;
unknown_type:
    memset(&type, 0, sizeof(type));
    type.name = name;
    type.kind = _UPF_TK_UNKNOWN;
    type.size = size;
    return _upf_new_type2(die_base, type);
}

static _upf_type *_upf_get_function_type(const _upf_function *function) {
    _UPF_ASSERT(function != NULL);
    _upf_type type = _UPF_ZERO_INIT;
    type.name = function->name;
    type.kind = _UPF_TK_FUNCTION;
    type.size = sizeof(void *);
    type.as.function.function_ptr = function;
    if (function->return_type_die == NULL) {
        type.as.function.return_type = _upf_get_void_type();
    } else {
        type.as.function.return_type = _upf_parse_type(_upf_state.current_cu, function->return_type_die);
    }
    return _upf_new_type(type);
}

static _upf_ns *_upf_parse_ns_body(_upf_cu *cu, const uint8_t *die, _upf_ns_vec *ns_stack);

static void _upf_parse_import(_upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev, _upf_ns_vec *nss) {
    _UPF_ASSERT(!abbrev->has_children);

    _upf_ns_import import = _UPF_ZERO_INIT;
    import.nss = nss;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        if (attr.name == DW_AT_import) import.die = cu->base + _upf_get_ref(die, attr.form);
        die += _upf_get_attr_size(die, attr.form);
    }
    _UPF_ASSERT(import.die != NULL);

    _UPF_VECTOR_PUSH(&cu->ns_imports, import);
}

static void _upf_parse_sub_namespace(_upf_cu *cu, const uint8_t *die, _upf_ns *ns, _upf_ns_vec *ns_stack) {
    const uint8_t *die_base = die;

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, cu, die);
    _UPF_ASSERT(abbrev != NULL && abbrev->tag == DW_TAG_namespace);

    const char *name = NULL;
    bool export_symbols = false;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:           name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_export_symbols: export_symbols = _upf_get_flag(die, attr.form); break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    _upf_ns *sub_ns = abbrev->has_children ? _upf_parse_ns_body(cu, die, ns_stack) : NULL;

    if (name == NULL || export_symbols) _UPF_VECTOR_PUSH(&ns->imported_nss, sub_ns);

    if (name != NULL) {
        _upf_named_ns entry = _UPF_ZERO_INIT;
        entry.name = name;
        entry.ns = sub_ns;
        _UPF_VECTOR_PUSH(&ns->sub_nss, entry);
    }

    _upf_ns_entry ns_entry = _UPF_ZERO_INIT;
    ns_entry.die = die_base;
    ns_entry.ns = sub_ns;
    _UPF_VECTOR_PUSH(&cu->nss, ns_entry);
}

static _upf_scope *_upf_parse_scope(_upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    const uint8_t *low_pc_die = NULL;
    _upf_attr low_pc_attr = _UPF_ZERO_INIT;
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = _UPF_ZERO_INIT;
    const uint8_t *ranges_die = NULL;
    _upf_attr ranges_attr = _UPF_ZERO_INIT;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_low_pc:
                low_pc_attr = attr;
                low_pc_die = die;
                break;
            case DW_AT_high_pc:
                high_pc_attr = attr;
                high_pc_die = die;
                break;
            case DW_AT_ranges:
                ranges_attr = attr;
                ranges_die = die;
                break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    _upf_range_vec ranges = _upf_get_die_ranges(cu, low_pc_die, low_pc_attr, high_pc_die, high_pc_attr, ranges_die, ranges_attr);
    if (ranges.length == 0) return NULL;

    _upf_scope *scope = (_upf_scope *) _upf_alloc(sizeof(*scope));
    memset(scope, 0, sizeof(*scope));
    scope->ranges = ranges;

    if (!abbrev->has_children) return scope;

    while (true) {
        const uint8_t *die_base = die;
        die += _upf_get_abbrev(&abbrev, cu, die);
        if (abbrev == NULL) break;

        switch (abbrev->tag) {
            case DW_TAG_variable:
            case DW_TAG_formal_parameter: {
                _upf_named_type var = _upf_parse_variable(cu, die, abbrev);
                if (var.name == NULL) break;
                _UPF_ASSERT(var.die != NULL);
                _UPF_VECTOR_PUSH(&scope->vars, var);
            } break;
            case DW_TAG_array_type:
            case DW_TAG_class_type:
            case DW_TAG_enumeration_type:
            case DW_TAG_pointer_type:
            case DW_TAG_reference_type:
            case DW_TAG_structure_type:
            case DW_TAG_typedef:
            case DW_TAG_union_type:
            case DW_TAG_ptr_to_member_type:
            case DW_TAG_subroutine_type:
            case DW_TAG_const_type:
            case DW_TAG_volatile_type:
            case DW_TAG_restrict_type:
            case DW_TAG_atomic_type:
            case DW_TAG_base_type:
            case DW_TAG_rvalue_reference_type: {
                const char *type_name = _upf_get_type_name(cu, die, abbrev);
                if (type_name != NULL) {
                    _upf_named_type type = _UPF_ZERO_INIT;
                    type.die = die_base;
                    type.name = type_name;
                    _UPF_VECTOR_PUSH(&scope->types, type);
                }
            } break;
            case DW_TAG_lexical_block:
            case DW_TAG_inlined_subroutine: {
                _upf_scope *sub_scope = _upf_parse_scope(cu, die, abbrev);
                if (sub_scope != NULL) _UPF_VECTOR_PUSH(&scope->scopes, sub_scope);
            } break;
            case DW_TAG_imported_module: _upf_parse_import(cu, die, abbrev, &scope->nss); break;
        }
        die = _upf_skip_die(cu, die, abbrev);
    }
    return scope;
}

static _upf_ns *_upf_parse_ns_body(_upf_cu *cu, const uint8_t *die, _upf_ns_vec *ns_stack) {
    _upf_ns *ns = (_upf_ns *) _upf_alloc(sizeof(*ns));
    memset(ns, 0, sizeof(*ns));
    _UPF_VECTOR_PUSH(ns_stack, ns);

    const _upf_abbrev *abbrev;
    while (true) {
        const uint8_t *die_base = die;
        die += _upf_get_abbrev(&abbrev, cu, die);
        if (abbrev == NULL) break;

        switch (abbrev->tag) {
            case DW_TAG_member:
            case DW_TAG_variable: {
                _upf_named_type var = _upf_parse_variable(cu, die, abbrev);
                if (var.name == NULL) break;
                _UPF_ASSERT(var.die != NULL);
                _UPF_VECTOR_PUSH(&ns->vars, var);
            } break;
            case DW_TAG_class_type:
            case DW_TAG_structure_type:
            case DW_TAG_union_type:     {
                const char *type_name = _upf_get_type_name(cu, die, abbrev);
                if (type_name != NULL) {
                    if (abbrev->has_children) {
                        _upf_ns *class_ns = _upf_parse_ns_body(cu, _upf_skip_attrs(die, abbrev), ns_stack);
                        _upf_named_ns entry = _UPF_ZERO_INIT;
                        entry.name = type_name;
                        entry.ns = class_ns;
                        _UPF_VECTOR_PUSH(&ns->sub_nss, entry);
                    }

                    _upf_named_type type = _UPF_ZERO_INIT;
                    type.die = die_base;
                    type.name = type_name;
                    _UPF_VECTOR_PUSH(&ns->types, type);
                }
            } break;
            case DW_TAG_array_type:
            case DW_TAG_enumeration_type:
            case DW_TAG_pointer_type:
            case DW_TAG_reference_type:
            case DW_TAG_typedef:
            case DW_TAG_ptr_to_member_type:
            case DW_TAG_subroutine_type:
            case DW_TAG_const_type:
            case DW_TAG_volatile_type:
            case DW_TAG_restrict_type:
            case DW_TAG_atomic_type:
            case DW_TAG_base_type:
            case DW_TAG_rvalue_reference_type: {
                const char *type_name = _upf_get_type_name(cu, die, abbrev);
                if (type_name != NULL) {
                    _upf_named_type type = _UPF_ZERO_INIT;
                    type.die = die_base;
                    type.name = type_name;
                    _UPF_VECTOR_PUSH(&ns->types, type);
                }
            } break;
            case DW_TAG_subprogram: {
                _upf_function function = _upf_parse_subprogram(cu, die, abbrev);
                if (function.name != NULL) {
                    _UPF_VECTOR_PUSH(&ns->functions, function);
                    if (function.is_external) _UPF_VECTOR_PUSH(&cu->extern_functions, function);
                }

                _upf_scope *scope = _upf_parse_scope(cu, die, abbrev);
                if (scope == NULL) {
                    for (uint32_t i = 0; i < cu->specified_functions.length; i++) {
                        if (cu->specified_functions.data[i].die == die_base) {
                            scope = cu->specified_functions.data[i].scope;
                            _UPF_VECTOR_UNORDERED_REMOVE(&cu->specified_functions, i);
                            break;
                        }
                    }
                    if (scope == NULL) {
                        _upf_declaration declaration = _UPF_ZERO_INIT;
                        declaration.die = die_base;
                        for (uint32_t i = 0; i < ns_stack->length; i++) {
                            _UPF_VECTOR_PUSH(&declaration.nss, ns_stack->data[ns_stack->length - 1 - i]);
                        }
                        _UPF_VECTOR_PUSH(&cu->declared_functions, declaration);
                    } else {
                        for (uint32_t i = 0; i < ns_stack->length; i++) {
                            _UPF_VECTOR_PUSH(&scope->nss, ns_stack->data[ns_stack->length - 1 - i]);
                        }
                        _UPF_VECTOR_PUSH(&cu->scope.scopes, scope);
                    }
                } else {
                    if (function.specification_die == NULL) {
                        for (uint32_t i = 0; i < ns_stack->length; i++) {
                            _UPF_VECTOR_PUSH(&scope->nss, ns_stack->data[ns_stack->length - 1 - i]);
                        }
                        _UPF_VECTOR_PUSH(&cu->scope.scopes, scope);
                    } else {
                        bool found = false;
                        for (uint32_t i = 0; i < cu->declared_functions.length; i++) {
                            if (cu->declared_functions.data[i].die == function.specification_die) {
                                scope->nss = cu->declared_functions.data[i].nss;
                                _UPF_VECTOR_PUSH(&cu->scope.scopes, scope);
                                _UPF_VECTOR_UNORDERED_REMOVE(&cu->declared_functions, i);
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            _upf_specification specification = _UPF_ZERO_INIT;
                            specification.die = die_base;
                            specification.scope = scope;
                            _UPF_VECTOR_PUSH(&cu->specified_functions, specification);
                        }
                    }
                }
            } break;
            case DW_TAG_imported_module: _upf_parse_import(cu, die, abbrev, &ns->imported_nss); break;
            case DW_TAG_namespace:       _upf_parse_sub_namespace(cu, die_base, ns, ns_stack); break;
        }
        die = _upf_skip_die(cu, die, abbrev);
    }

    _UPF_ASSERT(ns_stack->length > 0);
    ns_stack->length--;
    return ns;
}

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *die, const uint8_t *die_end, const uint8_t *abbrev_table) {
    _UPF_ASSERT(cu_base != NULL && die != NULL && die_end != NULL && abbrev_table != NULL);

    _upf_cu cu = _UPF_ZERO_INIT;
    cu.base = cu_base;
    cu.str_offsets_base = UINT64_MAX;
    cu.rnglists_base = UINT64_MAX;
    cu.abbrevs = _upf_parse_abbrevs(abbrev_table);

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, &cu, die);
    _UPF_ASSERT(abbrev != NULL && abbrev->tag == DW_TAG_compile_unit);

    // Save to parse after initializing addr_base, str_offsets, and rnglists_base.
    const uint8_t *low_pc_die = NULL;
    _upf_attr low_pc_attr = _UPF_ZERO_INIT;
    const uint8_t *high_pc_die = NULL;
    _upf_attr high_pc_attr = _UPF_ZERO_INIT;
    const uint8_t *ranges_die = NULL;
    _upf_attr ranges_attr = _UPF_ZERO_INIT;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_addr_base:        cu.addr_base = _upf_offset_cast(die); break;
            case DW_AT_str_offsets_base: cu.str_offsets_base = _upf_offset_cast(die); break;
            case DW_AT_rnglists_base:    cu.rnglists_base = _upf_offset_cast(die); break;
            case DW_AT_low_pc:
                low_pc_attr = attr;
                low_pc_die = die;
                break;
            case DW_AT_high_pc:
                high_pc_attr = attr;
                high_pc_die = die;
                break;
            case DW_AT_ranges:
                ranges_attr = attr;
                ranges_die = die;
                break;
            case DW_AT_language:
                if (!_upf_is_language_c(_upf_get_data(die, attr))) return;
                break;
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    if (low_pc_die != NULL) cu.base_address = _upf_get_addr(&cu, low_pc_die, low_pc_attr.form);

    cu.scope.ranges = _upf_get_die_ranges(&cu, low_pc_die, low_pc_attr, high_pc_die, high_pc_attr, ranges_die, ranges_attr);
    if (cu.scope.ranges.length == 0) return;

    _upf_ns_vec ns_stack = _UPF_ZERO_INIT;
    _UPF_VECTOR_PUSH(&cu.scope.nss, _upf_parse_ns_body(&cu, die, &ns_stack));

    for (uint32_t i = 0; i < cu.ns_imports.length; i++) {
        bool found = false;
        for (uint32_t j = 0; j < cu.nss.length; j++) {
            if (cu.ns_imports.data[i].die == cu.nss.data[j].die) {
                _UPF_VECTOR_PUSH(cu.ns_imports.data[i].nss, cu.nss.data[j].ns);
                found = true;
                break;
            }
        }
        _UPF_ASSERT(found);
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

// Extern function mapping: function pointer -> GOT -> RELA -> symbol -> name -> DIE.
static void _upf_parse_extern_functions(void) {
    _UPF_ASSERT(_DYNAMIC != NULL);

    const char *string_table = NULL;
    const Elf64_Sym *symbol_table = NULL;
    const Elf64_Rela *rela_table = NULL;
    int rela_size = -1;
    for (const Elf64_Dyn *dyn = _DYNAMIC; dyn->d_tag != DT_NULL; dyn++) {
        switch (dyn->d_tag) {
            case DT_STRTAB:  string_table = (char *) dyn->d_un.d_ptr; break;
            case DT_SYMTAB:  symbol_table = (Elf64_Sym *) dyn->d_un.d_ptr; break;
            case DT_RELA:    rela_table = (Elf64_Rela *) dyn->d_un.d_ptr; break;
            case DT_RELASZ:  rela_size = dyn->d_un.d_val / sizeof(Elf64_Rela); break;
            case DT_RELAENT: _UPF_ASSERT(dyn->d_un.d_val == sizeof(Elf64_Rela)); break;
        }
    }
    if (string_table == NULL || symbol_table == NULL || rela_table == NULL || rela_size == -1) {
        _UPF_WARN("Failed to find all required ELF sections. Ignoring extern functions.");
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

        _upf_extern_function extern_function = _UPF_ZERO_INIT;
        extern_function.name = symbol_name;
        extern_function.pc = symbol_address;
        _UPF_VECTOR_PUSH(&_upf_state.extern_functions, extern_function);
    }
}

static void _upf_parse_elf(void) {
    struct stat file_info;
    if (stat("/proc/self/exe", &file_info) == -1) _UPF_ERROR("Failed to stat \"/proc/self/exe\": %s.", strerror(errno));
    size_t size = file_info.st_size;
    _upf_state.file_size = size;

    int fd = open("/proc/self/exe", O_RDONLY);
    _UPF_ASSERT(fd != -1);

    // A new instance of file must be mmap-ed because the one loaded in memory
    // only contains information needed at runtime, and doesn't include debug
    // information, section table, etc.
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
        _UPF_ERROR("Failed to find the debugging information. %s", _UPF_NO_DEBUG_INFO_ERROR);
    }
}

// ======================= LEXER ==========================

static _upf_cstr_vec _upf_get_args(char *string) {
    _UPF_ASSERT(string != NULL);

    _upf_cstr_vec args = _UPF_ZERO_INIT;
    int parens = 0;
    int braces = 0;
    const char *start = string;
    for (char *ch = string; *ch != '\0'; ch++) {
        switch (*ch) {
            case '"': {
                ch++;
                bool is_escaped = false;
                while (*ch != '\0') {
                    if (is_escaped) is_escaped = false;
                    else if (*ch == '\\') is_escaped = true;
                    else if (*ch == '"') break;
                    ch++;
                }
            } break;
            case '(': parens++; break;
            case ')': parens--; break;
            case '{': braces++; break;
            case '}': braces--; break;
            case ',':
                if (parens == 0 && braces == 0) {
                    *ch = '\0';
                    _UPF_VECTOR_PUSH(&args, start);
                    start = ch + 1;
                }
                break;
        }
    }
    _UPF_VECTOR_PUSH(&args, start);

    return args;
}

static void _upf_new_token(_upf_token_type kind, const char *string) {
    _upf_token token = _UPF_ZERO_INIT;
    token.type = kind;
    token.string = string;
    _UPF_VECTOR_PUSH(&_upf_state.tokens, token);
}

static void _upf_tokenize(const char *string) {
    _UPF_ASSERT(string != NULL);

    // Signs must be ordered from longest to shortest to prevent multicharacter
    // sign from being tokenized as multiple single character ones.
    static const _upf_token signs[]
        = {{_UPF_TT_ASSIGNMENT, "<<="}, {_UPF_TT_ASSIGNMENT, ">>="}, {_UPF_TT_ARROW_STAR, "->*"},

           {_UPF_TT_ARROW, "->"},       {_UPF_TT_INCREMENT, "++"},   {_UPF_TT_INCREMENT, "--"},   {_UPF_TT_LOGICAL, "<="},
           {_UPF_TT_LOGICAL, ">="},     {_UPF_TT_LOGICAL, "=="},     {_UPF_TT_LOGICAL, "!="},     {_UPF_TT_DOUBLE_AMPERSAND, "&&"},
           {_UPF_TT_LOGICAL, "||"},     {_UPF_TT_LOGICAL, "<<"},     {_UPF_TT_LOGICAL, ">>"},     {_UPF_TT_ASSIGNMENT, "*="},
           {_UPF_TT_ASSIGNMENT, "/="},  {_UPF_TT_ASSIGNMENT, "%="},  {_UPF_TT_ASSIGNMENT, "+="},  {_UPF_TT_ASSIGNMENT, "-="},
           {_UPF_TT_ASSIGNMENT, "&="},  {_UPF_TT_ASSIGNMENT, "^="},  {_UPF_TT_ASSIGNMENT, "|="},  {_UPF_TT_DOT_STAR, ".*"},
           {_UPF_TT_CXX_SCOPE, "::"},

           {_UPF_TT_COMMA, ","},        {_UPF_TT_AMPERSAND, "&"},    {_UPF_TT_STAR, "*"},         {_UPF_TT_OPEN_PAREN, "("},
           {_UPF_TT_CLOSE_PAREN, ")"},  {_UPF_TT_DOT, "."},          {_UPF_TT_OPEN_BRACKET, "["}, {_UPF_TT_CLOSE_BRACKET, "]"},
           {_UPF_TT_OPEN_BRACE, "{"},   {_UPF_TT_CLOSE_BRACE, "}"},  {_UPF_TT_QUESTION, "?"},     {_UPF_TT_COLON, ":"},
           {_UPF_TT_LESS_THAN, "<"},    {_UPF_TT_GREATER_THAN, ">"}, {_UPF_TT_UNARY, "!"},        {_UPF_TT_PLUS, "+"},
           {_UPF_TT_MINUS, "-"},        {_UPF_TT_UNARY, "~"},        {_UPF_TT_FACTOR, "/"},       {_UPF_TT_FACTOR, "%"},
           {_UPF_TT_LOGICAL, "^"},      {_UPF_TT_LOGICAL, "|"},      {_UPF_TT_ASSIGNMENT, "="}};


    static const _upf_token keywords[] = {
        {_UPF_TT_TYPE_SPECIFIER, "void"},
        {_UPF_TT_TYPE_SPECIFIER, "char"},
        {_UPF_TT_TYPE_SPECIFIER, "short"},
        {_UPF_TT_TYPE_SPECIFIER, "int"},
        {_UPF_TT_TYPE_SPECIFIER, "long"},
        {_UPF_TT_TYPE_SPECIFIER, "float"},
        {_UPF_TT_TYPE_SPECIFIER, "double"},
        {_UPF_TT_TYPE_SPECIFIER, "signed"},
        {_UPF_TT_TYPE_SPECIFIER, "unsigned"},
        {_UPF_TT_TYPE_SPECIFIER, "bool"},
        {_UPF_TT_STRUCT, "struct"},
        {_UPF_TT_STRUCT, "union"},
        {_UPF_TT_ENUM, "enum"},
        {_UPF_TT_TYPE_QUALIFIER, "const"},
        {_UPF_TT_TYPE_QUALIFIER, "volatile"},
        {_UPF_TT_TYPE_QUALIFIER, "restrict"},
        {_UPF_TT_ATOMIC, "_Atomic"},
        {_UPF_TT_GENERIC, "_Generic"},
        {_UPF_TT_SIZEOF, "sizeof"},
        {_UPF_TT_ALIGNOF, "_Alignof"},
        {_UPF_TT_ALIGNOF, "alignof"},
        {_UPF_TT_CXX_CAST, "static_cast"},
        {_UPF_TT_CXX_CAST, "reinterpret_cast"},
        {_UPF_TT_CXX_CAST, "dynamic_cast"},
        {_UPF_TT_CXX_CAST, "const_cast"},
        {_UPF_TT_CXX_NEW, "new"},
    };

    const char *ch = string;
    while (*ch != '\0') {
        if (*ch == ' ') {
            ch++;
            continue;
        }

        if ('0' <= *ch && *ch <= '9') {
            // Handle floats written as ".123"
            if (_upf_state.tokens.length > 0 && _UPF_VECTOR_TOP(&_upf_state.tokens).type == _UPF_TT_DOT) {
                _upf_state.tokens.length--;
                while (*ch != '.') ch--;
            }

            errno = 0;
            char *end = NULL;
            strtof(ch, &end);
            _UPF_ASSERT(errno == 0 && end != NULL);

            _upf_new_token(_UPF_TT_NUMBER, _upf_new_string(ch, end));
            ch = end;
        } else if (('a' <= *ch && *ch <= 'z') || ('A' <= *ch && *ch <= 'Z') || *ch == '_') {
            const char *end = ch;
            while (('a' <= *end && *end <= 'z') || ('A' <= *end && *end <= 'Z') || ('0' <= *end && *end <= '9') || *end == '_') end++;
            size_t string_len = end - ch;

            bool found = false;
            for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords) && !found; i++) {
                if (string_len == strlen(keywords[i].string) && strncmp(ch, keywords[i].string, string_len) == 0) {
                    _UPF_VECTOR_PUSH(&_upf_state.tokens, keywords[i]);
                    found = true;
                }
            }
            if (!found) _upf_new_token(_UPF_TT_IDENTIFIER, _upf_new_string(ch, end));

            ch = end;
        } else if (*ch == '"') {
            ch++;

            bool is_escaped = false;
            const char *end = ch;
            while (*end != '\0') {
                if (is_escaped) is_escaped = false;
                else if (*end == '\\') is_escaped = true;
                else if (*end == '\"') break;
                end++;
            }
            _UPF_ASSERT(*end != '\0');

            _upf_new_token(_UPF_TT_STRING, _upf_new_string(ch, end));
            ch = end + 1;
        } else {
            bool found = false;
            for (size_t i = 0; i < sizeof(signs) / sizeof(*signs) && !found; i++) {
                size_t len = strlen(signs[i].string);
                if (strncmp(ch, signs[i].string, len) == 0) {
                    _UPF_VECTOR_PUSH(&_upf_state.tokens, signs[i]);
                    ch += len;
                    found = true;
                }
            }

            if (!found) {
                _upf_new_token(_UPF_TT_UNKNOWN, _upf_new_string(ch, ch + 1));
                ch++;
            }
        }
    }
}

static bool _upf_has_token(void) { return _upf_state.tokens_idx < _upf_state.tokens.length; }

static _upf_token _upf_peek_token(void) {
    _UPF_ASSERT(_upf_state.tokens_idx < _upf_state.tokens.length);
    return _upf_state.tokens.data[_upf_state.tokens_idx];
}

static _upf_token _upf_consume_token(void) {
    _UPF_ASSERT(_upf_state.tokens_idx < _upf_state.tokens.length);
    return _upf_state.tokens.data[_upf_state.tokens_idx++];
}

static void _upf_consume_parens(_upf_token_type open, _upf_token_type close) {
    int parens = 1;
    while (parens > 0) {
        _upf_token token = _upf_consume_token();
        if (token.type == open) parens++;
        else if (token.type == close) parens--;
    }
}

static bool _upf_match_token(_upf_token_type kind) {
    if (!_upf_has_token() || _upf_peek_token().type != kind) return false;
    _upf_state.tokens_idx++;
    return true;
}

static _upf_token _upf_expect_token(_upf_token_type kind) {
    _upf_token token = _upf_consume_token();
    _UPF_ASSERT(token.type == kind);
    return token;
}

// ====================== PARSER ==========================

static _upf_parse_rule _upf_parse_rules[_UPF_TT_COUNT] = _UPF_ZERO_INIT;

static _upf_type *_upf_parse(_upf_parse_precedence precedence);

static _upf_ns *_upf_ns_find_sub_ns(_upf_ns *ns, const char *name) {
    _UPF_ASSERT(ns != NULL && name != NULL);
    if (ns->is_visited) return NULL;

    for (uint32_t i = 0; i < ns->sub_nss.length; i++) {
        if (strcmp(ns->sub_nss.data[i].name, name) == 0) return ns->sub_nss.data[i].ns;
    }

    for (uint32_t i = 0; i < ns->imported_nss.length; i++) {
        ns->is_visited = true;
        _upf_ns *result = _upf_ns_find_sub_ns(ns->imported_nss.data[i], name);
        ns->is_visited = false;
        if (result != NULL) return result;
    }

    return NULL;
}

static _upf_ns *_upf_scope_resolve_ns(const _upf_cstr_vec *ns_names, const _upf_scope *scope) {
    _UPF_ASSERT(ns_names != NULL && scope != NULL);
    for (uint32_t i = 0; i < scope->nss.length; i++) {
        _upf_ns *current = scope->nss.data[i];
        for (uint32_t j = 0; j < ns_names->length; j++) {
            current = _upf_ns_find_sub_ns(current, ns_names->data[j]);
            if (current == NULL) break;
        }
        if (current != NULL) return current;
    }
    return NULL;
}

static _upf_type *_upf_ns_get_type_by_name(_upf_ns *ns, const char *type_name) {
    _UPF_ASSERT(ns != NULL && type_name != NULL);
    if (ns->is_visited) return NULL;

    for (uint32_t i = 0; i < ns->types.length; i++) {
        if (strcmp(ns->types.data[i].name, type_name) == 0) {
            return _upf_parse_type(_upf_state.current_cu, ns->types.data[i].die);
        }
    }

    for (uint32_t i = 0; i < ns->imported_nss.length; i++) {
        ns->is_visited = true;
        _upf_type *result = _upf_ns_get_type_by_name(ns->imported_nss.data[i], type_name);
        ns->is_visited = false;
        if (result != NULL) return result;
    }

    return NULL;
}

static _upf_type *_upf_get_type_by_name(const char *type_name) {
    _UPF_ASSERT(type_name != NULL);
    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];
        for (uint32_t j = 0; j < scope->types.length; j++) {
            if (strcmp(scope->types.data[j].name, type_name) == 0) {
                return _upf_parse_type(_upf_state.current_cu, scope->types.data[j].die);
            }
        }

        for (uint32_t j = 0; j < scope->nss.length; j++) {
            _upf_type *result = _upf_ns_get_type_by_name(scope->nss.data[j], type_name);
            if (result != NULL) return result;
        }
    }
    return NULL;
}

static _upf_type *_upf_ns_get_function_by_name(_upf_ns *ns, const char *function_name) {
    _UPF_ASSERT(ns != NULL && function_name != NULL);
    if (ns->is_visited) return NULL;

    for (uint32_t i = 0; i < ns->functions.length; i++) {
        _upf_function *function = &ns->functions.data[i];
        if (strcmp(function->name, function_name) == 0) return _upf_get_function_type(function);
    }

    for (uint32_t i = 0; i < ns->imported_nss.length; i++) {
        ns->is_visited = true;
        _upf_type *result = _upf_ns_get_function_by_name(ns->imported_nss.data[i], function_name);
        ns->is_visited = false;
        if (result != NULL) return result;
    }

    return NULL;
}

static _upf_type *_upf_get_function_by_name(const char *function_name) {
    _UPF_ASSERT(function_name != NULL);
    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];
        for (uint32_t j = 0; j < scope->nss.length; j++) {
            _upf_type *type = _upf_ns_get_function_by_name(scope->nss.data[j], function_name);
            if (type != NULL) return type;
        }
    }
    return NULL;
}

static _upf_function *_upf_ns_get_function_by_pc(_upf_ns *ns, uint64_t pc) {
    _UPF_ASSERT(ns != NULL);
    if (ns->is_visited) return NULL;

    for (uint32_t i = 0; i < ns->functions.length; i++) {
        if (ns->functions.data[i].pc == pc) return &ns->functions.data[i];
    }

    for (uint32_t i = 0; i < ns->imported_nss.length; i++) {
        ns->is_visited = true;
        _upf_function *result = _upf_ns_get_function_by_pc(ns->imported_nss.data[i], pc);
        ns->is_visited = false;
        if (result != NULL) return result;
    }

    return NULL;
}

static _upf_function *_upf_get_function_by_pc(uint64_t pc) {
    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];
        for (uint32_t j = 0; j < scope->nss.length; j++) {
            _upf_function *function = _upf_ns_get_function_by_pc(scope->nss.data[j], pc);
            if (function != NULL) return function;
        }
    }
    return NULL;
}

static _upf_type *_upf_ns_get_variable_type(_upf_ns *ns, const char *var_name) {
    _UPF_ASSERT(ns != NULL && var_name != NULL);
    if (ns->is_visited) return NULL;

    for (uint32_t i = 0; i < ns->vars.length; i++) {
        if (strcmp(ns->vars.data[i].name, var_name) == 0) {
            return _upf_parse_type(_upf_state.current_cu, ns->vars.data[i].die);
        }
    }

    for (uint32_t i = 0; i < ns->imported_nss.length; i++) {
        ns->is_visited = true;
        _upf_type *result = _upf_ns_get_variable_type(ns->imported_nss.data[i], var_name);
        ns->is_visited = false;
        if (result != NULL) return result;
    }

    return NULL;
}

static _upf_type *_upf_get_variable_type(const char *var_name) {
    _UPF_ASSERT(var_name != NULL);
    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];
        for (uint32_t j = 0; j < scope->vars.length; j++) {
            if (strcmp(scope->vars.data[j].name, var_name) == 0) {
                return _upf_parse_type(_upf_state.current_cu, scope->vars.data[j].die);
            }
        }

        for (uint32_t j = 0; j < scope->nss.length; j++) {
            _upf_type *result = _upf_ns_get_variable_type(scope->nss.data[j], var_name);
            if (result != NULL) return result;
        }
    }
    return NULL;
}

static _upf_type *_upf_parse_expression(void) { return _upf_parse(_UPF_PREC_ASSIGNMENT); }

static _upf_abstract_declarator _upf_parse_abstract_declarator(void) {
    _upf_abstract_declarator result = _UPF_ZERO_INIT;
    while (_upf_match_token(_UPF_TT_STAR)) {
        result.pointers++;
        while (_upf_match_token(_UPF_TT_TYPE_QUALIFIER)) continue;
    }

    result.reference = _upf_match_token(_UPF_TT_AMPERSAND) || _upf_match_token(_UPF_TT_DOUBLE_AMPERSAND);

    uint32_t current_idx = _upf_state.tokens_idx;
    if (!_upf_match_token(_UPF_TT_OPEN_PAREN)) return result;

    _upf_abstract_declarator sub_result = _upf_parse_abstract_declarator();
    if (sub_result.failed || !_upf_match_token(_UPF_TT_CLOSE_PAREN)) {
        _upf_state.tokens_idx = current_idx;
        result.failed = true;
        return result;
    }

    if (!_upf_match_token(_UPF_TT_OPEN_PAREN)) {
        sub_result.pointers += result.pointers;
        return sub_result;
    }
    _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);

    if (!sub_result.reference) {
        _UPF_ASSERT(sub_result.pointers > 0);
        sub_result.pointers--;
    }

    _upf_type type = _UPF_ZERO_INIT;
    type.kind = _UPF_TK_FUNCTION;
    type.size = sizeof(void *);
    _upf_type *current_function_type = _upf_new_type(type);
    _upf_type **current_function_return_type = &current_function_type->as.function.return_type;

    while (sub_result.pointers-- > 0) current_function_type = _upf_get_pointer_to_type(current_function_type);

    if (sub_result.function_type == NULL) {
        result.function_type = current_function_type;
    } else {
        _UPF_ASSERT(sub_result.function_return_type != NULL);
        *sub_result.function_return_type = current_function_type;

        result.function_type = sub_result.function_type;
    }
    result.function_return_type = current_function_return_type;

    return result;
}

static _upf_type *_upf_parse_type_name(void) {
    bool leading_scope_op = _upf_match_token(_UPF_TT_CXX_SCOPE);

    const char *type_specifiers[4];
    size_t type_specifiers_idx = 0;
    _upf_cstr_vec scopes = _UPF_ZERO_INIT;
    const char *identifier = NULL;
    bool parsed_type = false;
    while (!parsed_type) {
        switch (_upf_peek_token().type) {
            case _UPF_TT_TYPE_QUALIFIER: _upf_consume_token(); break;
            case _UPF_TT_TYPE_SPECIFIER:
                _UPF_ASSERT(type_specifiers_idx < (sizeof(type_specifiers) / sizeof(type_specifiers[0])));
                type_specifiers[type_specifiers_idx++] = _upf_consume_token().string;
                break;
            case _UPF_TT_ATOMIC:
                _upf_consume_token();
                if (_upf_match_token(_UPF_TT_OPEN_PAREN)) {
                    _upf_parse_type_name();
                    _upf_expect_token(_UPF_TT_CLOSE_PAREN);
                }
                break;
            case _UPF_TT_STRUCT:
                _upf_consume_token();
                if (_upf_peek_token().type == _UPF_TT_IDENTIFIER) identifier = _upf_consume_token().string;
                if (_upf_match_token(_UPF_TT_OPEN_BRACE)) {
                    _UPF_ERROR("Cast to anonymous struct is not supported at %s:%d.", _upf_state.file_path, _upf_state.line);
                }
                break;
            case _UPF_TT_ENUM:
                _upf_consume_token();
                if (_upf_peek_token().type == _UPF_TT_IDENTIFIER) identifier = _upf_consume_token().string;
                if (_upf_match_token(_UPF_TT_OPEN_BRACE)) {
                    _UPF_ERROR("Cast to anonymous enum is not supported at %s:%d.", _upf_state.file_path, _upf_state.line);
                }
                break;
            case _UPF_TT_IDENTIFIER:
                identifier = _upf_consume_token().string;
                while (_upf_match_token(_UPF_TT_CXX_SCOPE)) {
                    _UPF_VECTOR_PUSH(&scopes, identifier);
                    identifier = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
                }
                break;
            default: parsed_type = true; break;
        }
    }
    if (identifier == NULL && type_specifiers_idx == 0) return NULL;

    _upf_abstract_declarator abstract_declarator = _upf_parse_abstract_declarator();

    _upf_type *type_ptr;
    if (identifier != NULL) {
        _UPF_ASSERT(type_specifiers_idx == 0);

        if (leading_scope_op) {
            _upf_ns *ns = _upf_scope_resolve_ns(&scopes, &_upf_state.current_cu->scope);
            type_ptr = _upf_ns_get_type_by_name(ns, identifier);
        } else if (scopes.length > 0) {
            _upf_ns *ns = NULL;
            for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
                ns = _upf_scope_resolve_ns(&scopes, _upf_state.current_scopes.data[i]);
                if (ns != NULL) break;
            }
            _UPF_ASSERT(ns != NULL);
            type_ptr = _upf_ns_get_type_by_name(ns, identifier);
        } else {
            type_ptr = _upf_get_type_by_name(identifier);
        }

        if (type_ptr == NULL) _UPF_ERROR("Failed to find type \"%s\" at %s:%d.", identifier, _upf_state.file_path, _upf_state.line);
    } else if (strcmp(type_specifiers[0], "void") == 0) {
        _UPF_ASSERT(type_specifiers_idx == 1);
        type_ptr = _upf_get_void_type();
    } else if (strcmp(type_specifiers[0], "bool") == 0) {
        _UPF_ASSERT(type_specifiers_idx == 1);
        type_ptr = _upf_get_bool_type();
    } else {
        int kind = DW_ATE_signed;
        bool is_signed = true;
        int longness = 0;
        for (size_t i = 0; i < type_specifiers_idx; i++) {
            if (strcmp(type_specifiers[i], "long") == 0) {
                longness++;
            } else if (strcmp(type_specifiers[i], "short") == 0) {
                longness--;
            } else if (strcmp(type_specifiers[i], "unsigned") == 0) {
                is_signed = false;
            } else if (strcmp(type_specifiers[i], "signed") == 0) {
                is_signed = true;
            } else if (strcmp(type_specifiers[i], "char") == 0) {
                kind = DW_ATE_signed_char;
            } else if (strcmp(type_specifiers[i], "int") == 0) {
                kind = DW_ATE_signed;
            } else if (strcmp(type_specifiers[i], "double") == 0) {
                kind = DW_ATE_float;
                longness++;
            } else if (strcmp(type_specifiers[i], "float") == 0) {
                kind = DW_ATE_float;
            }
        }

        _upf_type type = _UPF_ZERO_INIT;
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
                default: _UPF_ERROR("Invalid floating-point number length at %s:%d.", _upf_state.file_path, _upf_state.line);
            }
        } else if (kind == DW_ATE_signed) {
            switch (longness) {
                case -1: type.size = sizeof(short int); break;
                case 0:  type.size = sizeof(int); break;
                case 1:  type.size = sizeof(long int); break;
                case 2:  type.size = sizeof(long long int); break;
                default: _UPF_ERROR("Invalid integer length at %s:%d.", _upf_state.file_path, _upf_state.line);
            }
            type.kind = _upf_get_type_kind(is_signed ? DW_ATE_signed : DW_ATE_unsigned, type.size);
        } else {
            _UPF_ERROR("Invalid integer type at %s:%d.", _upf_state.file_path, _upf_state.line);
        }
        type_ptr = _upf_new_type(type);
    }

    _UPF_ASSERT(type_ptr != NULL);
    while (abstract_declarator.pointers > 0) {
        type_ptr = _upf_get_pointer_to_type(type_ptr);
        abstract_declarator.pointers--;
    }

    if (abstract_declarator.reference) type_ptr = _upf_get_reference_to_type(type_ptr);

    if (abstract_declarator.function_type != NULL) {
        *abstract_declarator.function_return_type = type_ptr;
        type_ptr = abstract_declarator.function_type;
    }

    return type_ptr;
}

static _upf_type *_upf_string(void) {
    _upf_consume_token();
    return _upf_get_cstr_type();
}

static _upf_type *_upf_number(void) {
    _upf_consume_token();
    return _upf_get_number_type();
}

static _upf_type *_upf_identifier(void) {
    bool leading_scope_op = _upf_match_token(_UPF_TT_CXX_SCOPE);
    _upf_cstr_vec scopes = _UPF_ZERO_INIT;
    const char *identifier = _upf_consume_token().string;
    while (_upf_match_token(_UPF_TT_CXX_SCOPE)) {
        _UPF_VECTOR_PUSH(&scopes, identifier);
        identifier = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
    }

    if (leading_scope_op) {
        _upf_ns *ns = _upf_scope_resolve_ns(&scopes, &_upf_state.current_cu->scope);
        _upf_type *type = _upf_ns_get_variable_type(ns, identifier);
        if (type != NULL) return type;

        return _upf_ns_get_function_by_name(ns, identifier);
    } else if (scopes.length > 0) {
        _upf_ns *ns = NULL;
        for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
            ns = _upf_scope_resolve_ns(&scopes, _upf_state.current_scopes.data[i]);
            if (ns != NULL) break;
        }
        _UPF_ASSERT(ns != NULL);

        _upf_type *type = _upf_ns_get_variable_type(ns, identifier);
        if (type != NULL) return type;

        return _upf_ns_get_function_by_name(ns, identifier);
    } else {
        _upf_type *type = _upf_get_variable_type(identifier);
        if (type != NULL) return type;

        return _upf_get_function_by_name(identifier);
    }
}

static _upf_type *_upf_generic(void) { _UPF_ERROR("Generics are not supported at %s:%d.", _upf_state.file_path, _upf_state.line); }

static _upf_type *_upf_sizeof(void) {
    _upf_consume_token();
    if (_upf_match_token(_UPF_TT_OPEN_PAREN)) {
        _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);
    } else {
        _upf_parse(_UPF_PREC_UNARY);
    }
    return _upf_get_number_type();
}

static _upf_type *_upf_alignof(void) {
    _upf_consume_token();
    _upf_expect_token(_UPF_TT_OPEN_PAREN);
    _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);
    return _upf_get_number_type();
}

static _upf_type *_upf_cxx_cast(void) {
    _upf_consume_token();
    _upf_expect_token(_UPF_TT_LESS_THAN);
    _upf_type *type = _upf_parse_type_name();
    _upf_expect_token(_UPF_TT_GREATER_THAN);
    _upf_expect_token(_UPF_TT_OPEN_PAREN);
    _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);
    return type;
}

static _upf_type *_upf_cxx_new(void) {
    _upf_consume_token();

    _upf_type *type = NULL;
    if (_upf_match_token(_UPF_TT_OPEN_PAREN)) {
        // Try to parse as "new (type)".
        uint32_t current_idx = _upf_state.tokens_idx;
        type = _upf_parse_type_name();

        // Parse as "new (placement-args) type".
        if (!_upf_match_token(_UPF_TT_CLOSE_PAREN) || type == NULL) {
            _upf_state.tokens_idx = current_idx;
            _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);

            bool is_parenthesised = _upf_match_token(_UPF_TT_OPEN_PAREN);
            type = _upf_parse_type_name();
            if (is_parenthesised) _upf_expect_token(_UPF_TT_CLOSE_PAREN);
        }
    } else {
        // Parse as "new type".
        type = _upf_parse_type_name();
    }

    // Optional parentheses- or brace-enclosed initializer.
    if (_upf_match_token(_UPF_TT_OPEN_PAREN)) {
        _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);
    } else if (_upf_match_token(_UPF_TT_OPEN_BRACE)) {
        _upf_consume_parens(_UPF_TT_OPEN_BRACE, _UPF_TT_CLOSE_BRACE);
    }

    return _upf_get_pointer_to_type(type);
}

static _upf_type *_upf_cxx_scope(void) {
    if (_upf_state.tokens_idx + 1 < _upf_state.tokens.length && _upf_state.tokens.data[_upf_state.tokens_idx + 1].type == _UPF_TT_CXX_NEW) {
        _upf_consume_token();
        return _upf_cxx_new();
    } else {
        return _upf_identifier();
    }
}

static _upf_type *_upf_unary(void) {
    _upf_token unop = _upf_consume_token();
    _upf_type *type = _upf_parse(_UPF_PREC_UNARY);

    switch (unop.type) {
        case _UPF_TT_AMPERSAND: return _upf_get_pointer_to_type(type);
        case _UPF_TT_STAR:      return _upf_dereference_type(type);
        default:                return type;
    }
}

static _upf_type *_upf_paren(void) {
    _upf_consume_token();

    // Try to parse as grouping: "(expression)".
    uint32_t current_idx = _upf_state.tokens_idx;
    _upf_type *type = NULL;
    do {
        type = _upf_parse_expression();
    } while (_upf_match_token(_UPF_TT_COMMA));
    if (type != NULL) {
        _upf_expect_token(_UPF_TT_CLOSE_PAREN);
        return type;
    }

    // If failed, try to parse as a cast expression: "(typename) expression".
    _upf_state.tokens_idx = current_idx;
    type = _upf_parse_type_name();
    _upf_expect_token(_UPF_TT_CLOSE_PAREN);

    if (_upf_match_token(_UPF_TT_OPEN_BRACE)) {
        // If parenthesised typename is followed by a '{', it is a compound literal.
        _upf_consume_parens(_UPF_TT_OPEN_BRACE, _UPF_TT_CLOSE_BRACE);
    } else {
        _upf_parse(_UPF_PREC_UNARY);
    }

    return type;
}

static _upf_type *_upf_call(_upf_type *type) {
    _upf_consume_token();
    _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);

    _UPF_ASSERT(type != NULL);
    if (_upf_is_pointer(type)) type = _upf_dereference_type(type);

    _UPF_ASSERT(type->kind == _UPF_TK_FUNCTION);
    return type->as.function.return_type;
}

static _upf_type *_upf_index(_upf_type *type) {
    _upf_consume_token();
    _upf_consume_parens(_UPF_TT_OPEN_BRACKET, _UPF_TT_CLOSE_BRACKET);
    return _upf_dereference_type(type);
}

static _upf_type *_upf_dot(_upf_type *type) {
    _upf_token dot = _upf_consume_token();
    const char *member_name = _upf_expect_token(_UPF_TT_IDENTIFIER).string;

    if (dot.type == _UPF_TT_ARROW) type = _upf_dereference_type(type);

    _UPF_ASSERT(type->kind == _UPF_TK_STRUCT || type->kind == _UPF_TK_UNION);
    _upf_member_vec members = type->as.cstruct.members;
    for (uint32_t i = 0; i < members.length; i++) {
        if (strcmp(members.data[i].name, member_name) == 0) {
            return members.data[i].type;
        }
    }
    _upf_function_vec methods = type->as.cstruct.methods;
    for (uint32_t i = 0; i < methods.length; i++) {
        if (strcmp(methods.data[i].name, member_name) == 0) {
            return _upf_get_function_type(&methods.data[i]);
        }
    }
    return NULL;
}

static _upf_type *_upf_dot_star(__attribute__((unused)) _upf_type *_type) {
    _upf_consume_token();
    _upf_type *type = _upf_parse(_UPF_PREC_POSTFIX);
    _UPF_ASSERT(type->kind == _UPF_TK_MEMBER_POINTER);
    return type->as.pointer.type;
}

static _upf_type *_upf_postfix(_upf_type *type) {
    _upf_consume_token();
    // Post increment cannot change the type.
    return type;
}

static _upf_type *_upf_binary(_upf_type *lhs) {
    _upf_token binop = _upf_consume_token();
    _upf_type *rhs = _upf_parse((_upf_parse_precedence) (_upf_parse_rules[binop.type].precedence + 1));

    switch (binop.type) {
        case _UPF_TT_PLUS:
            // If either operands is pointer, the entire expression is of pointer type.
            // Otherwise, return type of either side since we don't care about the exact arithmetic type.
            if (_upf_is_pointer(lhs)) return lhs;
            return rhs;
        case _UPF_TT_MINUS:
            // If exactly one operand is pointer, the expression is of pointer type.
            // Otherwise, return type of either side since we don't care about the exact arithmetic type.
            if (_upf_is_pointer(lhs)) {
                if (_upf_is_pointer(rhs)) return _upf_get_number_type();
                return lhs;
            }
            return rhs;
        default:
            // We don't care about the exact arithmetic type, so return either operand.
            return rhs;
    }
}

static _upf_type *_upf_ternary(__attribute__((unused)) _upf_type *type) {
    _upf_consume_token();
    _upf_parse_expression();
    _upf_expect_token(_UPF_TT_COLON);
    // The result of both branches must have the same type, get type from either branch.
    return _upf_parse(_UPF_PREC_TERNARY);
}

static _upf_type *_upf_assignment(_upf_type *type) {
    _upf_consume_token();
    _upf_parse(_UPF_PREC_ASSIGNMENT);
    // Type of an assignment expression is that of its LHS.
    return type;
}

static _upf_type *_upf_parse(_upf_parse_precedence precedence) {
    _upf_token token = _upf_peek_token();
    _upf_parse_rule rule = _upf_parse_rules[token.type];
    if (rule.prefix == NULL) return NULL;

    _upf_type *type = rule.prefix();
    while (_upf_has_token() && type != NULL) {
        _upf_parse_rule next_rule = _upf_parse_rules[_upf_peek_token().type];
        if (precedence > next_rule.precedence) break;
        type = next_rule.infix(type);
    }
    return type;
}

static _upf_cu *_upf_find_cu(uint64_t pc) {
    for (uint32_t i = 0; i < _upf_state.cus.length; i++) {
        if (_upf_is_in_range(pc, _upf_state.cus.data[i].scope.ranges)) return &_upf_state.cus.data[i];
    }
    return NULL;
}

static void _upf_find_scopes(uint64_t pc, _upf_scope *scope, _upf_scope_vec *result) {
    for (uint32_t i = 0; i < scope->scopes.length; i++) {
        if (!_upf_is_in_range(pc, scope->scopes.data[i]->ranges)) continue;
        _upf_find_scopes(pc, scope->scopes.data[i], result);
        _UPF_VECTOR_PUSH(result, scope->scopes.data[i]);
        break;
    }
}

static const _upf_type *_upf_get_arg_type(const char *arg) {
    _UPF_ASSERT(arg != NULL);

    _upf_state.tokens.length = 0;
    _upf_state.tokens_idx = 0;
    _upf_tokenize(arg);

    _upf_type *type = _upf_parse_expression();
    if (type == NULL || _upf_state.tokens_idx != _upf_state.tokens.length) {
        _UPF_ERROR("Failed to parse the argument \"%s\" at %s:%d.", arg, _upf_state.file_path, _upf_state.line);
    }

    if (!_upf_is_pointer(type)) _UPF_ERROR("Argument \"%s\" must be a pointer at %s:%d.", arg, _upf_state.file_path, _upf_state.line);
    type = _upf_dereference_type(type);

    _UPF_ASSERT(type != NULL);
    if (type->kind == _UPF_TK_VOID) {
        _UPF_ERROR("Cannot print type void. To print the void pointer itself, get a pointer to \"%s\" at %s:%d.", arg, _upf_state.file_path,
                   _upf_state.line);
    }
    return type;
}

static void _upf_init_parsing_rules(void) {
#ifdef __cplusplus
#define _UPF_DEFINE_RULE(token, prefix, infix, precedence) _upf_parse_rules[token] = _upf_parse_rule{prefix, infix, precedence}
#else
#define _UPF_DEFINE_RULE(token, prefix, infix, precedence) _upf_parse_rules[token] = (_upf_parse_rule) {prefix, infix, precedence}
#endif
    // clang-format off
    _UPF_DEFINE_RULE(_UPF_TT_NUMBER,           _upf_number,     NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_STRING,           _upf_string,     NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_IDENTIFIER,       _upf_identifier, NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_GENERIC,          _upf_generic,    NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_SIZEOF,           _upf_sizeof,     NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_ALIGNOF,          _upf_alignof,    NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_CAST,         _upf_cxx_cast,   NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_NEW,          _upf_cxx_new,    NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_SCOPE,        _upf_cxx_scope,  NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_UNARY,            _upf_unary,      NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_OPEN_PAREN,       _upf_paren,      _upf_call,       _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_INCREMENT,        _upf_unary,      _upf_postfix,    _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_OPEN_BRACKET,     NULL,            _upf_index,      _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_DOT,              NULL,            _upf_dot,        _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_ARROW,            NULL,            _upf_dot,        _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_DOT_STAR,         NULL,            _upf_dot_star,   _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_ARROW_STAR,       NULL,            _upf_dot_star,   _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_FACTOR,           NULL,            _upf_binary,     _UPF_PREC_FACTOR    );
    _UPF_DEFINE_RULE(_UPF_TT_STAR,             _upf_unary,      _upf_binary,     _UPF_PREC_FACTOR    );
    _UPF_DEFINE_RULE(_UPF_TT_PLUS,             _upf_unary,      _upf_binary,     _UPF_PREC_TERM      );
    _UPF_DEFINE_RULE(_UPF_TT_MINUS,            _upf_unary,      _upf_binary,     _UPF_PREC_TERM      );
    _UPF_DEFINE_RULE(_UPF_TT_AMPERSAND,        _upf_unary,      _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_LESS_THAN,        _upf_unary,      _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_GREATER_THAN,     _upf_unary,      _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_DOUBLE_AMPERSAND, NULL,            _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_LOGICAL,          NULL,            _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_QUESTION,         NULL,            _upf_ternary,    _UPF_PREC_TERNARY   );
    _UPF_DEFINE_RULE(_UPF_TT_ASSIGNMENT,       NULL,            _upf_assignment, _UPF_PREC_ASSIGNMENT);
    // clang-format on
#undef _UPF_DEFINE_RULE
}

// ================== /proc/pid/maps ======================

static _upf_range_vec _upf_get_readable_address_ranges(void) {
    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) _UPF_ERROR("Failed to open \"/proc/self/maps\": %s.", strerror(errno));

    _upf_range_vec ranges = _UPF_ZERO_INIT;
    _upf_range range = _UPF_ZERO_INIT;
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
            _UPF_ERROR("Failed to parse \"/proc/self/maps\": invalid format.");
        }

        if (read_bit == 'r') _UPF_VECTOR_PUSH(&ranges, range);
    }
    free(line);
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

#define _UPF_INITIAL_BUFFER_SIZE 512

#define _upf_bprintf(...)                                                                         \
    while (true) {                                                                                \
        int bytes = snprintf(_upf_state.ptr, _upf_state.free, __VA_ARGS__);                       \
        if (bytes < 0) _UPF_ERROR("Unexpected error occurred in snprintf: %s.", strerror(errno)); \
        if ((uint32_t) bytes >= _upf_state.free) {                                                \
            uint32_t used = _upf_state.size - _upf_state.free;                                    \
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

    static char str[2];
    str[0] = ch;
    str[1] = '\0';
    return str;
}

static bool _upf_is_printable(char c) { return ' ' <= c && c <= '~'; }

static void _upf_print_modifiers(int modifiers, bool print_trailing_whitespace) {
    bool print_space = false;
    if (modifiers & _UPF_MOD_CONST) {
        _upf_bprintf("const");
        print_space = true;
    }
    if (modifiers & _UPF_MOD_VOLATILE) {
        if (print_space) _upf_bprintf(" ");
        _upf_bprintf("volatile");
        print_space = true;
    }
    if (modifiers & _UPF_MOD_RESTRICT) {
        if (print_space) _upf_bprintf(" ");
        _upf_bprintf("restrict");
        print_space = true;
    }
    if (modifiers & _UPF_MOD_ATOMIC) {
        if (print_space) _upf_bprintf(" ");
        // TODO: atomic -> _Atomic?
        _upf_bprintf("atomic");
        print_space = true;
    }
    if (print_space && print_trailing_whitespace) _upf_bprintf(" ");
}

static void _upf_print_type_name(const _upf_type *type, bool print_trailing_whitespace, bool is_return_type) {
    _UPF_ASSERT(type != NULL);

    switch (type->kind) {
        case _UPF_TK_POINTER:
            if (type->as.pointer.type == NULL) {
                _upf_bprintf("void *");
                _upf_print_modifiers(type->modifiers, print_trailing_whitespace);
                break;
            }

            if (type->as.pointer.type->kind == _UPF_TK_FUNCTION) {
                _upf_print_type_name(type->as.pointer.type, print_trailing_whitespace, is_return_type);
                break;
            }

            _upf_print_type_name(type->as.pointer.type, true, is_return_type);
            _upf_bprintf("*");
            _upf_print_modifiers(type->modifiers, print_trailing_whitespace);
            break;
        case _UPF_TK_REFERENCE:
            _upf_print_type_name(type->as.reference.type, true, is_return_type);
            _upf_bprintf("%s", type->as.reference.is_rvalue ? "&&" : "&");
            break;
        case _UPF_TK_FUNCTION:
            if (is_return_type) _upf_bprintf("(");

            if (type->as.function.return_type == NULL) {
                _upf_bprintf("void");
            } else {
                _upf_print_type_name(type->as.function.return_type, false, true);
            }

            _upf_bprintf("(");
            for (uint32_t i = 0; i < type->as.function.arg_types.length; i++) {
                if (i > 0) _upf_bprintf(", ");
                _upf_print_type_name(type->as.function.arg_types.data[i], false, false);
            }
            if (type->as.function.is_variadic) {
                if (type->as.function.arg_types.length > 0) _upf_bprintf(", ");
                _upf_bprintf("...");
            }
            _upf_bprintf(")");
            if (is_return_type) _upf_bprintf(")");
            if (print_trailing_whitespace) _upf_bprintf(" ");
            break;
        default:
            _upf_print_modifiers(type->modifiers, true);
            _upf_bprintf("%s", type->name == NULL ? "<unnamed>" : type->name);
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

    const char *end = (const char *) _upf_get_memory_region_end(str);
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
        // Increment inside of macro (_upf_bprintf) may be triggered twice.
        str++;
    }
    _upf_bprintf("\"");
    if (is_truncated) _upf_bprintf("...");
    _upf_bprintf(")");
}

__attribute__((no_sanitize_address)) static void _upf_collect_circular_structs(_upf_indexed_struct_vec *seen,
                                                                               _upf_indexed_struct_vec *circular, const uint8_t *data,
                                                                               const _upf_type *type, int depth) {
    _UPF_ASSERT(seen != NULL && circular != NULL && type != NULL);

    if (UPRINTF_MAX_DEPTH >= 0 && depth >= UPRINTF_MAX_DEPTH) return;
    if (data == NULL || _upf_get_memory_region_end(data) == NULL) return;

    if (type->kind == _UPF_TK_POINTER) {
        const uint8_t *ptr;
        memcpy(&ptr, data, sizeof(ptr));
        if (type->as.pointer.type == NULL) return;
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

    _upf_indexed_struct indexed_struct = _UPF_ZERO_INIT;
    indexed_struct.data = data;
    indexed_struct.type = type;
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
__attribute__((no_sanitize_address)) static void _upf_print_type(_upf_indexed_struct_vec *circular, const uint8_t *data,
                                                                 const _upf_type *type, int depth) {
    _UPF_ASSERT(circular != NULL && type != NULL);

    if (UPRINTF_MAX_DEPTH >= 0 && depth >= UPRINTF_MAX_DEPTH) {
        switch (type->kind) {
            case _UPF_TK_UNION:
            case _UPF_TK_STRUCT: _upf_bprintf("{...}"); return;
            default:             break;
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
        case _UPF_TK_UNION:  _upf_bprintf("<union> "); __attribute__((fallthrough));  // Handle union as struct
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
                if (type->as.cstruct.is_declaration) _upf_bprintf(" <opaque>");
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
                _upf_print_type_name(member->type, true, false);
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

            int64_t enum_value = 0;
            memcpy(&enum_value, data, underlying_type->size);

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
                while (j < type->as.array.lengths.data[0]) {
                    const uint8_t *next = data + element_size * j;

                    // Use a loop instead of memcmp to avoid triggering libasan
                    // in case the pointer is valid but unallocated.
                    // Disabling address sanitization for the function does not
                    // turn off the check because it is in the interceptor memcmp,
                    // and interceptors cannot be disabled for a single function.
                    bool equal = true;
                    for (size_t k = 0; k < element_size; k++) {
                        if (current[k] != next[k]) {
                            equal = false;
                            break;
                        }
                    }
                    if (!equal) break;

                    j++;
                }

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

            const _upf_type *pointed_type = type->as.pointer.type;
            if (pointed_type == NULL || pointed_type->kind == _UPF_TK_POINTER || pointed_type->kind == _UPF_TK_VOID) {
                _upf_bprintf("%p", ptr);
                return;
            }

            if (pointed_type->kind == _UPF_TK_FUNCTION) {
                _upf_print_type(circular, (const uint8_t *) ptr, pointed_type, depth);
                break;
            }

            if (pointed_type->kind == _UPF_TK_SCHAR || pointed_type->kind == _UPF_TK_UCHAR) {
                _upf_print_char_ptr((const char *) ptr);
                return;
            }

            _upf_bprintf("%p (", ptr);
            _upf_print_type(circular, (const uint8_t *) ptr, pointed_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_REFERENCE: {
            const uint8_t *ptr;
            memcpy(&ptr, data, sizeof(ptr));

            // If it is a valid pointer, treat reference as a pointer,
            // otherwise assume that it is the data.
            if (_upf_get_memory_region_end(ptr) == NULL) {
                _upf_print_type(circular, data, type->as.reference.type, depth);
            } else {
                _upf_print_type(circular, ptr, type->as.reference.type, depth);
            }
        } break;
        case _UPF_TK_MEMBER_POINTER: _upf_bprintf("<member pointer>"); break;
        case _UPF_TK_FUNCTION: {
            const _upf_function *function = type->as.function.function_ptr;
            if (function == NULL) {
                uint64_t relative_function_pc = data - _upf_state.base;
                function = _upf_get_function_by_pc(relative_function_pc);
            }
            if (function == NULL) {
                uint64_t absolute_function_pc = (uint64_t) data;
                for (uint32_t i = 0; i < _upf_state.extern_functions.length; i++) {
                    if (_upf_state.extern_functions.data[i].pc != absolute_function_pc) continue;

                    const char *function_name = _upf_state.extern_functions.data[i].name;
                    _UPF_ASSERT(function_name != NULL);

                    for (uint32_t j = 0; j < _upf_state.current_cu->extern_functions.length; j++) {
                        if (strcmp(_upf_state.current_cu->extern_functions.data[j].name, function_name) == 0) {
                            function = &_upf_state.current_cu->extern_functions.data[j];
                            break;
                        }
                    }
                    break;
                }
            }

            _upf_bprintf("%p", (void *) data);
            if (function != NULL) {
                _upf_bprintf(" (");
                _upf_print_type_name(type->as.function.return_type, true, true);
                _upf_bprintf("%s(", function->name);  // TODO: type->name?
                for (uint32_t i = 0; i < function->args.length; i++) {
                    _upf_named_type arg = function->args.data[i];
                    _upf_type *arg_type = _upf_parse_type(_upf_state.current_cu, arg.die);
                    bool has_name = arg.name != NULL;

                    if (i > 0) _upf_bprintf(", ");
                    _upf_print_type_name(arg_type, has_name, false);
                    if (has_name) _upf_bprintf("%s", arg.name);
                }
                if (function->is_variadic) {
                    if (function->args.length > 0) _upf_bprintf(", ");
                    _upf_bprintf("...");
                }
                _upf_bprintf("))");
            }
        } break;
        case _UPF_TK_U1: _upf_bprintf("%hhu", *data); break;
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
        case _UPF_TK_S1: _upf_bprintf("%hhd", *((const int8_t *) data)); break;
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
        case _UPF_TK_BOOL:  _upf_bprintf("%s", *data ? "true" : "false"); break;
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
        case _UPF_TK_VOID:    _UPF_WARN("Void must be a pointer. Ignoring this type."); break;
        case _UPF_TK_UNKNOWN: _upf_bprintf("<unknown>"); break;
    }
}

// ===================== GETTING PC =======================

static int _upf_phdr_callback(struct dl_phdr_info *info_, __attribute__((unused)) size_t _size, void *data_) {
    _upf_dl_phdr_info *info = (_upf_dl_phdr_info *) info_;
    void **data = (void **) data_;

    // Empty name seems to indicate current executable.
    if (strcmp(info->dlpi_name, "") == 0) {
        *data = (void *) info->dlpi_addr;
        // Return non-zero value to exit early.
        return 1;
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
    if (setjmp(_upf_state.error_jmp_buf) != 0) return;

    if (access("/proc/self/exe", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/exe\" to be a valid path.");
    if (access("/proc/self/maps", R_OK) != 0) _UPF_ERROR("Expected \"/proc/self/maps\" to be a valid path.");

    _upf_state.base = (const uint8_t *) _upf_get_this_executable_address();

    _upf_parse_elf();
    _upf_parse_extern_functions();
    _upf_parse_dwarf();

    _upf_init_parsing_rules();

    _upf_state.size = _UPF_INITIAL_BUFFER_SIZE;
    _upf_state.buffer = (char *) malloc(_upf_state.size * sizeof(*_upf_state.buffer));
    if (_upf_state.buffer == NULL) _UPF_OUT_OF_MEMORY();
    _upf_state.is_init = true;
}

__attribute__((destructor)) void _upf_fini(void) {
    if (_upf_state.file != NULL) munmap(_upf_state.file, _upf_state.file_size);
    free(_upf_state.buffer);
    _upf_free_allocator();
}

#ifdef __cplusplus
}  // namespace uprintf

extern "C" {
#endif

__attribute__((noinline)) void _upf_uprintf(const char *file_path, int line, const char *fmt, const char *args_string, ...) {
#ifdef __cplusplus
    using namespace uprintf;
#endif

    _UPF_ASSERT(file_path != NULL && line > 0 && fmt != NULL && args_string != NULL);

    if (setjmp(_upf_state.error_jmp_buf) != 0) return;
    if (!_upf_state.is_init) return;

    _upf_state.ptr = _upf_state.buffer;
    _upf_state.free = _upf_state.size;
    _upf_state.addresses = _upf_get_readable_address_ranges();
    _upf_state.circular_id = 0;
    _upf_state.file_path = file_path;
    _upf_state.line = line;

    uint8_t *pc_ptr = (uint8_t *) __builtin_extract_return_addr(__builtin_return_address(0));
    _UPF_ASSERT(pc_ptr != NULL);
    uint64_t pc = pc_ptr - _upf_state.base;

    _upf_state.current_cu = _upf_find_cu(pc);
    _UPF_ASSERT(_upf_state.current_cu != NULL);  // TODO: error instead

    _upf_state.current_scopes.length = 0;
    _upf_find_scopes(pc, &_upf_state.current_cu->scope, &_upf_state.current_scopes);

    char *args_string_copy = _upf_new_string(args_string, args_string + strlen(args_string));
    _upf_cstr_vec args = _upf_get_args(args_string_copy);
    size_t arg_idx = 0;

    _upf_indexed_struct_vec seen = _UPF_ZERO_INIT;
    _upf_indexed_struct_vec circular = _UPF_ZERO_INIT;

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

            const uint8_t *ptr = (const uint8_t *) va_arg(va_args, void *);
            const _upf_type *type = _upf_get_arg_type(args.data[arg_idx++]);
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

#ifdef __cplusplus
}  // extern "C"
#endif

// ====================== UNDEF ===========================

#undef __USE_XOPEN_EXTENDED
#undef _GNU_SOURCE
#undef _UPF_SET_TEST_STATUS
#undef _UPF_LOG
#undef _UPF_WARN
#undef _UPF_ERROR
#undef _UPF_ASSERT
#undef _UPF_OUT_OF_MEMORY
#undef _UPF_NO_DEBUG_INFO_ERROR
#undef _UPF_INITIAL_VECTOR_CAPACITY
#undef _UPF_VECTOR_TYPEDEF
#undef _UPF_VECTOR_PUSH
#undef _UPF_VECTOR_TOP
#undef _UPF_MOD_CONST
#undef _UPF_MOD_VOLATILE
#undef _UPF_MOD_RESTRICT
#undef _UPF_MOD_ATOMIC
#undef _UPF_INITIAL_REGION_SIZE
#undef _UPF_INITIAL_BUFFER_SIZE
#undef _upf_bprintf

#endif  // UPRINTF_IMPLEMENTATION
