#include <stdio.h>
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    FILE *file = fopen("/proc/self/exe", "r");

    uprintf("stdio.h's FILE: %S\n", file);

    fclose(file);
    return _upf_test_status;
}
