#include "lib.h"

#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

typedef struct {
    int b;
} LibPrivate;

void print_common(Common *s) { uprintf("Common: %S\n", s); }
void print_lib_private() { uprintf("LibPrivate: %S\n", &(LibPrivate) {.b = 2}); }
void print_bin_private(struct BinPrivate *s) { uprintf("BinPrivate: %S\n", s); }
