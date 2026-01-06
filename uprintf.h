// uprintf v1.1.0
// Documentation, examples and issues: https://github.com/spevnev/uprintf

// Minimal example:
/*
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

typedef struct {
    int i;
    float f;
    const char *str;
} S;

int main(void) {
    S s = {1, 2.3F, "string"};
    uprintf("structure: %S\n", &s);
    return 0;
}
*/

// Building and running:
/*
$ gcc -g2 example.c
$ ./a.out
structure: {
    int i = 1
    float f = 2.300000
    const char *str = 0x559467793d83 ("string")
}
*/

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

#ifndef UPRINTF_IGNORE_STRUCTS
#define UPRINTF_IGNORE_STRUCTS "FILE,pthread_mutex_t,pthread_cond_t"
#endif

#ifndef UPRINTF_ARRAY_COMPRESSION_THRESHOLD
#define UPRINTF_ARRAY_COMPRESSION_THRESHOLD 4
#endif

#ifndef UPRINTF_MAX_STRING_LENGTH
#define UPRINTF_MAX_STRING_LENGTH 200
#endif

// ===================== INCLUDES =========================

#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <link.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
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

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// Partial redefinition of dl_phdr_info.
typedef struct {
    ElfW(Addr) dlpi_addr;
    const char *dlpi_name;
} _upf_dl_phdr_info;

struct dl_phdr_info;
int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data);

// Linker-generated entry to the dynamic section of ELF.
extern ElfW(Dyn) _DYNAMIC[];

// ===================== dwarf.h ==========================

// dwarf.h's location is inconsistent and the package containing it may not be
// installed, so the library includes a partial implementation.

#define DW_UT_compile 0x01

#define DW_TAG_array_type 0x01
#define DW_TAG_class_type 0x02
#define DW_TAG_enumeration_type 0x04
#define DW_TAG_formal_parameter 0x05
#define DW_TAG_imported_declaration 0x08
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
#define DW_AT_linkage_name 0x6e
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

#define _UPF_LOG(type, ...)                       \
    do {                                          \
        fprintf(stderr, "(uprintf) [%s] ", type); \
        fprintf(stderr, __VA_ARGS__);             \
        fprintf(stderr, "\n");                    \
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

// ======================= C++ ============================

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

#define _UPF_VECTOR_TYPEDEF(name, type) \
    typedef struct {                    \
        uint32_t capacity;              \
        uint32_t length;                \
        type *data;                     \
    } name

#define _UPF_MAP_TYPEDEF(name, key_t, value_t) \
    typedef struct {                           \
        uint32_t capacity;                     \
        uint32_t size;                         \
        struct {                               \
            uint32_t hash;                     \
            key_t key;                         \
            value_t value;                     \
        } *data;                               \
    } name

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
    const uint8_t *die;
    const char *name;
} _upf_named_type;

_UPF_VECTOR_TYPEDEF(_upf_named_type_vec, _upf_named_type);

typedef struct {
    const char *name;
    const char *linkage_name;
    const uint8_t *return_type_die;
    const uint8_t *specification_die;
    _upf_named_type_vec args;
    bool is_variadic;
    bool is_external;
    uint64_t pc;
} _upf_function;

_UPF_VECTOR_TYPEDEF(_upf_function_vec, _upf_function);

typedef struct {
    // Number of inheritance levels from where the method originates.
    int inheritance;
    _upf_function function;
} _upf_method;

_UPF_VECTOR_TYPEDEF(_upf_method_vec, _upf_method);

typedef struct {
    const char *name;
    int64_t value;
} _upf_enum;

_UPF_VECTOR_TYPEDEF(_upf_enum_vec, _upf_enum);

#define _UPF_MOD_CONST 1 << 0
#define _UPF_MOD_VOLATILE 1 << 1
#define _UPF_MOD_RESTRICT 1 << 2
#define _UPF_MOD_ATOMIC 1 << 3

typedef struct _upf_type _upf_type;
_UPF_VECTOR_TYPEDEF(_upf_type_ptr_vec, struct _upf_type *);

typedef struct {
    const char *name;
    _upf_type *type;
    size_t offset;
    // Bit size is not zero only for bit field.
    int bit_size;
    // Number of inheritance levels from where the member originates.
    int inheritance;
} _upf_member;

_UPF_VECTOR_TYPEDEF(_upf_member_vec, _upf_member);

struct _upf_type {
    const char *name;
    _upf_type_kind kind;
    int modifiers;
    size_t size;
    union {
        struct {
            bool is_declaration;
            _upf_member_vec members;
            _upf_method_vec methods;
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
            // Pointer to a function definition, if type was generated from it.
            // NULL if the type is parsed from DWARF.
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

struct _upf_ns;
_UPF_VECTOR_TYPEDEF(_upf_ns_vec, struct _upf_ns *);

_UPF_MAP_TYPEDEF(_upf_name_type_map, const char *, const uint8_t *);
_UPF_MAP_TYPEDEF(_upf_name_function_map, const char *, _upf_function);
_UPF_MAP_TYPEDEF(_upf_name_ns_map, const char *, struct _upf_ns *);

typedef struct _upf_ns {
    bool is_visited;
    _upf_name_type_map vars;
    _upf_name_type_map types;
    _upf_name_function_map functions;
    _upf_ns_vec imported_nss;
    _upf_name_ns_map sub_nss;
} _upf_ns;

typedef struct _upf_ns_node {
    _upf_ns *value;
    struct _upf_ns_node *prev;
} _upf_ns_node;

typedef struct {
    _upf_ns_node *head;
    _upf_ns_node *tail;
} _upf_ns_queue;

struct _upf_scope;
_UPF_VECTOR_TYPEDEF(_upf_scope_vec, struct _upf_scope *);

typedef struct _upf_scope {
    _upf_range_vec ranges;
    _upf_scope_vec scopes;
    _upf_name_type_map vars;
    _upf_name_type_map types;
    _upf_ns_vec nss;
} _upf_scope;

typedef struct {
    bool is_cxx;
    const uint8_t *base;
    uint64_t base_address;
    uint64_t addr_base;
    uint64_t str_offsets_base;
    uint64_t rnglists_base;
    _upf_abbrev_vec abbrevs;
    _upf_scope scope;
    _upf_name_function_map extern_functions;
} _upf_cu;

_UPF_VECTOR_TYPEDEF(_upf_cu_vec, _upf_cu);

typedef struct {
    const uint8_t *die;
    _upf_ns_vec *nss;
} _upf_ns_import;

_UPF_VECTOR_TYPEDEF(_upf_ns_import_vec, _upf_ns_import);

_UPF_MAP_TYPEDEF(_upf_die_ns_map, const uint8_t *, _upf_ns *);
_UPF_MAP_TYPEDEF(_upf_die_nss_map, const uint8_t *, _upf_ns_vec);
_UPF_MAP_TYPEDEF(_upf_die_scope_map, const uint8_t *, _upf_scope *);

typedef struct {
    _upf_cu *cu;
    // All namespaces.
    _upf_die_ns_map nss;
    // Current namespaces.
    _upf_ns_vec ns_stack;
    // List of namespaces that need to be imported.
    _upf_ns_import_vec ns_imports;
    // List of declared functions that may be specified later.
    _upf_die_nss_map declared_functions;
    // List of specified functions that weren't declared yet.
    _upf_die_scope_map specified_functions;
} _upf_parsing_info;

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
    _UPF_TT_CXX_DECLTYPE,
    _UPF_TT_CXX_NOEXCEPT,
    _UPF_TT_CXX_TYPEID,

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
    const void *data;
    // Hashing `type` as pointer works because it is unique thanks to caching.
    const _upf_type *type;
} _upf_struct_key;

typedef struct {
    // Set if struct appears more than once.
    bool is_repeating;
    // Set if struct was printed.
    bool is_visited;
    // Set if struct was printed to reference later.
    uint32_t id;
} _upf_struct_info;

_UPF_MAP_TYPEDEF(_upf_struct_info_map, _upf_struct_key, _upf_struct_info);

_UPF_MAP_TYPEDEF(_upf_pc_cstr_map, uint64_t, const char *);
_UPF_MAP_TYPEDEF(_upf_die_type_map, const uint8_t *, _upf_type *);

// =================== GLOBAL STATE =======================

struct _upf_state {
    _upf_memory_region *allocator;
    _upf_cstr_vec ignored_structs;
    bool is_init;
    // Temporary global variable because expression cannot have the declaration.
    uint32_t map_index;
    // File loaded by dynamic linker (without debug info).
    const uint8_t *base;
    // mmap-ed file with debug info.
    uint8_t *elf_file;
    off_t elf_file_size;
    // DWARF info
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
    _upf_die_type_map type_map;
    _upf_cu_vec cus;
    _upf_pc_cstr_map extern_functions;
    // sequential id for enumerating repeating structs
    int struct_id;
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

    // Align to pointer size.
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
    _UPF_ASSERT(begin != NULL && end != NULL && end >= begin);

    size_t len = end - begin;
    char *string = (char *) _upf_alloc(len + 1);
    memcpy(string, begin, len);
    string[len] = '\0';

