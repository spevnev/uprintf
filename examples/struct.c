// Example of printing a simple structure.
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

typedef struct {
    unsigned short a;
    long int b;
    const char *text;
    double f;
    int *p;
} MyStruct;

int main(void) {
    int i = 5;
    MyStruct s = {100, 52, "test string", 0.52, &i};

    uprintf("This is my struct: %a\n", &s);

    return 0;
}