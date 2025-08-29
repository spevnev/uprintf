#include "uprintf.h"

typedef struct {
    int i;
    const char *str;
} S;

int main(void) {
    // It is valid to get pointers to the compound literals, because they are
    // implicitly declared on stack (i.e. as if they were local variables in main).
    uprintf("%S\n", &(int) {1});
    uprintf("%S\n", &(const char *) {"str"});

    // Clang doesn't produce type definition for struct when it is only used in
    // compound literals. Declare and use as local variable as a workaround.
    S s = {1, "clang"};
    uprintf("%S\n", &s);

    uprintf("%S\n", &(S) {1, "initializer"});
    uprintf("%S\n", &(S) {
                        .str = "designated initializer",
                        .i = 2,
                    });

    return _upf_test_status;
}
