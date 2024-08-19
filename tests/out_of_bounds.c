#include "uprintf.h"

int main(void) {
    // random, probably out-of-bounds, address
    char *out_of_bounds = (char *) 0x10000000;
    uprintf("Out-of-bounds string pointer: %S\n", &out_of_bounds);

    return _upf_test_status;
}
