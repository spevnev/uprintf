#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    uint8_t ubytes[32];
    for (size_t i = 0; i < 32; i++) ubytes[i] = i * 2;
    uprintf("Array of bytes: %S\n", &ubytes);

    unsigned char uchars[32];
    for (size_t i = 0; i < 32; i++) uchars[i] = i % 2 == 0 ? 'A' : 'B';
    uprintf("Array of chars: %S\n", &uchars);

    int8_t sbytes[32];
    for (size_t i = 0; i < 32; i++) sbytes[i] = i * 2;
    uprintf("Array of bytes: %S\n", &sbytes);

    signed char schars[32];
    for (size_t i = 0; i < 32; i++) schars[i] = i % 2 == 0 ? 'A' : 'B';
    uprintf("Array of chars: %S\n", &schars);

    return _upf_test_status;
}
