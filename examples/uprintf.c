#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    uprintf("uprintf's state: %S\n", &_upf_state);
    return 0;
}
