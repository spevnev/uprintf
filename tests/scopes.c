#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "uprintf.h"

static char var = 'v';

int main(void) {
    uprintf("char = %S\n", &var);

    int var = 1;
    uprintf("int = %S\n", &var);
    {
        double var = 1.234;
        uprintf("double = %S\n", &var);
    }

    {
        const char *var = "string variable";
        uprintf("string = %S\n", &var);
    }

    while (var++ < 3) {
        int8_t var = -5;
        uprintf("int8_t = %S\n", &var);
    }

    for (size_t var = 3; var < 5; var++) {
        uprintf("size_t = %S\n", &var);
    }

    {
        const char *var = "var";
        do {
            float var = 0.123;
            {
                int var = 333;
                if (var > 0) {
                    bool var = false;
                    for (int var = 0; var < 1; var++) {
                        void *var = NULL;
                        { uprintf("void* %S\n", &var); }
                    }
                    uprintf("bool %S\n", &var);
                }
                uprintf("int %S\n", &var);
            }
            uprintf("float %S\n", &var);
        } while (0);
        uprintf("c_str %S\n", &var);
    }

    {
        uprintf("int = %S\n", &var);
        char var = 'c';
        uprintf("char = %S\n", &var);
    }

    return _upf_test_status;
}
