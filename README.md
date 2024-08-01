# Ultimate printf

uprintf is a single-header [stb-style](https://github.com/nothings/stb?tab=readme-ov-file#faq) library for printing any array or structure in C. \
It only works on Linux when the program is built with debug information.

### What is a single header library?

The idea behind single-header libraries is that they're easy to distribute since it's trivial to add them to any project.

By default the header file acts as any header would, i.e. contain declarations without definitions. \
However, by defining a macro the header start to act as a .c file, i.e. contains definitions.

## Usage

In order to use it, you need to pick one file (preferably rarely modified to avoid rebuilding the library) and define `UPRINTF_IMPLEMENTATION` before the include:

```c
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"
```

This will copy library implementation to be built as a part of this file.

**NOTE: regular printf types are not supported**

`uprintf(fmt, ...)`: fmt is a string to be printed where `%S` will be replaced with a struct. Structures must be passed as non-`void*` pointers.

### Options

There are also options which can be set by defining other macros before *implementation*:

macro |  description | default
-|--|
UPRINTF_INDENTATION_WIDTH | The number of spaces to use for indentation | 4
UPRINTF_MAX_DEPTH | How deep can nested structures be | 10
