# Ultimate printf

uprintf is a single-header [stb-style](https://github.com/nothings/stb?tab=readme-ov-file#faq) library for printing any array or structure in C. \
It only works on Linux when the program is built with debug information.

### What is a single header library?

The idea behind single-header libraries is that they're easy to distribute since it's trivial to add them to any project.

By default the header file acts as any header would, i.e. contain declarations without definitions. \
However, by defining a macro the header start to act as a .c file, i.e. contains definitions.

## Usage


Adding uprintf to your project:

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
    *fmt* - format string with `%S` format specifier (**NOTE: printf specifiers are NOT supported**). \
    For each format specifier there must be a pointer to whatever should be printed in its place.

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

## Tests


Requirements: sed, awk, wdiff

```console
$ make all_tests
```

Tests take long to complete, so it is highly recommended to set `-j NUMBER_OF_CORES` to parallelize.