    return string;
}

static char *_upf_copy_string(const char *str) { return _upf_new_string(str, str + strlen(str)); }

// ====================== VECTOR ==========================

#define _UPF_INITIAL_VECTOR_CAPACITY 4

#ifdef __cplusplus
#define _UPF_VOID_CAST(T) (decltype(T))
#else
#define _UPF_VOID_CAST(T)
#endif

#define _UPF_VECTOR_PUSH(vec, element)                                  \
    do {                                                                \
        if ((vec)->capacity == 0) {                                     \
            (vec)->capacity = _UPF_INITIAL_VECTOR_CAPACITY;             \
            uint32_t size = (vec)->capacity * sizeof(*(vec)->data);     \
            (vec)->data = _UPF_VOID_CAST((vec)->data) _upf_alloc(size); \
        } else if ((vec)->capacity == (vec)->length) {                  \
            uint32_t old_size = (vec)->capacity * sizeof(*(vec)->data); \
            (vec)->capacity *= 2;                                       \
            void *new_data = _upf_alloc(old_size * 2);                  \
            memcpy(new_data, (vec)->data, old_size);                    \
            (vec)->data = _UPF_VOID_CAST((vec)->data) new_data;         \
        }                                                               \
        (vec)->data[(vec)->length++] = element;                         \
    } while (0)

#define _UPF_VECTOR_TOP(vec) (vec)->data[(vec)->length - 1]

// ===================== HASHMAP ==========================

#define _UPF_INITIAL_MAP_CAPACITY 6
#define _UPF_MAP_LOAD_FACTOR 0.75F

#define _UPF_OFFSET_OF(container_ptr, member_name) ((size_t) &((container_ptr)->member_name) - (size_t) (container_ptr))

// FNV-1a
static uint32_t _upf_hash(const void *data, size_t size) {
    const uint8_t *bytes = (const uint8_t *) data;
    uint32_t hash = 0x811c9dc5;
    for (size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= 0x01000193;
    }
    return hash;
}

static void _upf_map_impl_init(uint8_t **data, uint32_t *capacity, size_t entry_size) {
    if (*capacity > 0) return;

    *capacity = _UPF_INITIAL_MAP_CAPACITY;
    size_t size = *capacity * entry_size;
    *data = (uint8_t *) _upf_alloc(size);
    memset(*data, 0, size);
}

static uint32_t _upf_map_impl_hash_key(void *key, bool is_key_str, size_t key_size) {
    uint32_t hash;
    if (is_key_str) {
        const char *str = *((const char **) key);
        hash = _upf_hash(str, strlen(str));
    } else {
        hash = _upf_hash(key, key_size);
    }
    // Hash value 0 is reserved to mark unused entries.
    if (hash == 0) hash++;
    return hash;
}

static bool _upf_map_impl_keys_equal(const void *a, const void *b, bool is_key_str, uint32_t n) {
    if (!is_key_str) return memcmp(a, b, n) == 0;
    return strcmp(*((const char **) a), *((const char **) b)) == 0;
}

#define _UPF_MAP_GET_ENTRY(data, index, entry_size) (&(data)[(index) * (entry_size)])
#define _UPF_MAP_GET_HASH(data, index, entry_size) (*((uint32_t *) _UPF_MAP_GET_ENTRY(data, index, entry_size)))

// Find an element by the key, return whether it exists and write index to `result`.
static bool _upf_map_impl_find(const uint8_t *data, uint32_t capacity, size_t entry_size, void *key, bool is_key_str, size_t key_size,
                               size_t key_offset, uint32_t *result) {
    if (capacity == 0) return false;

    uint32_t index = _upf_map_impl_hash_key(key, is_key_str, key_size) % capacity;
    while (_UPF_MAP_GET_HASH(data, index, entry_size) > 0) {
        const uint8_t *current_key = &_UPF_MAP_GET_ENTRY(data, index, entry_size)[key_offset];
        if (_upf_map_impl_keys_equal(key, current_key, is_key_str, key_size)) {
            *result = index;
            return true;
        }
        index = (index + 1) % capacity;
    }
    *result = index;
    return false;
}

static void _upf_map_impl_set(uint8_t **data, uint32_t *size, uint32_t *capacity, size_t entry_size, void *key, bool is_key_str,
                              size_t key_size, size_t key_offset, void *value, size_t value_size, size_t value_offset) {
    _UPF_ASSERT(data != NULL && size != NULL && capacity != NULL && key != NULL && value != NULL);

    if (*size >= *capacity * _UPF_MAP_LOAD_FACTOR) {
        uint32_t new_capacity = *capacity * 2;
        size_t size = new_capacity * entry_size;
        uint8_t *new_data = (uint8_t *) _upf_alloc(size);
        memset(new_data, 0, size);

        // Rehash all the elements.
        for (uint32_t i = 0; i < *capacity; i++) {
            uint32_t hash = _UPF_MAP_GET_HASH(*data, i, entry_size);
            if (hash == 0) continue;

            uint32_t new_index = hash % new_capacity;
            while (_UPF_MAP_GET_HASH(new_data, new_index, entry_size) > 0) new_index = (new_index + 1) % new_capacity;
            memcpy(_UPF_MAP_GET_ENTRY(new_data, new_index, entry_size), _UPF_MAP_GET_ENTRY(*data, i, entry_size), entry_size);
        }
        *data = new_data;
        *capacity = new_capacity;
    }

    uint32_t index;
    bool exists = _upf_map_impl_find(*data, *capacity, entry_size, key, is_key_str, key_size, key_offset, &index);
    if (!exists) *size = *size + 1;

    uint8_t *new_entry = _UPF_MAP_GET_ENTRY(*data, index, entry_size);
    uint32_t hash = _upf_map_impl_hash_key(key, is_key_str, key_size);
    memcpy(new_entry, &hash, sizeof(hash));
    memcpy(&new_entry[key_offset], key, key_size);
    memcpy(&new_entry[value_offset], value, value_size);
}

#undef _UPF_MAP_GET_ENTRY
#undef _UPF_MAP_GET_HASH

// Returns pointer to the value if it exists, or NULL if it doesn't.
#define _UPF_MAP_IMPL_GET(map, search_key, is_key_str)                                                                            \
    ((map)->capacity > 0                                                                                                          \
         ? _upf_map_impl_find((uint8_t *) (map)->data, (map)->capacity, sizeof(*(map)->data), (void *) &(search_key), is_key_str, \
                              sizeof((map)->data->key), _UPF_OFFSET_OF((map)->data, key), &_upf_state.map_index)                  \
               ? &(map)->data[_upf_state.map_index].value                                                                         \
               : NULL                                                                                                             \
         : NULL)

// Initialize map before calling OFFSET_OF since it has a UB when container is NULL.
#define _UPF_MAP_IMPL_SET(map, entry_key, is_key_str, entry_value)                                                                         \
    (_upf_map_impl_init((uint8_t **) &(map)->data, &(map)->capacity, sizeof(*(map)->data)),                                                \
     _upf_map_impl_set((uint8_t **) &(map)->data, &(map)->size, &(map)->capacity, sizeof(*(map)->data), (void *) &(entry_key), is_key_str, \
                       sizeof((map)->data->key), _UPF_OFFSET_OF((map)->data, key), (void *) &(entry_value), sizeof((map)->data->value),    \
                       _UPF_OFFSET_OF((map)->data, value)))

#define _UPF_MAP_GET(map, search_key) _UPF_MAP_IMPL_GET(map, search_key, false)
#define _UPF_MAP_STR_GET(map, search_key) _UPF_MAP_IMPL_GET(map, search_key, true)

// Disable warning to allow storing pointers as values.
// NOLINTNEXTLINE(bugprone-sizeof-expression)
#define _UPF_MAP_SET(map, entry_key, entry_value) _UPF_MAP_IMPL_SET(map, entry_key, false, entry_value)
// NOLINTNEXTLINE(bugprone-sizeof-expression)
#define _UPF_MAP_STR_SET(map, entry_key, entry_value) _UPF_MAP_IMPL_SET(map, entry_key, true, entry_value)

#define _UPF_MAP_RESET(map)                                                                      \
    do {                                                                                         \
        (map)->size = 0;                                                                         \
        if ((map)->data != NULL) memset((map)->data, 0, sizeof(*(map)->data) * (map)->capacity); \
    } while (0)

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

static uint64_t _upf_offset_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);
    _UPF_ASSERT(_upf_state.offset_size <= sizeof(uint64_t));
    uint64_t offset = 0;
    memcpy(&offset, die, _upf_state.offset_size);
    return offset;
}

static uint64_t _upf_address_cast(const uint8_t *die) {
    _UPF_ASSERT(die != NULL);
    _UPF_ASSERT(_upf_state.address_size <= sizeof(uint64_t));
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
    _UPF_ERROR("Invalid DWARF attribute type: 0x%" PRIx64 ".", form);
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
    _UPF_ERROR("Invalid DWARF addrx/strx type: 0x%" PRIx64 ".", form);
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
    _UPF_ERROR("Invalid DWARF string type: 0x%" PRIx64 ".", form);
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
    _UPF_ERROR("Reference to different compilation units are not supported.");
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
        case DW_FORM_data16:         _UPF_ERROR("16 byte data blocks are not supported.");
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
    _UPF_ERROR("Invalid DWARF data type: 0x%" PRIx64 ".", attr.form);
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
    _UPF_ERROR("Invalid DWARF address type: 0x%" PRIx64 ".", form);
}

static bool _upf_get_flag(const uint8_t *die, uint64_t form) {
    if (form == DW_FORM_flag_present) return true;
    if (form == DW_FORM_flag) return *die;
    _UPF_ERROR("Invalid DWARF flag type: 0x%" PRIx64 ".", form);
}

static size_t _upf_get_abbrev(const _upf_abbrev **abbrev, const _upf_cu *cu, const uint8_t *die) {
    _UPF_ASSERT(abbrev != NULL && cu != NULL);
    uint64_t code;
    size_t offset = _upf_uLEB_to_uint64(die, &code);
    *abbrev = code == 0 ? NULL : &cu->abbrevs.data[code - 1];
    return offset;
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

static const uint8_t *_upf_skip_attrs(const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(die != NULL && abbrev != NULL);
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) die += _upf_get_attr_size(die, abbrev->attrs.data[i].form);
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
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &code);
        if (code == 0) break;

        _upf_abbrev abbrev = _UPF_ZERO_INIT;
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

