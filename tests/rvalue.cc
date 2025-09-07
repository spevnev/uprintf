#include "uprintf.h"

void fun1(int &&i) { (void) i; }
void fun2(int *&&i) { (void) i; }

int main() {
    uprintf("%S\n", &fun1);
    uprintf("%S\n", &fun2);
    return _upf_test_status;
}
