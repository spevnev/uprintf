#include "uprintf.h"

int main(void) {
    {
        typedef int S;
        S i = 1;
        uprintf("int %S\n", (S *) &i);
    }
    {
        typedef float S;
        S f = 2.3;
        uprintf("float %S\n", (S *) &f);
    }
    {
        typedef const char *S;
        S str = "str";
        uprintf("%S\n", (S *) &str);
    }
    return _upf_test_status;
}
