#include <stdlib.h>
#include "uprintf.h"

struct B;

typedef struct {
    const char *value;
} C;

typedef struct A {
    int value;
    struct B *b;
    C *c;
} A;

typedef struct B {
    float value;
    A *a;
    C *c;
} B;

int main(void) {
    C c = {
        .value = "value",
    };
    A a = {
        .value = 1,
        .b = NULL,
        .c = &c,
    };
    B b = {
        .value = 1.23,
        .a = &a,
        .c = &c,
    };
    a.b = &b;

    uprintf("Circular struct: %S\n", &a);

    return _upf_test_status;
}
