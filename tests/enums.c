#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

enum DefaultEnum { ZERO, ONE, TWO, THREE };

enum PositiveEnum { FIVE = 5, SEVEN = 7 };

enum NegativeEnum { MINUS_THREE = -3, MINUS_TWO, MINUS_ONE, MINUS_ZERO };

int main(void) {
    enum DefaultEnum d1 = ZERO, d2 = THREE;
    uprintf("Default enums: %S, %S\n", &d1, &d2);

    enum PositiveEnum p1 = FIVE, p2 = SEVEN;
    uprintf("Positive enums: %S, %S\n", &p1, &p2);

    enum NegativeEnum n1 = MINUS_TWO, n2 = MINUS_ZERO;
    uprintf("Negative enums: %S, %S\n", &n1, &n2);

    uprintf("Different zero enums: %S, %S\n", &d1, &n2);

    enum DefaultEnum i1 = -10;
    enum NegativeEnum i2 = 10;
    uprintf("Invalid enums: %S, %S\n", &i1, &i2);

    return _upf_test_status;
}
