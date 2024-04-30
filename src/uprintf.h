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


// Usage:
// `#define UPRINTF_IMPLEMENTATION` before the include to include implementations.



#ifndef __linux__
#error ERROR: uprintf only works on Linux
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define byte uint8_t

// See https://dwarfstd.org/doc/DWARF5.pdf for more details:
// Section 7.6 contains description
// Appendix C contains pseudo code implemented below
// Converts `leb` to `result` and returns the number of bytes
static size_t LEB128_to_uint64(uint64_t *result, const byte *leb) {
    size_t i = 0;
    int shift = 0;

    while (1) {
        byte b = leb[i++];
        *result |= (((uint64_t) (b & 0b01111111)) << shift);
        if ((b & 0b10000000) == 0) break;
        shift += 7;
    }

    return i;
}

// Visualization of Compilation Unit and Abbreviation Table:
// https://dwarfstd.org/doc/DWARF5.pdf#subsection.D.1.1
static void _upf_parse_cu(const byte *cu, const byte *abbrev) {
    uint64_t needed_code;
    LEB128_to_uint64(&needed_code, cu);  // TODO: offset both in arg and return value

    uint64_t code, tag;
    size_t abbrev_offset = 0;
    while (1) {
        abbrev_offset += LEB128_to_uint64(&code, abbrev + abbrev_offset);

        if (code == 0) return;

        abbrev_offset += LEB128_to_uint64(&tag, abbrev + abbrev_offset);

        printf("%d: %X\n", code, tag);

        // byte is_next_child = *(abbrev + abbrev_offset);
        abbrev_offset += sizeof(byte);

        while (1) {
            uint64_t attr_name, attr_form;
            abbrev_offset += LEB128_to_uint64(&attr_name, abbrev + abbrev_offset);
            abbrev_offset += LEB128_to_uint64(&attr_form, abbrev + abbrev_offset);
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

static void _upf_parse_dwarf() {
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

    assert(header->e_machine == EM_X86_64);
    assert(header->e_version == 1);
    assert(header->e_shentsize == sizeof(Elf64_Shdr));

    const Elf64_Shdr *string_section = (const Elf64_Shdr *) (file + header->e_shoff + header->e_shstrndx * header->e_shentsize);
    const char *string_table = (char *) (file + string_section->sh_offset);


    const Elf64_Shdr *info = NULL;
    const Elf64_Shdr *abbrev = NULL;

    const Elf64_Shdr *section = (const Elf64_Shdr *) (file + header->e_shoff);
    for (size_t i = 0; i < header->e_shnum; i++) {
        const char *name = string_table + section->sh_name;

        if (strcmp(name, ".debug_info") == 0) info = section;
        else if (strcmp(name, ".debug_abbrev") == 0) abbrev = section;

        section++;
    }

    assert(info != NULL && abbrev != NULL);
    _upf_parse_info(file + info->sh_offset, info->sh_size, file + abbrev->sh_offset);

    munmap(file, size);
}

void uprintf(const char *fmt, ...) { _upf_parse_dwarf(); }

#undef byte

#endif  // UPRINTF_IMPLEMENTATION