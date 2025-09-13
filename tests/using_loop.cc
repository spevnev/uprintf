#include "uprintf.h"

int v = 1;

namespace A {
namespace B {
using namespace A;

void fun() {
    __asm__ __volatile__("" ::"r"(v));
    uprintf("%S\n", &v);
}
};  // namespace B

using namespace A::B;
};  // namespace A

int main() {
    A::B::fun();
    return _upf_test_status;
}
