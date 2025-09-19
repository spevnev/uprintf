#include "uprintf.h"

class C {
public:
    int i = 1;
    float f = 1.2F;
    const char *cstr = "str";

    __attribute__((noinline)) void foo() { uprintf("C: %S\n", this); }
};

int main() {
    C{}.foo();
    return _upf_test_status;
}
