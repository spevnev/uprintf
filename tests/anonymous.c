#include <stdint.h>
#include "uprintf.h"

typedef struct {
    union {
        float f;
        uint32_t u;
    };
    struct {
        const char *str;
        int i;
    };
} S;

typedef union {
    struct {
        int i;
        float f;
    };
    union {
        uint32_t u;
    };
} U;

int main(void) {
    S s = {
        .f = 1.23F,
        .str = "str",
        .i = 1,
    };
    uprintf("%S\n", &s);

    U u = {
        .i = 321,
        .f = 1.23F,
    };
    uprintf("%S\n", &u);

    return _upf_test_status;
}
