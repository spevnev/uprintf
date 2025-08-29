#include <stdlib.h>
#include "uprintf.h"

static int i;

static int *foo(int num) {
    i = num;
    return &i;
}

static int *bar(int num) {
    i = num * 2;
    return &i;
}

#define generic_macro(i) _Generic(i, int: foo, unsigned int: bar)(i)

int main(void) {
    int signed_int = 1;
    uprintf("%S\n", generic_macro(signed_int));
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
