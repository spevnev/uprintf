#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
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

    signed char uch = 'c';
    unsigned char sch = -'c';

    void *void_ptr = &i8;
    void *null_ptr = NULL;
    int i = 123;
    int *int_ptr = &i;
    const char *str = "const char *str";

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

    uprintf("uch: %S\n", &uch);
    uprintf("sch: %S\n", &sch);

    uprintf("void_ptr: %S\n", &void_ptr);
    uprintf("null_ptr: %S\n", &null_ptr);
    uprintf("int_ptr: %S\n", &int_ptr);
    uprintf("str: %S\n", &str);

    return _upf_test_status;
}
