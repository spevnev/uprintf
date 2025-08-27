#include "uprintf.h"

int main(void) {
    char c = 'c';
    int i = 1;

    uprintf("int %S\n", ((void) c, &i));
    uprintf("int %S\n", ((void) ((void) c, i), &i));
    uprintf("int %S\n", ((void) "string with ,", &i));
    uprintf("int %S\n", ((void) "string with \" and ,", &i));

    return _upf_test_status;
}
