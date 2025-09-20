#include <cstdio>
#include "uprintf.h"

namespace I {

int v = 1;
struct S {
    int v;
};
int *f() { return &v; }

void fun1() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I %S\n", &v);
    uprintf("I %S\n", (S *) &v);
    uprintf("I %S\n", f());
    uprintf("I %S\n", &::I::v);
    uprintf("I %S\n", (::I::S *) &v);
    uprintf("I %S\n", ::I::f());
    uprintf("I %S\n", &I::v);
    uprintf("I %S\n", (I::S *) &v);
    uprintf("I %S\n\n", I::f());
}

struct P {
    int *v = &I::v;
    struct S {
        int *v;
    };
    int **f() { return &v; }
};

class C : public P {
public:
    char v = 'c';
    struct S {
        char v;
    };
    char *f() { return &v; }

    void fun() {
        S s = {v};
        uprintf("%d\n", &s);

        uprintf("C %S\n", &v);
        uprintf("C %S\n", (S *) &v);
        uprintf("C %S\n", f());
        uprintf("C %S\n", &this->v);
        uprintf("C %S\n", (S *) &this->v);
        uprintf("C %S\n\n", this->f());

        uprintf("P %S\n", &(P::v));
        uprintf("P %S\n", P::f());
        uprintf("P %S\n", &this->P::v);
        uprintf("P %S\n\n", this->P::f());
    }
};

namespace {

float v = 1.23F;
struct S {
    float v;
};
float *f() {
    __asm__ __volatile__("" ::"r"(v));
    return &v;
}

__attribute__((noinline)) void fun2() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I anonymous %S\n", &v);
    uprintf("I anonymous %S\n", (S *) &v);
    uprintf("I anonymous %S\n\n", f());
}

}  // namespace

namespace J {

const char *v = "str";
struct S {
    const char *v;
};
const char **f() { return &v; }

void fun1() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I::J %S\n", &v);
    uprintf("I::J %S\n", (S *) &v);
    uprintf("I::J %S\n", f());
    uprintf("I::J %S\n", &::I::J::v);
    uprintf("I::J %S\n", (::I::J::S *) &v);
    uprintf("I::J %S\n", ::I::J::f());
    uprintf("I::J %S\n", &I::J::v);
    uprintf("I::J %S\n", (I::J::S *) &v);
    uprintf("I::J %S\n", I::J::f());
    uprintf("I::J %S\n", &J::v);
    uprintf("I::J %S\n", (J::S *) &v);
    uprintf("I::J %S\n\n", J::f());
}

namespace {

bool v = true;
struct S {
    bool v;
};
bool *f() {
    __asm__ __volatile__("" ::"r"(v));
    return &v;
}

__attribute__((noinline)) void fun2() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I::J anonymous %S\n", &v);
    uprintf("I::J anonymous %S\n", (S *) &v);
    uprintf("I::J anonymous %S\n\n", f());
}

}  // namespace

namespace K {

void *v = NULL;
struct S {
    void *v;
};
void **f() { return &v; }

void fun1() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I::J::K %S\n", &v);
    uprintf("I::J::K %S\n", (S *) &v);
    uprintf("I::J::K %S\n", f());
    uprintf("I::J::K %S\n", &::I::J::K::v);
    uprintf("I::J::K %S\n", (::I::J::K::S *) &v);
    uprintf("I::J::K %S\n", ::I::J::K::f());
    uprintf("I::J::K %S\n", &I::J::K::v);
    uprintf("I::J::K %S\n", (I::J::K::S *) &v);
    uprintf("I::J::K %S\n", I::J::K::f());
    uprintf("I::J::K %S\n", &J::K::v);
    uprintf("I::J::K %S\n", (J::K::S *) &v);
    uprintf("I::J::K %S\n", J::K::f());
    uprintf("I::J::K %S\n", &K::v);
    uprintf("I::J::K %S\n", (K::S *) &v);
    uprintf("I::J::K %S\n\n", K::f());
}

namespace {

char v = 'c';
struct S {
    char v;
};
char *f() {
    __asm__ __volatile__("" ::"r"(v));
    return &v;
}

__attribute__((noinline)) void fun2() {
    S s = {v};
    uprintf("%d\n", &s);

    uprintf("I::J::K anonymous %S\n", &v);
    uprintf("I::J::K anonymous %S\n", (S *) &v);
    uprintf("I::J::K anonymous %S\n\n", f());
}

}  // namespace
}  // namespace K
}  // namespace J
}  // namespace I

int main() {
    I::fun1();
    I::fun2();
    I::J::fun1();
    I::J::fun2();
    I::J::K::fun1();
    I::J::K::fun2();
    I::C{}.fun();
    return _upf_test_status;
}
