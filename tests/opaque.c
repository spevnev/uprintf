#include "uprintf.h"

int main(void) {
    int i = 1;
    struct A *opaque = (void *) &i;
    uprintf("opaque struct: %S\n", &opaque);
    return _upf_test_status;
}
