#include "uprintf.h"

struct S {
    int i;
    int &ir;
    int *&ptr_ref;

    S(int i, int &j, int *&ptr) : i(i), ir(j), ptr_ref(ptr) {}
};

int main() {
    int i = 2;
    int *ptr = &i;
    S s{1, i, ptr};
    uprintf("%S\n", &s);
    return _upf_test_status;
}
