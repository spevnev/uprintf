#include <new>
#include "uprintf.h"

struct S {
    int i;
    float f;
};

int main() {
    // Clang doesn't produce type definition for struct when it is only created
    // with new. Declare and use as local variable as a workaround.
    S s{1, 0.1f};
    uprintf("%S\n", &s);

    int ibuf;
    uprintf("%S\n", ::new int(1));
    uprintf("%S\n", new (&ibuf) int(2));
    uprintf("%S\n", new (S){1, 2.3f});
    return _upf_test_status;
}
