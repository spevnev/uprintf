# Universal printf

uprintf is a single-header library for printing structures in C. \
It only works on Linux when the executable is built with debug information.

### What is a single header library?

The idea behind single-header libraries is that they're easy to distribute since it's trivial to add them to any project.

By default the header file acts as any header would, i.e. contains declarations without definitions. \
However, by defining a macro the header start to act as a .c file, i.e. contains definitions.

## Installation

Installing a single header library is as simple as downloading `src/uprintf.h`.

If you have cloned the repository, it can be installed to `/usr/local/include` by running `make install`. \
This way you should be able to include it without copying the header to every project (since it is a default include path).

## Usage

You can take a look at `examples/`, and build them using `make examples`

Adding uprintf to your project:

0. Requirements: \
    Minimum C version is 99 \
    Compile with debug information, either `-g2` or `-g3` \
    Have `elf.h` and `dwarf.h` in include path

1. Pick a single file and define `UPRINTF_IMPLEMENTATION` before the include:
    ```c
    #define UPRINTF_IMPLEMENTATION
    #include "uprintf.h"
    ```
    This will copy library's implementation to be built as a part of this file, so you should choose rarely modified file in order to avoid unnecessary rebuilding.

2. Add include in other files where you intend to use it:
    ```c
    #include "uprintf.h"
    ```

3. Call function.
    ```c
    uprintf(fmt, ...);
    ```
    *fmt* - format string with `%S` format specifier (**NOTE: printf specifiers are NOT supported**). Use `%%` to print `%`. \
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

## How does it work?

TL;DR It works by inspecting debugging information of the executable in a way similar to `print` in `gdb`.

Longer explanation:
1. Parse ELF and DWARF.
2. Get all scopes and types.
3. Associate current call with one of scopes based on its PC.
4. Parse provided arguments.
5. If the argument is casted, get typename, otherwise get variable name.
6. If the argument is casted, find type by its name, otherwise find variable in the scope and get its type.
7. Print data using type definition to know location and format of the fields.

## Tests

Requirements: sed, awk, wdiff

Tests take long to complete, so it is highly recommended to set `-j NUMBER_OF_CORES`.

```console
$ make tests
```

Tests work by:
1. Checking errors during compilation.
2. Checking errors and warnings during execution.
3. Comparing output to baseline, which is manually created/updated.
