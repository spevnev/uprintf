#include "uprintf.h"

struct A {
    int *i;
    int *f() { return i; }
};

struct O {
    int *A::*m;
    int *(A::*f)();
};

int main() {
    int i = 3;
    A a{&i};

    int *A::*m = &A::i;
    int *(A::*f)() = &A::f;

    uprintf("%S\n", a.*m);
    uprintf("%S\n", (a.*f)());

    A *a_ptr = &a;
    uprintf("%S\n", a_ptr->*m);
    uprintf("%S\n", (a_ptr->*f)());

    O o{&A::i, &A::f};
    uprintf("%S\n", a.*(o.m));
    uprintf("%S\n", (a.*o.f)());

    return _upf_test_status;
}
