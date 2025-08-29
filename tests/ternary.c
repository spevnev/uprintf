#include <stdbool.h>
#include "uprintf.h"

int main(void) {
    int i = -1;
    int j = 1;

    uprintf("%S\n", true ? &j : &i);
    uprintf("%S\n", true ? false ? &i : &j : &i);
    uprintf("%S\n", false ? &i : false ? &i : &j);

    return _upf_test_status;
}
