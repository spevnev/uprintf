#include <stdlib.h>
#include "uprintf.h"

int main(void) {
    int num = 100;

    uprintf("Different format specifiers: %w %B %y %R %l %K %m %S %z\n", &num, &num, &num, &num, &num, &num, &num, &num, &num);
    uprintf("Escaped format specifier: %S %%\n", &num);
    if (_upf_test_status == EXIT_FAILURE) return EXIT_FAILURE;

    uprintf("Unfinished format specifier: %S %", &num);
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("Unfinished format specifier: %S %\n", &num);
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("Invalid format specifier: %S %3\n", &num);
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("Invalid format specifier: %S %-\n", &num);
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
