#include <stdlib.h>
#include "lib.h"

typedef struct BinPrivate {
    int c;
} BinPrivate;

int main(void) {
    print_common(&(Common) {.a = 1});
    print_lib_private();
    print_bin_private(&(BinPrivate) {.c = 3});
    return EXIT_SUCCESS;
}
