#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    int arr1d[3] = {5, 3, 1};

    uprintf("1D array: %S\n", &arr1d);

    int arr3d[2][2][3] = {{{0, 1, 0}, {2, 3, 2}}, {{4, 5, 4}, {6, 7, 6}}};

    uprintf("2x2x3 array %S\n", &arr3d);
    uprintf("2x2x1 subarray %S\n", &arr3d[1]);
    uprintf("2x1x1 subarray %S\n", &arr3d[1][1]);

    uprintf("2x2x1 array %S\n", arr3d);
    uprintf("2x1x1 array %S\n", arr3d[1]);
    uprintf("int %S\n", arr3d[1][1]);

    return _upf_test_status;
}
