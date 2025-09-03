#include <stdbool.h>
#include <stdint.h>
#include "uprintf.h"

typedef union {
    uint8_t bit : 3;
    uint64_t u64;
    float f32;
    int *int_ptr;
    const char *str;
    bool b;
    void (*fun)(void);
} Union;

void foo(void) {}

int main(void) {
    Union u1 = {.bit = 0x5};
    Union u2 = {.u64 = 18446744073709551615UL};
    Union u3 = {.f32 = 0.123F};
    int i = 1;
    Union u4 = {.int_ptr = &i};
    Union u5 = {.str = "string"};
    Union u6 = {.fun = foo};

    uprintf("Union with bit: %S\n", &u1);
    uprintf("Union with u64: %S\n", &u2);
    uprintf("Union with f32: %S\n", &u3);
    uprintf("Union with int*: %S\n", &u4);
    uprintf("Union with str: %S\n", &u5);
    uprintf("Union with function pointer: %S\n", &u6);

    return _upf_test_status;
}
