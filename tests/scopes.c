#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    // clang-format off
    int var = 1; uprintf("int = %S\n", &var); {double var = 1.234; uprintf("double = %S\n", &var);} {const char *var = "string variable"; uprintf("string = %S\n", &var);} while (var++ < 3) {int8_t var = -5; uprintf("int8_t = %S\n", &var);} for (size_t var = 3;var < 5;var++){uprintf("size_t = %S\n", &var);}
    // clang-format on

    return _upf_test_status;
}
