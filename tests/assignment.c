#include <stdint.h>
#include "uprintf.h"

int main(void) {
    uint32_t u = UINT32_MAX;
    uint32_t *u_ptr;
    uprintf("uint %S\n", u_ptr = &u);

    // Silence "pointers of different types" warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
    int32_t *i_ptr;
    uprintf("int %S\n", i_ptr = &u);
#pragma GCC diagnostic pop

    return _upf_test_status;
}
