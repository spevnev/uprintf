#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>  // IWYU pragma: keep
#include "uprintf.h"

typedef int8_t i8_t;
typedef char *cstr;
typedef double long_float;

typedef struct {
    uint8_t u8;
    unsigned short u16;
    unsigned u32;
    long unsigned long int u64;

    i8_t i8;
    short int i16;
    int32_t i32;
    signed long int long i64;

    signed char sch;
    unsigned char uch;

    float f32;
    long_float f64;

    void *void_ptr;
    int *int_ptr;
    float *null_ptr;

    const char *str;
    const cstr const_str;

    bool b;

    void (*void_fun)(void);
    int (*other_fun)(char, float *, unsigned long);
} Struct;

void void_fun(void) {}

int other_fun(char a, float *b, unsigned long c) {
    (void) a;
    (void) b;
    (void) c;
    return 1;
}

int main(void) {
    int i = 5;

    Struct s = {
        .u8 = 255,
        .u16 = 65535,
        .u32 = 4294967295,
        .u64 = 18446744073709551615UL,

        .i8 = -128,
        .i16 = -32768,
        .i32 = -2147483648,
        .i64 = -9223372036854775807L,

        .f32 = 0.123,
        .f64 = -0.321,

        .uch = 'c',
        .sch = -'c',

        .void_ptr = &i,
        .int_ptr = &i,
        .null_ptr = NULL,

        .str = "str string",
        .const_str = "const_str string",

        .b = true,

        .void_fun = &void_fun,
        .other_fun = &other_fun,
    };

    uprintf("Struct containing all types: %S\n", &s);

    return _upf_test_status;
}
