#include <cstdint>
#include "uprintf.h"

struct S {
    int64_t i;
    int &ir;
    int *&ptr_ref;

    S(int i, int &ir, int *&ptr) : i(i), ir(ir), ptr_ref(ptr) {}
};

void fun1(int &&i) { (void) i; }
void fun2(int *&i) { (void) i; }

int *i_fun(int i) {
    static int j;
    j = i;
    return &j;
}

int *(*j_fun(char c))(int i) {
    (void) c;
    return i_fun;
}

int main() {
    int i = 2;
    int *ptr = &i;
    S s{1, i, ptr};

    uprintf("%S\n", &s);

    uprintf("%S\n", &fun1);
    uprintf("%S\n", &fun2);

    uprintf("%S\n", (((int *((*(&) (char) )(int) )) j_fun)('a')(4)));
    uprintf("%S\n", (((int *((*(&&) (char) )(int) )) j_fun)('b')(5)));

    return _upf_test_status;
}
