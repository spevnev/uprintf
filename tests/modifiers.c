#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

typedef struct {
    int i;
    const int ci;
    volatile int vi;

    int *p;
    const int *cp;
    int *const pc;
    const int *const cpc;
    volatile int *vp;
    int *volatile pv;
    volatile int *volatile vpv;
    int *restrict pr;
    const volatile int *restrict const volatile cvprcv;
} Modifiers;

int main(void) {
    int i = 5;
    Modifiers m = {
        .i = 1,
        .ci = 2,
        .vi = 3,
        .p = &i,
        .cp = &i,
        .pc = &i,
        .cpc = &i,
        .vp = &i,
        .pv = &i,
        .vpv = &i,
        .pr = &i,
        .cvprcv = &i,
    };

    uprintf("Modifiers: %S\n", &m);

    return _upf_test_status;
}
