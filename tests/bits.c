#include <stdbool.h>
#include <stdint.h>
#include "uprintf.h"

typedef struct {
    int byte_field1;
    uint16_t bit_field1 : 5;
    int : 0;
    uint8_t bit_field2 : 6;
    unsigned int bit_field3 : 2;
    uint64_t bit_field4 : 1;
    bool bit_field5 : 1;
    int byte_field2;
} BitFields;

int main(void) {
    BitFields fields = {
        .byte_field1 = 100,
        .bit_field1 = 31,
        .bit_field2 = 63,
        .bit_field3 = 3,
        .bit_field4 = 0,
        .bit_field5 = 1,
        .byte_field2 = -100,
    };

    uprintf("Bit fields: %S\n", &fields);

    return _upf_test_status;
}
