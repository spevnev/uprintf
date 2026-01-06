#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "uprintf.h"

void void_fun(void) {}

int other_fun(char a, float *b, unsigned long c) {
    (void) a;
    (void) b;
    (void) c;
    return 1;
}

int main(void) {
    int i = 1;

    uint8_t u8 = 255;
    uint16_t u16 = 65535;
    uint32_t u32 = 4294967295;
    uint64_t u64 = 18446744073709551615UL;

    int8_t i8 = -128;
    int16_t i16 = -32768;
    int32_t i32 = -2147483648;
    int64_t i64 = -9223372036854775807L;

    float f32 = 0.123;
    double f64 = -0.321;

    signed char sch = 'c';
    unsigned char uch = -'c';

    void *void_ptr = &i;
    int *int_ptr = &i;
    float *null_ptr = NULL;

    const char *str = "string";

    bool b = false;

    void (*void_fun_v)(void) = &void_fun;
    int (*other_fun_v)(char, float *, unsigned long) = &other_fun;

    uprintf("u8: %S\n", &u8);
    uprintf("u16: %S\n", &u16);
    uprintf("u32: %S\n", &u32);
    uprintf("u64: %S\n", &u64);

    uprintf("i8: %S\n", &i8);
    uprintf("i16: %S\n", &i16);
    uprintf("i32: %S\n", &i32);
    uprintf("i64: %S\n", &i64);

    uprintf("f32: %S\n", &f32);
    uprintf("f64: %S\n", &f64);

    uprintf("sch: %S\n", &sch);
    uprintf("uch: %S\n", &uch);

    uprintf("void_ptr: %S\n", &void_ptr);
    uprintf("int_ptr: %S\n", &int_ptr);
    uprintf("null_ptr: %S\n", &null_ptr);

    uprintf("str: %S\n", &str);

    uprintf("b: %S\n", &b);

    uprintf("void_fun: %S\n", &void_fun_v);
    uprintf("other_fun: %S\n", &other_fun_v);

    return _upf_test_status;
}
