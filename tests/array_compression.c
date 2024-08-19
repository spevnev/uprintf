#include <stdint.h>
#include <string.h>
#include "uprintf.h"

int main(void) {
    uint8_t array[32];
    memset(array, 1, 32);
    uprintf("Array of 1s: %S\n", &array);

    uint8_t arrays[16][8];
    memset(arrays, 2, 16 * 8);
    uprintf("Array of arrays of 2s: %S\n", &arrays);

    return _upf_test_status;
}
