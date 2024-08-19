#include <stdlib.h>
#include "uprintf.h"

#define COUNT 5

typedef struct {
    int count;
    int flexible[];
} Flexible;

int main(void) {
    Flexible *flex = malloc(sizeof(*flex) + COUNT * sizeof(int));
    flex->count = COUNT;

    for (int i = 0; i < COUNT; i++) flex->flexible[i] = i * 3;

    uprintf("Struct with a flexible array member: %S\n", flex);

    free(flex);
    return _upf_test_status;
}
