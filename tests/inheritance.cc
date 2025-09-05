#include "uprintf.h"

struct A {
    int i;
};

struct B {
    const char *cstr;
};

struct C : A, B {
    float f;

    C(int i, const char *cstr, float f) : A{i}, B{cstr}, f(f) {}
};

int main() {
    C c{5, "str", 1.23F};
    uprintf("refs: %S\n", &c);
    return _upf_test_status;
}
