#include "uprintf.h"

int main(void) {
    int arr[] = {1, 2, 3};
    int *ptr = arr;

    uprintf("3rd element: %S\n", arr + 2);
    uprintf("2nd element: %S\n", ++ptr);
    uprintf("1st element: %S\n", ptr - 1);

    return _upf_test_status;
}
