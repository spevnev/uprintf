#include "uprintf.h"

struct P {
    P(int i) : p(i) {}

    int p;
};

struct C : P {
    C(int i, int j) : P(i), c(j) {}

    int c;
};

int main() {
    int i = 5;
    int *i_ptr = &i;
    void *ptr = static_cast<void *>(i_ptr);
    C *c = new C(1, 2);

    uprintf("int: %S\n", static_cast<int *>(ptr));
    uprintf("float: %S\n", reinterpret_cast<float *>(ptr));
    uprintf("const int: %S\n", const_cast<const int *>(i_ptr));
    uprintf("struct: %S\n", dynamic_cast<P *>(c));

    delete c;
    return _upf_test_status;
}
