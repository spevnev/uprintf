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

void _upf_uprintf(const char *file, int line, int id, const char *fmt, ...);

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
    } while (0);


struct _upf_dwarf {
    uint8_t *file;
    off_t file_size;

    uint8_t offset_size;
    uint8_t address_size;

    const Elf64_Shdr *info;
    const Elf64_Shdr *abbrev;
};

struct _upf_attr {
    uint64_t name;
    uint64_t form;
    int64_t implicit_const;
};

VECTOR_TYPEDEF(_upf_attrs, struct _upf_attr);

struct _upf_abbrev {
    uint64_t code;
    uint64_t tag;
    uint8_t has_child;
    _upf_attrs attrs;
};

VECTOR_TYPEDEF(_upf_abbrevs, struct _upf_abbrev);

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

static _upf_abbrevs _upf_parse_abbrevs(const uint8_t *abbrev_table) {
    _upf_abbrevs abbrevs = {0};

    while (1) {
        struct _upf_abbrev abbrev = {0};
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.code);
        if (abbrev.code == 0) break;
        abbrev_table += _upf_uLEB_to_uint64(abbrev_table, &abbrev.tag);

        abbrev.has_child = *abbrev_table;
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


static void _upf_parse_cu(const uint8_t *info, const uint8_t *info_end, const uint8_t *abbrev_table) {
    _upf_abbrevs abbrevs = _upf_parse_abbrevs(abbrev_table);

    uint64_t code;
    while (info < info_end) {
        info += _upf_uLEB_to_uint64(info, &code);
        if (code == 0) continue;
        assert(code <= abbrevs.length);

        const struct _upf_abbrev *abbrev = &abbrevs.data[code - 1];
        for (size_t i = 0; i < abbrev->attrs.length; i++) {
            uint64_t form = abbrev->attrs.data[i].form;
        something:
            switch (form) {
                case DW_FORM_addr:
                    info += _upf_dwarf.address_size;
                    break;
                case DW_FORM_strx1:
                case DW_FORM_addrx1:
                case DW_FORM_flag:
                case DW_FORM_ref1:
                case DW_FORM_data1:
                case DW_FORM_block1:
                    info += 1;
                    break;
                case DW_FORM_strx2:
                case DW_FORM_addrx2:
                case DW_FORM_ref2:
                case DW_FORM_data2:
                    info += 2;
                    break;
                case DW_FORM_strx3:
                case DW_FORM_addrx3:
                    info += 3;
                    break;
                case DW_FORM_ref_sup4:
                case DW_FORM_strx4:
                case DW_FORM_addrx4:
                case DW_FORM_ref4:
                case DW_FORM_data4:
                case DW_FORM_block4:
                    info += 4;
                    break;
                case DW_FORM_ref_sig8:
                case DW_FORM_ref_sup8:
                case DW_FORM_ref8:
                case DW_FORM_data8:
                    info += 8;
                    break;
                case DW_FORM_data16:
                    info += 16;
                    break;
                case DW_FORM_sdata: {
                    int64_t result;
                    info += _upf_LEB_to_int64(info, &result);
                } break;
                case DW_FORM_loclistx:
                case DW_FORM_rnglistx:
                case DW_FORM_addrx:
                case DW_FORM_strx:
                case DW_FORM_ref_udata:
                case DW_FORM_udata: {
                    uint64_t result;
                    info += _upf_uLEB_to_uint64(info, &result);
                } break;
                case DW_FORM_string:
                    info += strlen((const char *) info) + 1;
                    break;
                case DW_FORM_exprloc:
                case DW_FORM_block: {
                    uint64_t length;
                    info += _upf_uLEB_to_uint64(info, &length);
                    info += length;
                } break;
                case DW_FORM_line_strp:
                case DW_FORM_strp_sup:
                case DW_FORM_sec_offset:
                case DW_FORM_ref_addr:
                case DW_FORM_strp:
                    info += _upf_dwarf.offset_size;
                    break;
                case DW_FORM_indirect:
                    info += _upf_uLEB_to_uint64(info, &form);
                    goto something;
                    break;
                case DW_FORM_flag_present:
                case DW_FORM_implicit_const:
                    break;
                default:
                    assert(0 && "Unreachable");
            }
        }
    }

    for (size_t i = 0; i < abbrevs.length; i++) VECTOR_FREE(&abbrevs.data[i].attrs);
    VECTOR_FREE(&abbrevs);
}

static void _upf_parse_dwarf(struct _upf_dwarf *dwarf) {
    const uint8_t *abbrev = dwarf->file + dwarf->abbrev->sh_offset;
    const uint8_t *info = dwarf->file + dwarf->info->sh_offset;
    const uint8_t *info_end = info + dwarf->info->sh_size;

    while (info < info_end) {
        uint64_t length = *((uint32_t *) info);
        info += sizeof(uint32_t);

        char is_64bit = 0;
        if (length == 0xffffffffU) {
            length = *((uint64_t *) info);
            info += sizeof(uint64_t);
            is_64bit = 1;
        }
        dwarf->offset_size = is_64bit ? 8 : 4;

        const uint8_t *next = info + length;

        uint16_t version = *((uint16_t *) info);
        info += sizeof(version);
        assert(version == 5);

        uint8_t type = *info;
        info += sizeof(type);
        assert(type == DW_UT_compile);

        dwarf->address_size = *info;
        info += sizeof(dwarf->address_size);

        uint64_t abbrev_offset;
        if (is_64bit) {
            abbrev_offset = *((uint64_t *) info);
            info += sizeof(uint64_t);
        } else {
            abbrev_offset = *((uint32_t *) info);
            info += sizeof(uint32_t);
        }

        _upf_parse_cu(info, next, abbrev + abbrev_offset);

        info = next;
    }
}

static struct _upf_dwarf _upf_parse_elf(void) {
    static const char *this_file_path = "/proc/self/exe";


    struct stat file_info;
    if (stat(this_file_path, &file_info) == -1) abort();
    size_t size = file_info.st_size;

    int fd = open(this_file_path, O_RDONLY);
    assert(fd != -1);

    uint8_t *file = (uint8_t *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    assert(file != MAP_FAILED);

    close(fd);


    const Elf64_Ehdr *header = (const Elf64_Ehdr *) file;

    assert(memcmp(header->e_ident, ELFMAG, SELFMAG) == 0);
    assert(header->e_ident[EI_CLASS] == ELFCLASS64);
    assert(header->e_ident[EI_VERSION] == 1);
    assert(header->e_machine == EM_X86_64);
    assert(header->e_version == 1);
    assert(header->e_shentsize == sizeof(Elf64_Shdr));

    const Elf64_Shdr *string_section = (const Elf64_Shdr *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);

    struct _upf_dwarf dwarf = {0};
    dwarf.file = file;
    dwarf.file_size = size;

    const Elf64_Shdr *section = (const Elf64_Shdr *) (file + header->e_shoff);
    for (size_t i = 0; i < header->e_shnum; i++) {
        const char *name = string_table + section->sh_name;

        if (strcmp(name, ".debug_info") == 0) dwarf.info = section;
        else if (strcmp(name, ".debug_abbrev") == 0) dwarf.abbrev = section;

        section++;
    }

    assert(dwarf.info != NULL && dwarf.abbrev != NULL);
    return dwarf;
}


__attribute__((constructor)) void _upf_init(void) {
    _upf_dwarf = _upf_parse_elf();
    _upf_parse_dwarf(&_upf_dwarf);
}

__attribute__((destructor)) void _upf_fini(void) { munmap(_upf_dwarf.file, _upf_dwarf.file_size); }

void _upf_uprintf(const char *file, int line, int id, const char *fmt, ...) {
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