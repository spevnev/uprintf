#include <stdlib.h>
#include "uprintf.h"

int main(void) {
    uprintf("%S\n", &(struct inline_anonymous_struct { int a; }) {1});
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("%S\n", &(struct { int a; }) {1});
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("%S\n", &(union inline_anonymous_union { int a; }) {1});
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    _upf_test_status = EXIT_SUCCESS;

    uprintf("%S\n", &(union { int a; }) {1});
    if (_upf_test_status == EXIT_SUCCESS) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