static _upf_type_kind _upf_get_base_type_kind(int64_t encoding, int64_t size) {
    switch (encoding) {
        case DW_ATE_boolean:
            if (size == 1) return _UPF_TK_BOOL;
            _UPF_WARN("Expected boolean to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_address: _UPF_WARN("Segmented addresses are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_signed:
            switch (size) {
                case 1:  return _UPF_TK_S1;
                case 2:  return _UPF_TK_S2;
                case 4:  return _UPF_TK_S4;
                case 8:  return _UPF_TK_S8;
                case 16: _UPF_WARN("16 byte integers are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
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
                case 16: _UPF_WARN("16 byte unsigned integers are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
                default: _UPF_WARN("Expected integer to be 1, 2, 4, 8 or 16 bytes long. Ignoring this type."); return _UPF_TK_UNKNOWN;
            }
        case DW_ATE_unsigned_char:
            if (size == 1) return _UPF_TK_UCHAR;
            _UPF_WARN("Expected char to be 1 byte long. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_ASCII:
        case DW_ATE_UCS:   _UPF_WARN("UCS and ASCII DWARF types are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_UTF:
            if (size == 1) return _UPF_TK_UCHAR;
            _UPF_WARN("UTF-16 and UTF-32 are not supported. Ignoring this type.");
            return _UPF_TK_UNKNOWN;
        case DW_ATE_signed_fixed:
        case DW_ATE_unsigned_fixed: _UPF_WARN("C shouldn't have fixed-point decimals. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_float:
            switch (size) {
                case 4:  return _UPF_TK_F4;
                case 8:  return _UPF_TK_F8;
                case 16: _UPF_WARN("16 byte floats are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
                default: _UPF_WARN("Expected floats to be 4, 8 or 16 bytes long. Ignoring this type."); return _UPF_TK_UNKNOWN;
            }
        case DW_ATE_complex_float:   _UPF_WARN("Complex floats are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_imaginary_float: _UPF_WARN("Imaginary floats are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_decimal_float:   _UPF_WARN("Decimal floats are not supported. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_packed_decimal:  _UPF_WARN("C shouldn't have packed decimals. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_numeric_string:  _UPF_WARN("C shouldn't have numeric strings. Ignoring this type."); return _UPF_TK_UNKNOWN;
        case DW_ATE_edited:          _UPF_WARN("C shouldn't have edited strings. Ignoring this type."); return _UPF_TK_UNKNOWN;
        default:                     _UPF_WARN("Skipping unknown DWARF type encoding (0x%02" PRIx64 ")", encoding); return _UPF_TK_UNKNOWN;
    }
}

static int _upf_get_type_modifier(uint64_t tag) {
    switch (tag) {
        case DW_TAG_const_type:    return _UPF_MOD_CONST;
        case DW_TAG_volatile_type: return _UPF_MOD_VOLATILE;
        case DW_TAG_restrict_type: return _UPF_MOD_RESTRICT;
        case DW_TAG_atomic_type:   return _UPF_MOD_ATOMIC;
    }
    _UPF_ERROR("Invalid DWARF type modifier: 0x%" PRIx64 ".", tag);
}

static _upf_type *_upf_new_type(_upf_type type) {
    _upf_type *type_ptr = (_upf_type *) _upf_alloc(sizeof(type));
    memcpy(type_ptr, &type, sizeof(type));
    return type_ptr;
}

static _upf_type *_upf_new_type2(const uint8_t *type_die, _upf_type type) {
    _UPF_ASSERT(type_die != NULL);

    _upf_type **opt_type_ptr = _UPF_MAP_GET(&_upf_state.type_map, type_die);
    if (opt_type_ptr != NULL) return *opt_type_ptr;

    _upf_type *type_ptr = (_upf_type *) _upf_alloc(sizeof(type));
    memcpy(type_ptr, &type, sizeof(type));
    _UPF_MAP_SET(&_upf_state.type_map, type_die, type_ptr);
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

static bool _upf_is_pointer(const _upf_type *type) {
    _UPF_ASSERT(type != NULL);
    if (type->kind == _UPF_TK_POINTER || type->kind == _UPF_TK_ARRAY) return true;
    return false;
}

static _upf_type *_upf_dereference_type(const _upf_type *type_ptr) {
    _UPF_ASSERT(_upf_is_pointer(type_ptr));
    if (type_ptr->kind == _UPF_TK_POINTER) return type_ptr->as.pointer.type;
    if (type_ptr->as.array.lengths.length <= 1) return type_ptr->as.array.element_type;
    return _upf_new_type(_upf_get_subarray(type_ptr, 1));
}

static _upf_type *_upf_get_reference_to_type(_upf_type *type_ptr) {
    _UPF_ASSERT(type_ptr != NULL);
    _upf_type type = _UPF_ZERO_INIT;
    type.kind = _UPF_TK_REFERENCE;
    type.as.reference.type = type_ptr;
    return _upf_new_type(type);
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

static _upf_named_type _upf_parse_variable(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

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

static const uint8_t *_upf_get_import_die(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        if (attr.name == DW_AT_import) return cu->base + _upf_get_ref(die, attr.form);
        die += _upf_get_attr_size(die, attr.form);
    }
    _UPF_ERROR("Import entry must have DW_AT_import");
}

static _upf_function _upf_parse_subprogram(const _upf_cu *cu, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(cu != NULL && die != NULL && abbrev != NULL);

    _upf_function function = _UPF_ZERO_INIT;
    function.pc = UINT64_MAX;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:         function.name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_linkage_name: function.linkage_name = _upf_get_str(cu, die, attr.form); break;
            case DW_AT_type:         function.return_type_die = cu->base + _upf_get_ref(die, attr.form); break;
            case DW_AT_low_pc:       function.pc = _upf_get_addr(cu, die, attr.form); break;
            case DW_AT_ranges:       {
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
    if (function.linkage_name == NULL) function.linkage_name = function.name;

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

    _upf_type **opt_type_ptr = _UPF_MAP_GET(&_upf_state.type_map, die);
    if (opt_type_ptr != NULL) return *opt_type_ptr;

    const uint8_t *die_base = die;

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, cu, die);

    const char *name = NULL;
    uint64_t subtype_offset = UINT64_MAX;
    size_t size = SIZE_MAX;
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
                    _UPF_WARN("Non-constant type sizes are not supported. Marking size as unknown.");
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
                    _UPF_WARN("Enumerator array subranges are not supported. Ignoring this type.");
                    goto unknown_type;
                }
                _UPF_ASSERT(abbrev->tag == DW_TAG_subrange_type);

                size_t length = SIZE_MAX;
                for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
                    _upf_attr attr = abbrev->attrs.data[i];
                    switch (attr.name) {
                        case DW_AT_count:
                        case DW_AT_upper_bound:
                            if (_upf_is_data(attr.form)) {
                                length = _upf_get_data(die, attr);
                                if (attr.name == DW_AT_upper_bound) length++;
                            } else {
                                _UPF_WARN("Non-constant array lengths are not supported. Marking it unknown.");
                            }
                            break;
                    }
                    die += _upf_get_attr_size(die, attr.form);
                }

                if (length == SIZE_MAX) {
                    is_static = false;
                    array_size = SIZE_MAX;
                    type.as.array.lengths.length = 0;
                }

                if (is_static) {
                    array_size *= length;
                    _UPF_VECTOR_PUSH(&type.as.array.lengths, length);
                }

                if (generate_name) brackets++;
            }

            if (element_type->size != SIZE_MAX && type.size == SIZE_MAX) {
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
            type.size = size == SIZE_MAX ? type.as.cenum.underlying_type->size : size;

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
                                _UPF_WARN("Non-constant enum values are not supported. Ignoring this type.");
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
            type.as.reference.is_rvalue = abbrev->tag == DW_TAG_rvalue_reference_type;

            _upf_type *type_ptr = _upf_new_type2(die_base, type);
            type_ptr->as.reference.type = _upf_parse_type(cu, cu->base + subtype_offset);
            return type_ptr;
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
                        member.offset = SIZE_MAX;

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
                                        _UPF_WARN("Non-constant member offsets are not supported. Skipping this field.");
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
                                        _UPF_WARN("Non-constant bit field sizes are not supported. Skipping this field.");
                                        add_member = false;
                                    }
                                    break;
                            }
                            die += _upf_get_attr_size(die, attr.form);
                        }
                        if (add_member) {
                            _UPF_ASSERT(member.type != NULL && member.offset != SIZE_MAX);
                            if (member.name == NULL) member.name = "<anonymous>";
                            _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
                        }
                    } break;
                    case DW_TAG_subprogram: {
                        _upf_method method = _UPF_ZERO_INIT;
                        method.function = _upf_parse_subprogram(cu, die, abbrev);
                        _UPF_VECTOR_PUSH(&type.as.cstruct.methods, method);
                        die = _upf_skip_die(cu, die, abbrev);
                    } break;
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
                                        _UPF_WARN("Non-constant member offsets are not supported. Skipping this field.");
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
                                member.inheritance++;
                                _UPF_VECTOR_PUSH(&type.as.cstruct.members, member);
                            }
                            for (uint32_t i = 0; i < parent_type->as.cstruct.methods.length; i++) {
                                _upf_method method = parent_type->as.cstruct.methods.data[i];
                                method.inheritance++;
                                _UPF_VECTOR_PUSH(&type.as.cstruct.methods, method);
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
                                _UPF_WARN("Non-constant bit field sizes are not supported. Skipping this field.");
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
            _UPF_ASSERT(name != NULL && size != SIZE_MAX && encoding != 0);

            _upf_type type = _UPF_ZERO_INIT;
            type.name = name;
            type.kind = _upf_get_base_type_kind(encoding, size);
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
        default: _UPF_WARN("Found unsupported type (0x%" PRIx64 "). Ignoring it.", abbrev->tag); break;
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

static void _upf_parse_inheritance(_upf_parsing_info *p, const uint8_t *die, const _upf_abbrev *abbrev, _upf_ns_vec *nss) {
    _UPF_ASSERT(p != NULL && die != NULL && abbrev != NULL && nss != NULL);

    const uint8_t *parent_die = NULL;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        if (attr.name == DW_AT_type) {
            parent_die = p->cu->base + _upf_get_ref(die, attr.form);
            break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }
    _UPF_ASSERT(parent_die != NULL);

    const _upf_abbrev *parent_abbrev;
    _upf_get_abbrev(&parent_abbrev, p->cu, parent_die);
    _UPF_ASSERT(parent_abbrev != NULL);

    // Add the import only if inherited class is not empty.
    if (!parent_abbrev->has_children) return;

    _upf_ns_import import = _UPF_ZERO_INIT;
    import.nss = nss;
    import.die = parent_die;
    _UPF_VECTOR_PUSH(&p->ns_imports, import);
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

static _upf_scope *_upf_parse_scope(_upf_parsing_info *p, const uint8_t *die, const _upf_abbrev *abbrev);

static const uint8_t *_upf_parse_scope_die(_upf_parsing_info *p, _upf_scope *scope, const uint8_t *die) {
    _UPF_ASSERT(p != NULL && scope != NULL && die != NULL);

    const uint8_t *die_base = die;
    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, p->cu, die);
    if (abbrev == NULL) return NULL;

    switch (abbrev->tag) {
        case DW_TAG_variable:
        case DW_TAG_formal_parameter: {
            _upf_named_type var = _upf_parse_variable(p->cu, die, abbrev);
            if (var.name == NULL) break;
            _UPF_ASSERT(var.die != NULL);
            _UPF_MAP_STR_SET(&scope->vars, var.name, var.die);
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
            const char *type_name = _upf_get_type_name(p->cu, die, abbrev);
            if (type_name != NULL) _UPF_MAP_STR_SET(&scope->types, type_name, die_base);
        } break;
        case DW_TAG_lexical_block:
        case DW_TAG_inlined_subroutine: {
            _upf_scope *sub_scope = _upf_parse_scope(p, die, abbrev);
            if (sub_scope != NULL) _UPF_VECTOR_PUSH(&scope->scopes, sub_scope);
        } break;
        case DW_TAG_imported_module: {
            _upf_ns_import import = _UPF_ZERO_INIT;
            import.nss = &scope->nss;
            import.die = _upf_get_import_die(p->cu, die, abbrev);
            _UPF_VECTOR_PUSH(&p->ns_imports, import);
        } break;
        case DW_TAG_imported_declaration: _upf_parse_scope_die(p, scope, _upf_get_import_die(p->cu, die, abbrev)); break;
    }
    return _upf_skip_die(p->cu, die, abbrev);
}

static _upf_scope *_upf_parse_scope(_upf_parsing_info *p, const uint8_t *die, const _upf_abbrev *abbrev) {
    _UPF_ASSERT(p != NULL && die != NULL && abbrev != NULL);

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

    _upf_range_vec ranges = _upf_get_die_ranges(p->cu, low_pc_die, low_pc_attr, high_pc_die, high_pc_attr, ranges_die, ranges_attr);
    if (ranges.length == 0) return NULL;

    _upf_scope *scope = (_upf_scope *) _upf_alloc(sizeof(*scope));
    memset(scope, 0, sizeof(*scope));
    scope->ranges = ranges;

    if (abbrev->has_children) {
        while (die != NULL) die = _upf_parse_scope_die(p, scope, die);
    }

    return scope;
}

static void _upf_add_current_nss(_upf_parsing_info *p, _upf_ns_vec *nss) {
    _UPF_ASSERT(p != NULL && nss != NULL);
    for (uint32_t i = 0; i < p->ns_stack.length; i++) {
        _UPF_VECTOR_PUSH(nss, p->ns_stack.data[p->ns_stack.length - 1 - i]);
    }
}

static void _upf_add_function_scope(_upf_parsing_info *p, const uint8_t *die_base, _upf_function function, _upf_scope *scope) {
    _UPF_ASSERT(p != NULL && die_base != NULL);

    if (scope == NULL) {
        // Function doesn't have a scope, look for its specification.
        _upf_scope **opt_scope = _UPF_MAP_GET(&p->specified_functions, die_base);
        if (opt_scope != NULL) {
            scope = *opt_scope;
            _upf_add_current_nss(p, &scope->nss);
            _UPF_VECTOR_PUSH(&p->cu->scope.scopes, scope);
            return;
        }

        // There is none, save the declaration.
        _upf_ns_vec nss = _UPF_ZERO_INIT;
        _upf_add_current_nss(p, &nss);
        _UPF_MAP_SET(&p->declared_functions, die_base, nss);
        return;
    }

    // Function has a scope and isn't a specification, save it.
    if (function.specification_die == NULL) {
        _upf_add_current_nss(p, &scope->nss);
        _UPF_VECTOR_PUSH(&p->cu->scope.scopes, scope);
        return;
    }

    // Function is a specification, look for the matching declaration.
    _upf_ns_vec *opt_nss = _UPF_MAP_GET(&p->declared_functions, function.specification_die);
    if (opt_nss != NULL) {
        scope->nss = *opt_nss;
        _UPF_VECTOR_PUSH(&p->cu->scope.scopes, scope);
        return;
    }

    // There is no matching declaration, save the specification.
    _UPF_MAP_SET(&p->specified_functions, die_base, scope);
}

static _upf_ns *_upf_parse_ns_body(_upf_parsing_info *p, const uint8_t *die);

static void _upf_parse_ns(_upf_parsing_info *p, const uint8_t *die, _upf_ns *current_ns) {
    _UPF_ASSERT(p != NULL && die != NULL && current_ns != NULL);

    const uint8_t *die_base = die;

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, p->cu, die);
    _UPF_ASSERT(abbrev != NULL && abbrev->tag == DW_TAG_namespace);

    const char *name = NULL;
    bool export_symbols = false;
    for (uint32_t i = 0; i < abbrev->attrs.length; i++) {
        _upf_attr attr = abbrev->attrs.data[i];
        switch (attr.name) {
            case DW_AT_name:           name = _upf_get_str(p->cu, die, attr.form); break;
            case DW_AT_export_symbols: export_symbols = _upf_get_flag(die, attr.form); break;
        }
        die += _upf_get_attr_size(die, attr.form);
    }

    // Add namespace even if it doesn't have any children in order to resolve the import.
    _upf_ns *sub_ns = abbrev->has_children ? _upf_parse_ns_body(p, die) : NULL;
    if (name == NULL || export_symbols) _UPF_VECTOR_PUSH(&current_ns->imported_nss, sub_ns);
    if (name != NULL) _UPF_MAP_STR_SET(&current_ns->sub_nss, name, sub_ns);
    _UPF_MAP_SET(&p->nss, die_base, sub_ns);
}

static const uint8_t *_upf_parse_ns_die(_upf_parsing_info *p, _upf_ns *ns, const uint8_t *die) {
    _UPF_ASSERT(p != NULL && ns != NULL && die != NULL);

    const uint8_t *die_base = die;
    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, p->cu, die);
    if (abbrev == NULL) return NULL;

    switch (abbrev->tag) {
        case DW_TAG_member:
        case DW_TAG_variable: {
            _upf_named_type var = _upf_parse_variable(p->cu, die, abbrev);
            if (var.name == NULL) break;
            _UPF_ASSERT(var.die != NULL);
            _UPF_MAP_STR_SET(&ns->vars, var.name, var.die);
        } break;
        case DW_TAG_class_type:
        case DW_TAG_structure_type:
        case DW_TAG_union_type:     {
            const char *type_name = _upf_get_type_name(p->cu, die, abbrev);
            if (type_name == NULL) break;
            _UPF_MAP_STR_SET(&ns->types, type_name, die_base);

            // If class/struct has children, parse it as a namespace.
            if (!abbrev->has_children) break;

            _upf_ns *class_ns = _upf_parse_ns_body(p, _upf_skip_attrs(die, abbrev));
            _UPF_MAP_STR_SET(&ns->sub_nss, type_name, class_ns);
            _UPF_MAP_SET(&p->nss, die_base, class_ns);
        } break;
        // Treat inheritance as namespace import.
        case DW_TAG_inheritance:           _upf_parse_inheritance(p, die, abbrev, &ns->imported_nss); break;
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
            const char *type_name = _upf_get_type_name(p->cu, die, abbrev);
            if (type_name != NULL) _UPF_MAP_STR_SET(&ns->types, type_name, die_base);
        } break;
        case DW_TAG_subprogram: {
            _upf_function function = _upf_parse_subprogram(p->cu, die, abbrev);
            _upf_scope *scope = _upf_parse_scope(p, die, abbrev);
            _upf_add_function_scope(p, die_base, function, scope);

            if (function.name == NULL) break;
            _UPF_MAP_STR_SET(&ns->functions, function.name, function);
            if (function.is_external) _UPF_MAP_STR_SET(&p->cu->extern_functions, function.name, function);
        } break;
        case DW_TAG_namespace:       _upf_parse_ns(p, die_base, ns); break;
        case DW_TAG_imported_module: {
            _upf_ns_import import = _UPF_ZERO_INIT;
            import.nss = &ns->imported_nss;
            import.die = _upf_get_import_die(p->cu, die, abbrev);
            _UPF_VECTOR_PUSH(&p->ns_imports, import);
        } break;
        case DW_TAG_imported_declaration: _upf_parse_ns_die(p, ns, _upf_get_import_die(p->cu, die, abbrev)); break;
    }
    return _upf_skip_die(p->cu, die, abbrev);
}

static _upf_ns *_upf_parse_ns_body(_upf_parsing_info *p, const uint8_t *die) {
    _UPF_ASSERT(p != NULL && die != NULL);

    _upf_ns *ns = (_upf_ns *) _upf_alloc(sizeof(*ns));
    memset(ns, 0, sizeof(*ns));
    _UPF_VECTOR_PUSH(&p->ns_stack, ns);

    while (die != NULL) die = _upf_parse_ns_die(p, ns, die);

    _UPF_ASSERT(p->ns_stack.length > 0);
    p->ns_stack.length--;
    return ns;
}

static void _upf_resolve_ns_imports(_upf_parsing_info *p) {
    _UPF_ASSERT(p != NULL);
    for (uint32_t i = 0; i < p->ns_imports.length; i++) {
        _upf_ns **opt_ns = _UPF_MAP_GET(&p->nss, p->ns_imports.data[i].die);
        if (opt_ns == NULL) _UPF_ERROR("Failed to resolve namespace import.");
        _UPF_VECTOR_PUSH(p->ns_imports.data[i].nss, *opt_ns);
    }
}

static bool _upf_is_language_c(int64_t language) {
    switch (language) {
        case DW_LANG_C:
        case DW_LANG_C89:
        case DW_LANG_C99:
        case DW_LANG_C11:
        case DW_LANG_C17:
        case DW_LANG_C23: return true;
        default:          return false;
    }
}

static bool _upf_is_language_cxx(int64_t language) {
    switch (language) {
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

static void _upf_parse_cu(const uint8_t *cu_base, const uint8_t *die, const uint8_t *abbrev_table) {
    _UPF_ASSERT(cu_base != NULL && die != NULL && abbrev_table != NULL);

    _upf_cu cu = _UPF_ZERO_INIT;
    cu.base = cu_base;
    cu.str_offsets_base = UINT64_MAX;
    cu.rnglists_base = UINT64_MAX;
    cu.abbrevs = _upf_parse_abbrevs(abbrev_table);

    const _upf_abbrev *abbrev;
    die += _upf_get_abbrev(&abbrev, &cu, die);
    _UPF_ASSERT(abbrev != NULL && abbrev->tag == DW_TAG_compile_unit);

    // Save to parse after initializing CU.
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
            case DW_AT_language: {
                int64_t lang = _upf_get_data(die, attr);
                if (_upf_is_language_c(lang)) {
                    break;
                } else if (_upf_is_language_cxx(lang)) {
                    cu.is_cxx = true;
                    break;
                } else {
                    // Skip CU if it is not C or C++.
                    return;
                }
            } break;
        }

        die += _upf_get_attr_size(die, attr.form);
    }

    if (low_pc_die != NULL) cu.base_address = _upf_get_addr(&cu, low_pc_die, low_pc_attr.form);

    cu.scope.ranges = _upf_get_die_ranges(&cu, low_pc_die, low_pc_attr, high_pc_die, high_pc_attr, ranges_die, ranges_attr);
    if (cu.scope.ranges.length == 0) return;

    _upf_parsing_info parsing_info = _UPF_ZERO_INIT;
    parsing_info.cu = &cu;

    _upf_ns *global_ns = _upf_parse_ns_body(&parsing_info, die);
    _upf_resolve_ns_imports(&parsing_info);

    _UPF_VECTOR_PUSH(&cu.scope.nss, global_ns);
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

        bool is64bit = false;
        if (length == UINT32_MAX) {
            memcpy(&length, die, sizeof(uint64_t));
            die += sizeof(uint64_t);
            is64bit = true;
        }
        _upf_state.offset_size = is64bit ? 8 : 4;

        const uint8_t *next = die + length;

        uint16_t version = 0;
        memcpy(&version, die, sizeof(version));
        die += sizeof(version);
        if (version != 5) _UPF_ERROR("uprintf only supports DWARF version 5.");

        uint8_t type = *die;
        die += sizeof(type);
        if (type != DW_UT_compile) _UPF_ERROR("uprintf does NOT support split debug information.");

        _upf_state.address_size = *die;
        die += sizeof(_upf_state.address_size);

        const uint8_t *abbrev_table = _upf_state.abbrev + _upf_offset_cast(die);
        die += _upf_state.offset_size;

        _upf_parse_cu(cu_base, die, abbrev_table);

        die = next;
    }
}

// ================= EXTERN FUNCTIONS =====================

// Extern function mapping: function pointer -> GOT -> RELA -> symbol -> name -> DIE.
static void _upf_parse_extern_functions(void) {
    _UPF_ASSERT(_DYNAMIC != NULL);

    const char *string_table = NULL;
    const ElfW(Sym) *symbol_table = NULL;
    const ElfW(Rela) *rela_table = NULL;
    int rela_size = -1;
    for (const ElfW(Dyn) *dyn = _DYNAMIC; dyn->d_tag != DT_NULL; dyn++) {
        switch (dyn->d_tag) {
            case DT_STRTAB:  string_table = (char *) dyn->d_un.d_ptr; break;
            case DT_SYMTAB:  symbol_table = (ElfW(Sym) *) dyn->d_un.d_ptr; break;
            case DT_RELA:    rela_table = (ElfW(Rela) *) dyn->d_un.d_ptr; break;
            case DT_RELASZ:  rela_size = dyn->d_un.d_val / sizeof(ElfW(Rela)); break;
            case DT_RELAENT: _UPF_ASSERT(dyn->d_un.d_val == sizeof(ElfW(Rela))); break;
        }
    }
    if (string_table == NULL || symbol_table == NULL || rela_table == NULL || rela_size == -1) {
        _UPF_WARN("Failed to find all required ELF sections. Ignoring extern functions.");
        return;
    }

    for (int i = 0; i < rela_size; i++) {
        ElfW(Rela) rela = rela_table[i];

        int symbol_idx = ELF64_R_SYM(rela.r_info);
        if (symbol_idx == STN_UNDEF) continue;

        ElfW(Sym) symbol = symbol_table[symbol_idx];
        uint64_t symbol_address = *((uint64_t *) (_upf_state.base + rela.r_offset));
        const char *symbol_name = string_table + symbol.st_name;
        _UPF_MAP_SET(&_upf_state.extern_functions, symbol_address, symbol_name);
    }
}

static _upf_function *_upf_get_extern_function(uint64_t absolute_pc) {
    const char **opt_function_name = _UPF_MAP_GET(&_upf_state.extern_functions, absolute_pc);
    if (opt_function_name == NULL) return NULL;
    return _UPF_MAP_STR_GET(&_upf_state.current_cu->extern_functions, *opt_function_name);
}

// ======================= ELF ============================

static void _upf_parse_elf(void) {
    struct stat file_info;
    if (stat("/proc/self/exe", &file_info) == -1) _UPF_ERROR("Failed to stat \"/proc/self/exe\": %s.", strerror(errno));
    size_t size = file_info.st_size;

    int fd = open("/proc/self/exe", O_RDONLY);
    if (fd == -1) _UPF_ERROR("Failed to open \"/proc/self/exe\": %s.", strerror(errno));

    // A new instance of file must be mmap-ed because the one loaded in memory
    // only contains information needed at runtime, and doesn't include debug
    // information, section table, etc.
    uint8_t *file = (uint8_t *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    _UPF_ASSERT(file != MAP_FAILED);

    _upf_state.elf_file = file;
    _upf_state.elf_file_size = size;

    close(fd);

    const ElfW(Ehdr) *header = (ElfW(Ehdr) *) file;

    if (memcmp(header->e_ident, ELFMAG, SELFMAG) != 0 ||  //
        header->e_ident[EI_CLASS] != ELFCLASS64 ||        //
        header->e_ident[EI_VERSION] != 1 ||               //
        header->e_machine != EM_X86_64 ||                 //
        header->e_version != 1 ||                         //
        header->e_shentsize != sizeof(ElfW(Shdr))) {
        _UPF_ERROR("Unsupported or invalid ELF file.");
    }

    const ElfW(Shdr) *string_section = (ElfW(Shdr) *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);

    const ElfW(Shdr) *section = (ElfW(Shdr) *) (file + header->e_shoff);
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

static const _upf_token *_upf_find_keyword(const _upf_token *keywords, size_t length, const char *str, size_t str_len) {
    _UPF_ASSERT(keywords != NULL && str != NULL);
    for (size_t i = 0; i < length; i++) {
        if (str_len == strlen(keywords[i].string) && strncmp(str, keywords[i].string, str_len) == 0) return &keywords[i];
    }
    return NULL;
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
    };

    static const _upf_token cxx_keywords[] = {
        {_UPF_TT_STRUCT, "class"},          {_UPF_TT_ALIGNOF, "alignas"},
        {_UPF_TT_CXX_CAST, "static_cast"},  {_UPF_TT_CXX_CAST, "reinterpret_cast"},
        {_UPF_TT_CXX_CAST, "dynamic_cast"}, {_UPF_TT_CXX_CAST, "const_cast"},
        {_UPF_TT_CXX_NEW, "new"},           {_UPF_TT_CXX_DECLTYPE, "decltype"},
        {_UPF_TT_CXX_NOEXCEPT, "noexcept"}, {_UPF_TT_CXX_TYPEID, "typeid"},
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
            size_t str_len = end - ch;

            const _upf_token *keyword = _upf_find_keyword(keywords, sizeof(keywords) / sizeof(*keywords), ch, str_len);
            if (keyword == NULL && _upf_state.current_cu->is_cxx) {
                keyword = _upf_find_keyword(cxx_keywords, sizeof(cxx_keywords) / sizeof(*cxx_keywords), ch, str_len);
            }

            if (keyword == NULL) {
                _upf_new_token(_UPF_TT_IDENTIFIER, _upf_new_string(ch, end));
            } else {
                _UPF_VECTOR_PUSH(&_upf_state.tokens, *keyword);
            }

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

// ================== TYPE INFERENCE ======================

static _upf_ns *_upf_ns_find_sub_ns(_upf_ns *ns, const char *name) {
    _UPF_ASSERT(ns != NULL && name != NULL);
    if (ns->is_visited) return NULL;

    _upf_ns **opt_ns = _UPF_MAP_STR_GET(&ns->sub_nss, name);
    if (opt_ns != NULL) return *opt_ns;

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

static void _upf_ns_queue_push(_upf_ns_queue *queue, _upf_ns *element) {
    _UPF_ASSERT(queue != NULL && element != NULL);

    _upf_ns_node *node = (_upf_ns_node *) _upf_alloc(sizeof(*node));
    node->value = element;
    node->prev = NULL;

    if (queue->tail == NULL) {
        queue->head = node;
    } else {
        queue->tail->prev = node;
    }
    queue->tail = node;
}

static _upf_ns *_upf_ns_queue_pop(_upf_ns_queue *queue) {
    _UPF_ASSERT(queue != NULL);
    if (queue->head == NULL) return NULL;

    _upf_ns *result = queue->head->value;
    if (queue->tail == queue->head) {
        queue->tail = queue->head = NULL;
    } else {
        queue->head = queue->head->prev;
    }
    return result;
}

typedef void *(*_upf_ns_cb)(const _upf_ns *ns, const void *data);
typedef void *(*_upf_scope_cb)(const _upf_scope *scope, const void *data);

static void *_upf_ns_bfs(_upf_ns *ns, _upf_ns_cb callback, const void *data) {
    _UPF_ASSERT(ns != NULL && callback != NULL);

    _upf_ns_queue queue = _UPF_ZERO_INIT;
    _upf_ns_queue visited = _UPF_ZERO_INIT;

    ns->is_visited = true;
    _upf_ns_queue_push(&queue, ns);
    _upf_ns_queue_push(&visited, ns);

    void *result = NULL;
    _upf_ns *current;
    while ((current = _upf_ns_queue_pop(&queue)) != NULL) {
        result = callback(current, data);
        if (result != NULL) break;

        for (uint32_t i = 0; i < current->imported_nss.length; i++) {
            _upf_ns *next = current->imported_nss.data[i];
            if (next->is_visited) continue;

            next->is_visited = true;
            _upf_ns_queue_push(&queue, next);
            _upf_ns_queue_push(&visited, next);
        }
    }

    while ((current = _upf_ns_queue_pop(&visited)) != NULL) current->is_visited = false;

    return result;
}

static void *_upf_scope_search(bool search_global_scope, const _upf_cstr_vec *ns_names, _upf_ns_cb ns_callback,
                               _upf_scope_cb scope_callback, const void *data) {
    if (search_global_scope) {
        _upf_ns *ns = _upf_scope_resolve_ns(ns_names, &_upf_state.current_cu->scope);
        return _upf_ns_bfs(ns, ns_callback, data);
    }

    if (ns_names->length > 0) {
        _upf_ns *ns = NULL;
        for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
            ns = _upf_scope_resolve_ns(ns_names, _upf_state.current_scopes.data[i]);
            if (ns != NULL) break;
        }
        if (ns == NULL) _UPF_ERROR("Failed to find namespace at %s:%d.", _upf_state.file_path, _upf_state.line);
        return _upf_ns_bfs(ns, ns_callback, data);
    }

    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];

        void *result = scope_callback(scope, data);
        if (result != NULL) return result;

        for (uint32_t j = 0; j < scope->nss.length; j++) {
            result = _upf_ns_bfs(scope->nss.data[j], ns_callback, data);
            if (result != NULL) return result;
        }
    }
    return NULL;
}

static void *_upf_ns_get_identifier_cb(const _upf_ns *ns, const void *data) {
    _UPF_ASSERT(ns != NULL && data != NULL);

    const uint8_t **opt_var_die = _UPF_MAP_STR_GET(&ns->vars, data);
    if (opt_var_die != NULL) return _upf_parse_type(_upf_state.current_cu, *opt_var_die);

    const _upf_function *opt_function = _UPF_MAP_STR_GET(&ns->functions, data);
    if (opt_function != NULL) return _upf_get_function_type(opt_function);
    return NULL;
}

static void *_upf_scope_get_identifier_cb(const _upf_scope *scope, const void *data) {
    _UPF_ASSERT(scope != NULL && data != NULL);
    const uint8_t **opt_var_die = _UPF_MAP_STR_GET(&scope->vars, data);
    if (opt_var_die != NULL) return _upf_parse_type(_upf_state.current_cu, *opt_var_die);
    return NULL;
}

static void *_upf_ns_get_type_cb(const _upf_ns *ns, const void *data) {
    _UPF_ASSERT(ns != NULL && data != NULL);
    const uint8_t **opt_type_die = _UPF_MAP_STR_GET(&ns->types, data);
    if (opt_type_die != NULL) return _upf_parse_type(_upf_state.current_cu, *opt_type_die);
    return NULL;
}

static void *_upf_scope_get_type_cb(const _upf_scope *scope, const void *data) {
    _UPF_ASSERT(scope != NULL && data != NULL);
    const uint8_t **opt_type_die = _UPF_MAP_STR_GET(&scope->types, data);
    if (opt_type_die != NULL) return _upf_parse_type(_upf_state.current_cu, *opt_type_die);
    return NULL;
}

static void *_upf_ns_get_function_by_pc_cb(const _upf_ns *ns, const void *data) {
    _UPF_ASSERT(ns != NULL && data != NULL);
    uint64_t pc = *((const uint64_t *) data);
    for (uint32_t i = 0; i < ns->functions.capacity; i++) {
        if (ns->functions.data[i].hash == 0) continue;
        if (ns->functions.data[i].value.pc == pc) return &ns->functions.data[i].value;
    }
    return NULL;
}

static _upf_function *_upf_get_function_by_pc(uint64_t pc) {
    for (uint32_t i = 0; i < _upf_state.current_scopes.length; i++) {
        _upf_scope *scope = _upf_state.current_scopes.data[i];
        for (uint32_t j = 0; j < scope->nss.length; j++) {
            _upf_function *function = (_upf_function *) _upf_ns_bfs(scope->nss.data[j], _upf_ns_get_function_by_pc_cb, &pc);
            if (function != NULL) return function;
        }
    }
    return NULL;
}

// ====================== PARSER ==========================

// Pratt parser of C/C++ expressions.
// C grammar: https://www.quut.com/c/ANSI-C-grammar-y.html

static _upf_parse_rule _upf_parse_rules[_UPF_TT_COUNT] = _UPF_ZERO_INIT;

static _upf_type *_upf_parse(_upf_parse_precedence precedence);

static _upf_type *_upf_parse_expression(void) { return _upf_parse(_UPF_PREC_ASSIGNMENT); }

typedef struct {
    int pointers;
    bool reference;
    bool failed;
    _upf_type *function_type;
    _upf_type **function_return_type;
} _upf_abstract_declarator;

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
    _upf_cstr_vec ns_names = _UPF_ZERO_INIT;
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
                    _UPF_VECTOR_PUSH(&ns_names, identifier);
                    identifier = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
                }
                if (_upf_match_token(_UPF_TT_LESS_THAN)) {
                    _UPF_ERROR("Cast to templated type is not supported at %s:%d.", _upf_state.file_path, _upf_state.line);
                }
                break;
            case _UPF_TT_CXX_DECLTYPE: _UPF_ERROR("decltype is not supported at %s:%d.", _upf_state.file_path, _upf_state.line);
            default:                   parsed_type = true; break;
        }
    }
    if (identifier == NULL && type_specifiers_idx == 0) return NULL;

    _upf_abstract_declarator abstract_declarator = _upf_parse_abstract_declarator();

    _upf_type *type_ptr;
    if (identifier != NULL) {
        _UPF_ASSERT(type_specifiers_idx == 0);
        type_ptr = (_upf_type *) _upf_scope_search(leading_scope_op, &ns_names, _upf_ns_get_type_cb, _upf_scope_get_type_cb, identifier);
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
                        _UPF_WARN("Long doubles are not supported. Ignoring this type.");
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
            type.kind = _upf_get_base_type_kind(is_signed ? DW_ATE_signed : DW_ATE_unsigned, type.size);
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
    const char *identifier = _upf_consume_token().string;
    if (_upf_match_token(_UPF_TT_LESS_THAN)) {
        _UPF_ERROR("Templated variables/functions are not supported at %s:%d.", _upf_state.file_path, _upf_state.line);
    }

    _upf_cstr_vec ns_names = _UPF_ZERO_INIT;
    while (_upf_match_token(_UPF_TT_CXX_SCOPE)) {
        _UPF_VECTOR_PUSH(&ns_names, identifier);
        identifier = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
    }

    return (_upf_type *) _upf_scope_search(leading_scope_op, &ns_names, _upf_ns_get_identifier_cb, _upf_scope_get_identifier_cb,
                                           identifier);
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
    _UPF_ASSERT(type != NULL);

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

static _upf_type *_upf_cxx_noexcept(void) {
    _upf_consume_token();
    _upf_expect_token(_UPF_TT_OPEN_PAREN);
    _upf_consume_parens(_UPF_TT_OPEN_PAREN, _UPF_TT_CLOSE_PAREN);
    return _upf_get_bool_type();
}

static _upf_type *_upf_cxx_typeid(void) { _UPF_ERROR("typeid is not supported at %s:%d.", _upf_state.file_path, _upf_state.line); }

static _upf_type *_upf_cxx_lambda(void) { _UPF_ERROR("Lambdas are not supported at %s:%d.", _upf_state.file_path, _upf_state.line); }

static _upf_type *_upf_unary(void) {
    _upf_token unop = _upf_consume_token();
    _upf_type *type = _upf_parse(_UPF_PREC_UNARY);
    if (type == NULL) return NULL;

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
    if (dot.type == _UPF_TT_ARROW) type = _upf_dereference_type(type);

    const char *member_name = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
    if (_upf_match_token(_UPF_TT_CXX_SCOPE)) {
        _upf_cstr_vec ns_names = _UPF_ZERO_INIT;
        type = (_upf_type *) _upf_scope_search(false, &ns_names, _upf_ns_get_type_cb, _upf_scope_get_type_cb, member_name);
        member_name = _upf_expect_token(_UPF_TT_IDENTIFIER).string;
    }

    _UPF_ASSERT(type->kind == _UPF_TK_STRUCT || type->kind == _UPF_TK_UNION);

    int inheritance = INT_MAX;
    _upf_type *member_type = NULL;
    _upf_member_vec members = type->as.cstruct.members;
    for (uint32_t i = 0; i < members.length; i++) {
        if (strcmp(members.data[i].name, member_name) == 0 && members.data[i].inheritance < inheritance) {
            member_type = members.data[i].type;
            inheritance = members.data[i].inheritance;
        }
    }
    if (member_type != NULL) return member_type;

    _upf_method_vec methods = type->as.cstruct.methods;
    const _upf_function *method_type = NULL;
    for (uint32_t i = 0; i < methods.length; i++) {
        if (strcmp(methods.data[i].function.name, member_name) == 0 && methods.data[i].inheritance < inheritance) {
            method_type = &methods.data[i].function;
            inheritance = methods.data[i].inheritance;
        }
    }
    if (method_type != NULL) return _upf_get_function_type(method_type);

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

static void _upf_init_parsing_rules(void) {
#ifdef __cplusplus
#define _UPF_DEFINE_RULE(token, prefix, infix, precedence) _upf_parse_rules[token] = _upf_parse_rule{prefix, infix, precedence}
#else
#define _UPF_DEFINE_RULE(token, prefix, infix, precedence) _upf_parse_rules[token] = (_upf_parse_rule) {prefix, infix, precedence}
#endif
    // clang-format off
    _UPF_DEFINE_RULE(_UPF_TT_NUMBER,           _upf_number,       NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_STRING,           _upf_string,       NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_IDENTIFIER,       _upf_identifier,   NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_GENERIC,          _upf_generic,      NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_SIZEOF,           _upf_sizeof,       NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_ALIGNOF,          _upf_alignof,      NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_CAST,         _upf_cxx_cast,     NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_UNARY,            _upf_unary,        NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_NEW,          _upf_cxx_new,      NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_SCOPE,        _upf_cxx_scope,    NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_NOEXCEPT,     _upf_cxx_noexcept, NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_CXX_TYPEID,       _upf_cxx_typeid,   NULL,            _UPF_PREC_NONE      );
    _UPF_DEFINE_RULE(_UPF_TT_OPEN_PAREN,       _upf_paren,        _upf_call,       _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_INCREMENT,        _upf_unary,        _upf_postfix,    _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_OPEN_BRACKET,     _upf_cxx_lambda,   _upf_index,      _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_DOT,              NULL,              _upf_dot,        _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_ARROW,            NULL,              _upf_dot,        _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_DOT_STAR,         NULL,              _upf_dot_star,   _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_ARROW_STAR,       NULL,              _upf_dot_star,   _UPF_PREC_POSTFIX   );
    _UPF_DEFINE_RULE(_UPF_TT_FACTOR,           NULL,              _upf_binary,     _UPF_PREC_FACTOR    );
    _UPF_DEFINE_RULE(_UPF_TT_STAR,             _upf_unary,        _upf_binary,     _UPF_PREC_FACTOR    );
    _UPF_DEFINE_RULE(_UPF_TT_PLUS,             _upf_unary,        _upf_binary,     _UPF_PREC_TERM      );
    _UPF_DEFINE_RULE(_UPF_TT_MINUS,            _upf_unary,        _upf_binary,     _UPF_PREC_TERM      );
    _UPF_DEFINE_RULE(_UPF_TT_AMPERSAND,        _upf_unary,        _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_LESS_THAN,        _upf_unary,        _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_GREATER_THAN,     _upf_unary,        _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_DOUBLE_AMPERSAND, NULL,              _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_LOGICAL,          NULL,              _upf_binary,     _UPF_PREC_LOGICAL   );
    _UPF_DEFINE_RULE(_UPF_TT_QUESTION,         NULL,              _upf_ternary,    _UPF_PREC_TERNARY   );
    _UPF_DEFINE_RULE(_UPF_TT_ASSIGNMENT,       NULL,              _upf_assignment, _UPF_PREC_ASSIGNMENT);
    // clang-format on
#undef _UPF_DEFINE_RULE
}

// ================== /proc/pid/maps ======================

static _upf_range_vec _upf_get_readable_addresses(void) {
    FILE *file = fopen("/proc/self/maps", "r");
    if (file == NULL) _UPF_ERROR("Failed to open \"/proc/self/maps\": %s.", strerror(errno));

    _upf_range_vec ranges = _UPF_ZERO_INIT;
    _upf_range range = _UPF_ZERO_INIT;
    size_t length = 0;
    char *line = NULL;
    ssize_t read;
    while ((read = getline(&line, &length, file)) != -1) {
        if (read == 0) continue;

        char *cur = line;
        char *cur_end = NULL;
        errno = 0;
        uintmax_t start = strtoumax(cur, &cur_end, 16);
        if (*cur_end != '-' || errno != 0) _UPF_ERROR("Failed to parse \"/proc/self/maps\": invalid format.");
        cur = cur_end + 1;

        uintmax_t end = strtoumax(cur, &cur_end, 16);
        if (*cur_end != ' ' || errno != 0) _UPF_ERROR("Failed to parse \"/proc/self/maps\": invalid format.");
        cur = cur_end + 1;

        if (*cur != 'r') continue;

        if (start > UINTPTR_MAX || end > UINTPTR_MAX) _UPF_ERROR("Failed to parse \"/proc/self/maps\": address is too long.");
        range.start = start;
        range.end = end;
        _UPF_VECTOR_PUSH(&ranges, range);
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

static bool _upf_is_memory_readable(const uint8_t *ptr, size_t size) {
    _UPF_ASSERT(ptr != NULL);
    if (_upf_get_memory_region_end(ptr) == NULL) return false;
    if (size == 0 || size == SIZE_MAX) return true;
    return _upf_get_memory_region_end(ptr + size) != NULL;
}

// ===================== PRINTING =========================

#define _UPF_INITIAL_BUFFER_SIZE 512

#define _upf_bprintf(...)                                                             \
    while (true) {                                                                    \
        int bytes = snprintf(_upf_state.ptr, _upf_state.free, __VA_ARGS__);           \
        _UPF_ASSERT(bytes >= 0);                                                      \
        if ((uint32_t) bytes >= _upf_state.free) {                                    \
            uint32_t used = _upf_state.size - _upf_state.free;                        \
            _upf_state.size *= 2;                                                     \
            _upf_state.buffer = (char *) realloc(_upf_state.buffer, _upf_state.size); \
            if (_upf_state.buffer == NULL) _UPF_OUT_OF_MEMORY();                      \
            _upf_state.ptr = _upf_state.buffer + used;                                \
            _upf_state.free = _upf_state.size - used;                                 \
            continue;                                                                 \
        }                                                                             \
        _upf_state.free -= bytes;                                                     \
        _upf_state.ptr += bytes;                                                      \
        break;                                                                        \
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
    _UPF_ASSERT(false && "Unreachable");
}

__attribute__((no_sanitize_address)) static void _upf_find_repeating_structs(_upf_struct_info_map *structs, const uint8_t *data,
                                                                             const _upf_type *type, int depth) {
    _UPF_ASSERT(structs != NULL && type != NULL);

    if (UPRINTF_MAX_DEPTH >= 0 && depth >= UPRINTF_MAX_DEPTH) return;
    if (data == NULL || !_upf_is_memory_readable(data, type->size)) return;

    if (type->kind == _UPF_TK_POINTER || type->kind == _UPF_TK_REFERENCE) {
        const _upf_type *ptr_type = type->kind == _UPF_TK_POINTER ? type->as.pointer.type : type->as.reference.type;
        if (ptr_type == NULL) return;

        const uint8_t *ptr;
        memcpy(&ptr, data, sizeof(ptr));
        _upf_find_repeating_structs(structs, ptr, ptr_type, depth);
        return;
    }

    if (type->kind != _UPF_TK_STRUCT && type->kind != _UPF_TK_UNION) return;

    _upf_struct_key key = _UPF_ZERO_INIT;
    key.data = data;
    key.type = type;

    _upf_struct_info *opt_value = _UPF_MAP_GET(structs, key);
    if (opt_value != NULL) {
        opt_value->is_repeating = true;
        return;
    }

    _upf_struct_info new_struct_info = _UPF_ZERO_INIT;
    _UPF_MAP_SET(structs, key, new_struct_info);

    _upf_member_vec members = type->as.cstruct.members;
    for (uint32_t i = 0; i < members.length; i++) {
        const _upf_member *member = &members.data[i];
        if (member->bit_size > 0) continue;  // skip bit fields
        _upf_find_repeating_structs(structs, data + member->offset, member->type, depth + 1);
    }
}

static void _upf_print_modifiers(int modifiers, bool trailing_whitespace) {
    static struct {
        const char *name;
        int mask;
    } MODIFIERS_TABLE[] = {
        {"const", _UPF_MOD_CONST},
        {"volatile", _UPF_MOD_VOLATILE},
        {"restrict", _UPF_MOD_RESTRICT},
        {"atomic", _UPF_MOD_ATOMIC},
    };

    bool print_space = false;
    for (size_t i = 0; i < sizeof(MODIFIERS_TABLE) / sizeof(*MODIFIERS_TABLE); i++) {
        if (modifiers & MODIFIERS_TABLE[i].mask) {
            if (print_space) _upf_bprintf(" ");
            print_space = true;
            _upf_bprintf("%s", MODIFIERS_TABLE[i].name);
        }
    }
    if (print_space && trailing_whitespace) _upf_bprintf(" ");
}

static void _upf_print_type_name(const _upf_type *type, bool trailing_whitespace, bool is_return_type) {
    _UPF_ASSERT(type != NULL);
    switch (type->kind) {
        case _UPF_TK_POINTER:
            if (type->as.pointer.type != NULL && type->as.pointer.type->kind == _UPF_TK_FUNCTION) {
                _upf_print_type_name(type->as.pointer.type, trailing_whitespace, is_return_type);
                break;
            }

            if (type->as.pointer.type == NULL) {
                _upf_bprintf("void ");
            } else {
                _upf_print_type_name(type->as.pointer.type, true, is_return_type);
            }
            _upf_bprintf("*");
            _upf_print_modifiers(type->modifiers, trailing_whitespace);
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
            if (trailing_whitespace) _upf_bprintf(" ");
            break;
        default:
            _upf_print_modifiers(type->modifiers, true);
            _upf_bprintf("%s", type->name == NULL ? "<unnamed>" : type->name);
            if (trailing_whitespace) _upf_bprintf(" ");
            break;
    }
}

__attribute__((no_sanitize_address)) static void _upf_print_bit_field(const uint8_t *data, int total_bit_offset, int bit_size) {
    _UPF_ASSERT(data != NULL);

    int byte_offset = total_bit_offset / 8;
    int bit_offset = total_bit_offset % 8;
    uint8_t value = (data[byte_offset] >> bit_offset) & ((1 << bit_size) - 1);

    _upf_bprintf("%" PRIu8 " <%d bit%s>", value, bit_size, bit_size > 1 ? "s" : "");
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

// Brackets/parentheses printing guidelines:
// () -> additional information, e.g. pointer's value, char's ascii, enum's value, etc.
// [] -> arrays
// {} -> structs/unions
// <> -> meta information, e.g. unnamed, unknown, invalid, out of bounds, truncated, etc.
__attribute__((no_sanitize_address)) static void _upf_print_type(_upf_struct_info_map *structs, const uint8_t *data, const _upf_type *type,
                                                                 int depth) {
    _UPF_ASSERT(structs != NULL && type != NULL);

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

    if (!_upf_is_memory_readable(data, type->size)) {
        _upf_bprintf("<out-of-bounds>");
        return;
    }

    switch (type->kind) {
        case _UPF_TK_UNION:  _upf_bprintf("<union> "); __attribute__((fallthrough));  // Handle union as struct
        case _UPF_TK_STRUCT: {
            for (uint32_t i = 0; i < _upf_state.ignored_structs.length; i++) {
                if (strcmp(type->name, _upf_state.ignored_structs.data[i]) == 0) {
                    _upf_bprintf("<ignored>");
                    return;
                }
            }

            _upf_member_vec members = type->as.cstruct.members;
            if (members.length == 0) {
                _upf_bprintf("{}");
                if (type->as.cstruct.is_declaration) _upf_bprintf(" <opaque>");
                return;
            }

            _upf_struct_key key = _UPF_ZERO_INIT;
            key.data = data;
            key.type = type;

            _upf_struct_info *opt_info = _UPF_MAP_GET(structs, key);
            if (opt_info != NULL && opt_info->is_repeating) {
                if (opt_info->is_visited) {
                    _upf_bprintf("<points to #%" PRIu32 ">", opt_info->id);
                    return;
                }

                opt_info->is_visited = true;
                opt_info->id = _upf_state.struct_id++;
                _upf_bprintf("<#%" PRIu32 "> ", opt_info->id);
            }

            _upf_bprintf("{\n");
            for (uint32_t i = 0; i < members.length; i++) {
                const _upf_member *member = &members.data[i];

                _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");
                _upf_print_type_name(member->type, true, false);
                _upf_bprintf("%s = ", member->name);
                if (member->bit_size == 0) {
                    _upf_print_type(structs, data + member->offset, member->type, depth + 1);
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

            uint64_t enum_value = 0;
            memcpy(&enum_value, data, underlying_type->size);

            // Use memcmp instead of comparison to avoid handling signed values,
            // which when casted up require sign-extension.
            const char *name = NULL;
            for (uint32_t i = 0; i < enums.length; i++) {
                if (memcmp(&enums.data[i].value, &enum_value, underlying_type->size) == 0) {
                    name = enums.data[i].name;
                    break;
                }
            }

            _upf_bprintf("%s (", name ? name : "<unknown>");
            _upf_print_type(structs, data, underlying_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_ARRAY: {
            const _upf_type *element_type = type->as.array.element_type;
            size_t element_size = element_type->size;
            if (element_size == SIZE_MAX) {
                _upf_bprintf("<unknown>");
                return;
            }

            if (type->as.array.lengths.length == 0) {
                _upf_bprintf("<non-static array>");
                return;
            }
            uint32_t length = type->as.array.lengths.data[0];

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
            for (size_t i = 0; i < length; i++) {
                if (i > 0) _upf_bprintf(is_primitive ? ", " : ",\n");
                if (!is_primitive) _upf_bprintf("%*s", UPRINTF_INDENTATION_WIDTH * (depth + 1), "");

                const uint8_t *current = data + element_size * i;
                _upf_print_type(structs, current, element_type, depth + 1);

#if UPRINTF_ARRAY_COMPRESSION_THRESHOLD > 0
                size_t j = i;
                // Advance while element at `j` is same as at `i`.
                while (j < length) {
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
#endif  // UPRINTF_ARRAY_COMPRESSION_THRESHOLD
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
                _upf_print_type(structs, (const uint8_t *) ptr, pointed_type, depth);
                return;
            }

            if (pointed_type->kind == _UPF_TK_SCHAR || pointed_type->kind == _UPF_TK_UCHAR) {
                _upf_print_char_ptr((const char *) ptr);
                return;
            }

            _upf_bprintf("%p (", ptr);
            _upf_print_type(structs, (const uint8_t *) ptr, pointed_type, depth);
            _upf_bprintf(")");
        } break;
        case _UPF_TK_REFERENCE: {
            const _upf_type *ref_type = type->as.reference.type;
            _UPF_ASSERT(ref_type != NULL);

            const uint8_t *ptr;
            memcpy(&ptr, data, sizeof(ptr));

            // If it is a valid pointer, treat reference as a pointer,
            // otherwise assume that it is the data.
            if (ptr == NULL || !_upf_is_memory_readable(ptr, ref_type->size)) {
                _upf_print_type(structs, data, ref_type, depth);
            } else {
                _upf_print_type(structs, ptr, ref_type, depth);
            }
        } break;
        case _UPF_TK_MEMBER_POINTER: _upf_bprintf("<member pointer>"); break;
        case _UPF_TK_FUNCTION:       {
            const _upf_function *function = type->as.function.function_ptr;
            if (function == NULL) {
                uint64_t relative_function_pc = data - _upf_state.base;
                function = _upf_get_function_by_pc(relative_function_pc);
            }
            if (function == NULL) {
                uint64_t absolute_function_pc = (uint64_t) data;
                function = _upf_get_extern_function(absolute_function_pc);
            }

            _upf_bprintf("%p", (void *) data);
            if (function == NULL) return;

            _upf_bprintf(" (");
            _upf_type *return_type = function->return_type_die == NULL ? _upf_get_void_type()
                                                                       : _upf_parse_type(_upf_state.current_cu, function->return_type_die);
            _upf_print_type_name(return_type, true, true);
            _upf_bprintf("%s(", function->name);
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
        } break;
        case _UPF_TK_U1: _upf_bprintf("%" PRIu8, *data); break;
        case _UPF_TK_U2: {
            uint16_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIu16, temp);
        } break;
        case _UPF_TK_U4: {
            uint32_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIu32, temp);
        } break;
        case _UPF_TK_U8: {
            uint64_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIu64, temp);
        } break;
        case _UPF_TK_S1: _upf_bprintf("%" PRIi8, *((const int8_t *) data)); break;
        case _UPF_TK_S2: {
            int16_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIi16, temp);
        } break;
        case _UPF_TK_S4: {
            int32_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIi32, temp);
        } break;
        case _UPF_TK_S8: {
            int64_t temp;
            memcpy(&temp, data, sizeof(temp));
            _upf_bprintf("%" PRIi64, temp);
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
            _upf_bprintf("%" PRIi8, (int8_t) ch);
            if (_upf_is_printable(ch)) _upf_bprintf(" ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_UCHAR: {
            char ch = *((char *) data);
            _upf_bprintf("%" PRIu8, (uint8_t) ch);
            if (_upf_is_printable(ch)) _upf_bprintf(" ('%s')", _upf_escape_char(ch));
        } break;
        case _UPF_TK_VOID:    _UPF_WARN("Void must be a pointer. Ignoring this type."); break;
        case _UPF_TK_UNKNOWN: _upf_bprintf("<unknown>"); break;
    }
}

// ======================== PC ============================

static int _upf_phdr_callback(struct dl_phdr_info *raw_info, __attribute__((unused)) size_t size, void *raw_data) {
    _upf_dl_phdr_info *info = (_upf_dl_phdr_info *) raw_info;
    void **data = (void **) raw_data;

    // Empty name indicates the current executable.
    if (info->dlpi_name[0] == '\0') {
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

static bool _upf_is_in_range(uint64_t addr, _upf_range_vec ranges) {
    for (uint32_t i = 0; i < ranges.length; i++) {
        if (ranges.data[i].start <= addr && addr < ranges.data[i].end) return true;
    }
    return false;
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

// ======================= MAIN ===========================

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

    if (type == NULL) _UPF_ERROR("Failed to find the type of \"%s\" at %s:%d.", arg, _upf_state.file_path, _upf_state.line);
    if (type->kind == _UPF_TK_VOID) {
        _UPF_ERROR("Cannot print type void. To print the void pointer itself, get a pointer to \"%s\" at %s:%d.", arg, _upf_state.file_path,
                   _upf_state.line);
    }
    return type;
}

static void _upf_add_ignored_types(void) {
    char *str = _upf_copy_string(UPRINTF_IGNORE_STRUCTS);
    const char *start = str;
    for (char *ch = str; *ch != '\0'; ch++) {
        if (*ch != ',') continue;

        _UPF_VECTOR_PUSH(&_upf_state.ignored_structs, start);
        *ch = '\0';
        start = ch + 1;
    }
    _UPF_VECTOR_PUSH(&_upf_state.ignored_structs, start);
}

__attribute__((constructor)) static void _upf_init(void) {
    if (setjmp(_upf_state.error_jmp_buf) != 0) return;

    _upf_state.base = (const uint8_t *) _upf_get_this_executable_address();
    _upf_init_parsing_rules();
    _upf_add_ignored_types();

    _upf_parse_elf();
    _upf_parse_extern_functions();
    _upf_parse_dwarf();

    _upf_state.size = _UPF_INITIAL_BUFFER_SIZE;
    _upf_state.buffer = (char *) malloc(_upf_state.size * sizeof(*_upf_state.buffer));
    if (_upf_state.buffer == NULL) _UPF_OUT_OF_MEMORY();
    _upf_state.is_init = true;
}

__attribute__((destructor)) static void _upf_fini(void) {
    if (_upf_state.elf_file != NULL) munmap(_upf_state.elf_file, _upf_state.elf_file_size);
    free(_upf_state.buffer);
    _upf_free_allocator();
}

#ifdef __cplusplus
}  // namespace uprintf

extern "C" {
#endif

__attribute__((noinline)) void _upf_uprintf(const char *file_path, int line, const char *fmt, const char *arg_str, ...) {
#ifdef __cplusplus
    using namespace uprintf;
#endif

    _UPF_ASSERT(file_path != NULL && line > 0 && fmt != NULL && arg_str != NULL);

    if (!_upf_state.is_init) return;
    if (setjmp(_upf_state.error_jmp_buf) != 0) return;

    _upf_state.ptr = _upf_state.buffer;
    _upf_state.free = _upf_state.size;
    _upf_state.addresses = _upf_get_readable_addresses();
    _upf_state.struct_id = 0;
    _upf_state.file_path = file_path;
    _upf_state.line = line;

    uint8_t *pc_ptr = (uint8_t *) __builtin_extract_return_addr(__builtin_return_address(0));
    _UPF_ASSERT(pc_ptr != NULL);
    uint64_t pc = pc_ptr - _upf_state.base;

    _upf_state.current_cu = _upf_find_cu(pc);
    _UPF_ASSERT(_upf_state.current_cu != NULL);

    _upf_state.current_scopes.length = 0;
    _upf_find_scopes(pc, &_upf_state.current_cu->scope, &_upf_state.current_scopes);

    _upf_cstr_vec args = _upf_get_args(_upf_copy_string(arg_str));
    size_t arg_idx = 0;

    _upf_struct_info_map structs = _UPF_ZERO_INIT;

    va_list va_args;
    va_start(va_args, arg_str);
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
            _UPF_MAP_RESET(&structs);
            _upf_find_repeating_structs(&structs, ptr, type, 0);
            _upf_print_type(&structs, ptr, type, 0);
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

#undef _UPF_SET_TEST_STATUS
#undef _UPF_LOG
#undef _UPF_WARN
#undef _UPF_ERROR
#undef _UPF_ASSERT
#undef _UPF_OUT_OF_MEMORY
#undef _UPF_NO_DEBUG_INFO_ERROR
#undef _UPF_ZERO_INIT
#undef _UPF_VECTOR_TYPEDEF
#undef _UPF_MAP_TYPEDEF
#undef _UPF_MOD_CONST
#undef _UPF_MOD_VOLATILE
#undef _UPF_MOD_RESTRICT
#undef _UPF_MOD_ATOMIC
#undef _UPF_INITIAL_REGION_SIZE
#undef _UPF_INITIAL_VECTOR_CAPACITY
#undef _UPF_VECTOR_PUSH
#undef _UPF_VECTOR_TOP
#undef _UPF_INITIAL_MAP_CAPACITY
#undef _UPF_MAP_LOAD_FACTOR
#undef _UPF_OFFSET_OF
#undef _UPF_MAP_IMPL_GET
#undef _UPF_MAP_IMPL_SET
#undef _UPF_MAP_GET
#undef _UPF_MAP_STR_GET
#undef _UPF_MAP_SET
#undef _UPF_MAP_STR_SET
#undef _UPF_MAP_RESET
#undef _UPF_INITIAL_BUFFER_SIZE
#undef _upf_bprintf

#endif  // UPRINTF_IMPLEMENTATION
