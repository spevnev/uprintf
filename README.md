# Universal printf

uprintf is a single-header library for printing structures in C, created for debugging and prototyping purposes.

### What is a single header library?

The idea behind single-header libraries is that they're easy to distribute since it's trivial to add them to any project.

By default the header file acts as any header would, i.e. contains **declarations** (without definitions). \
However, by defining a macro the header start to act as a `.c` file, i.e. contains **definitions**.

## Examples

Source files as well as their outputs can be found in [examples](examples). \
You can also build them yourself using `make examples`, but note that `sqlite` takes long to build.

In addition, there are also [tests](tests) with their outputs in the [baselines](tests/baselines).

## Installation

Installing a single header library is as simple as downloading `uprintf.h`.

If you have cloned the repository, it can be installed to `/usr/local/include` by running `make install`. \
This way you should be able to include it without copying the header to every project (since it is a default include path).

## Requirements

- Linux
- Minimum C version is `c99`
- Debug information included, `-g2` or higher
- Have `elf.h` in include path

### Tested on:

Architectures: \
`x86_64`, `amd64`

Compilers: \
`gcc-14.2`, `gcc-13.3`, `gcc-12.4` \
`clang-18`, `clang-17`, `clang-14`


## Usage

0. Copy or install the library, ensure it is in the include path

1. Pick a single file and define `UPRINTF_IMPLEMENTATION` before the include:
    ```c
    #define UPRINTF_IMPLEMENTATION
    #include "uprintf.h"
    ```
    This will copy library's implementation to be built as a part of this file, so you should choose rarely modified file in order to avoid unnecessary rebuilding. \
    You can also define options here, see below.

2. Add include in other files where you intend to use it:
    ```c
    #include "uprintf.h"
    ```

3. Call function.
    ```c
    uprintf(fmt, ...);
    ```
    *fmt* - format string with `%S` format specifier (**printf specifiers are NOT supported**). Use `%%` to print `%`. \
    For each format specifier there must be a pointer to whatever should be printed in its place (except `void*`).

### Options

Behavior of the library can be changed by setting options before **implementation**:

```c
#define OPTION_NAME VALUE
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"
```

The list of options:

macro |  description | default
-|-|-
`UPRINTF_INDENTATION_WIDTH` | The number of spaces to use for indentation | 4
`UPRINTF_MAX_DEPTH` | How deep can nested structures be | 10
`UPRINTF_IGNORE_STDIO_FILE` | Should `stdio.h`'s `FILE` be ignored | true
`UPRINTF_ARRAY_COMPRESSION_THRESHOLD` | The minimum number of consecutive array values that get compressed(`VALUE <repeats X times>`). Use non-positive value to disable it. | 4

## How does it work?

TL;DR: It works by inspecting debugging information of the executable in a debugger-like manner, which allows it to interpret and format passed pointers.

0. Read current binary.
1. Parse ELF and DWARF.
2. Get info about scopes and types.
3. Locate scopes from which current call is invoked based on the PC.
4. Parse provided arguments.
5. Infer types from the parsed arguments.
7. Print data using type definition as reference.

## Tests

Tests take long to complete, so it is highly recommended to set `-j NUMBER_OF_CORES`.

If you use compiler other than `gcc` and `clang` (e.g. `clang-18`) make sure to specify `COMPILERS` or edit [Makefile](Makefile).

```console
$ make tests
$ make tests -jNUMBER_OF_CORES
$ make tests COMPILERS=COMPILER_NAME
```
