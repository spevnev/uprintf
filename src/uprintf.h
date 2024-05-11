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


#ifndef UPRINTF_H
#define UPRINTF_H

void uprintf(const char *fmt, ...);

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

#define byte uint8_t


struct _upf_dwarf {
    byte *file;
    off_t file_size;

    const Elf64_Shdr *info;
    const Elf64_Shdr *abbrev;
};

static struct _upf_dwarf _upf_dwarf = {0};


// Converts LEB128 to uint64_t and returns number of bytes.
// See https://dwarfstd.org/doc/DWARF5.pdf for more details:
// Section 7.6 contains description
// Appendix C contains pseudo code of this implementation
static size_t _upf_uLEB_to_uint64(uint64_t *result, const byte *leb) {
    static const byte BITS_MASK = 0x7f;      // 01111111
    static const byte CONTINUE_MASK = 0x80;  // 10000000

    size_t i = 0;
    int shift = 0;

    *result = 0;
    while (1) {
        byte b = leb[i++];
        *result |= (((uint64_t) (b & BITS_MASK)) << shift);
        if ((b & CONTINUE_MASK) == 0) break;
        shift += 7;
    }

    return i;
}


// Visualization of Compilation Unit and Abbreviation Table:
// https://dwarfstd.org/doc/DWARF5.pdf#subsection.D.1.1
static void _upf_parse_cu(const byte *cu, const byte *abbrev) {
    uint64_t needed_code;
    _upf_uLEB_to_uint64(&needed_code, cu);  // TODO: offset both in arg and return value

    uint64_t code, tag;
    size_t abbrev_offset = 0;
    while (1) {
        abbrev_offset += _upf_uLEB_to_uint64(&code, abbrev + abbrev_offset);

        if (code == 0) return;

        abbrev_offset += _upf_uLEB_to_uint64(&tag, abbrev + abbrev_offset);

        printf("%d: %X\n", code, tag);

        // byte is_next_child = *(abbrev + abbrev_offset);
        abbrev_offset += sizeof(byte);

        while (1) {
            uint64_t attr_name, attr_form;
            abbrev_offset += _upf_uLEB_to_uint64(&attr_name, abbrev + abbrev_offset);
            abbrev_offset += _upf_uLEB_to_uint64(&attr_form, abbrev + abbrev_offset);
            if (attr_name == 0 && attr_form == 0) break;
        }

        if (code == needed_code) break;
    }

    // TODO: parse current DIE given abbreviation table's info
}

static void _upf_parse_info(const byte *info, size_t length, const byte *abbrev) {
    size_t offset = 0;

    while (offset < length) {
        uint64_t length = *((uint32_t *) (info + offset));
        offset += sizeof(uint32_t);

        char is_64bit = 0;
        if (length == 0xffffffffU) {
            length = *((uint64_t *) (info + offset));
            offset += sizeof(uint64_t);
            is_64bit = 1;
        }
        uint64_t next = offset + length;

        uint16_t version = *((uint16_t *) (info + offset));
        offset += sizeof(version);
        assert(version == 5);

        uint8_t type = *(info + offset);
        offset += sizeof(type);
        assert(type == DW_UT_compile);

        uint8_t address_size = *(info + offset);
        offset += sizeof(address_size);
        assert(address_size == sizeof(void *));

        uint64_t abbrev_offset;
        if (is_64bit) {
            abbrev_offset = *((uint64_t *) (info + offset));
            offset += sizeof(uint64_t);
        } else {
            abbrev_offset = *((uint32_t *) (info + offset));
            offset += sizeof(uint32_t);
        }

        _upf_parse_cu(info + offset, abbrev + abbrev_offset);

        offset = next;
    }
}

static struct _upf_dwarf _upf_parse_elf(void) {
    static const char *self_file = "/proc/self/exe";


    struct stat file_info;
    if (stat(self_file, &file_info) == -1) abort();
    size_t size = file_info.st_size;

    int fd = open(self_file, O_RDONLY);
    assert(fd != -1);

    byte *file = (byte *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
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

    _upf_parse_info(_upf_dwarf.file + _upf_dwarf.info->sh_offset, _upf_dwarf.info->sh_size, _upf_dwarf.file + _upf_dwarf.abbrev->sh_offset);
}

__attribute__((destructor)) void _upf_fini(void) { munmap(_upf_dwarf.file, _upf_dwarf.file_size); }

void _upf_uprintf(const char *file, int line, const char *fmt, ...) {
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
            fprintf(stderr, "[ERROR] uprintf: unfinished format at the end of line at %s:%d\n", file, line);
            exit(1);
        } else {
            fprintf(stderr, "[ERROR] uprintf: unkown format '%%%c' at %s:%d\n", next, file, line);
            exit(1);
        }
    }

    va_end(args);
}

#define uprintf(...) _upf_uprintf(__FILE__, __LINE__, __VA_ARGS__)

#undef byte

#endif  // UPRINTF_IMPLEMENTATION