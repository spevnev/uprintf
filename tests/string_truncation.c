#define UPRINTF_MAX_STRING_LENGTH 10
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    const char *string10 = "0123456789";
    const char *string11 = "01234567890";

    uprintf("10-char string: %S\n", &string10);
    uprintf("11-char string: %S\n", &string11);

    return _upf_test_status;
}
