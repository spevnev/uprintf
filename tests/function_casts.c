#include "uprintf.h"

int *i_fun(int i) {
    static int j;
    j = i;
    return &j;
}

int *(*j_fun(char c))(int i) {
    (void) c;
    return i_fun;
}

int *(*(**k_fun(void))(char c))(int i) {
    static int *(*(*t)(char) )(int) = j_fun;
    return &t;
}

int main(void) {
    uprintf("%S\n", ((int *(*) (int) ) i_fun)(1));
    uprintf("%S\n", ((int *((*) )(int) ) i_fun)(2));
    uprintf("%S\n", ((int *((*) (int) )) i_fun)(3));

    uprintf("%S\n", (((int *((*(*) (char) )(int) )) j_fun)('a')(4)));
    uprintf("%S\n", (((int *((*((*) (char) ))(int) )) j_fun)('b')(5)));
    uprintf("%S\n", (((int *((*(((*) (char) )))(int) )) j_fun)('c')(6)));

    uprintf("%S\n", (*((int *(*(**(*) (void) )(char) )(int) ) k_fun)())('d')(7));
    uprintf("%S\n", (*((int *(*((**(*) (void) )(char) ))(int) ) k_fun)())('e')(8));

    return _upf_test_status;
}
