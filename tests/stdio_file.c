#include <stdio.h>
#define UPRINTF_IGNORE_STDIO_FILE 0
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    FILE *file = fopen("/proc/self/exe", "r");

    uprintf("stdio.h's FILE before read: %S\n", file);

    char buffer[100];
    // Read populates FILE's fields
    if (fread(buffer, 100, 1, file) < 1) {
        fprintf(stderr, "fread failed.\n");
        return EXIT_FAILURE;
    }

    uprintf("stdio.h's FILE after read: %S\n", file);

    fclose(file);
    return _upf_test_status;
}
