#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    int i = 6;
    void *v = &i;

    uprintf("invalid pointer %S\n", v);
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("void* %S\n", &v);

    void **vv = &v;
    uprintf("void** %S, void* %S\n", &vv, vv);

    void ***vvv = &vv;
    uprintf("void*** %S, void** %S\n", &vvv, vvv);

    void *varr[3] = {v, (void *) vv, (void *) vvv};
    uprintf("void*[] %S\n", &varr);

    return _upf_test_status;
}
