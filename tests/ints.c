#include "uprintf.h"

int main(void) {
    unsigned long i = 18446744073709551615UL;

    // int16_t
    uprintf("%S\n", (short*) &i);
    uprintf("%S\n", (int short*) &i);
    uprintf("%S\n", (short int*) &i);
    uprintf("%S\n", (signed short*) &i);
    uprintf("%S\n", (short signed*) &i);
    uprintf("%S\n", (int signed short*) &i);
    uprintf("%S\n", (int short signed*) &i);
    uprintf("%S\n", (signed int short*) &i);
    uprintf("%S\n", (short int signed*) &i);
    uprintf("%S\n", (signed short int*) &i);
    uprintf("%S\n", (short signed int*) &i);

    // uint16_t
    uprintf("%S\n", (unsigned short*) &i);
    uprintf("%S\n", (short unsigned*) &i);
    uprintf("%S\n", (int unsigned short*) &i);
    uprintf("%S\n", (int short unsigned*) &i);
    uprintf("%S\n", (unsigned int short*) &i);
    uprintf("%S\n", (short int unsigned*) &i);
    uprintf("%S\n", (unsigned short int*) &i);
    uprintf("%S\n", (short unsigned int*) &i);

    // int32_t
    uprintf("%S\n", (int*) &i);
    uprintf("%S\n", (signed*) &i);
    uprintf("%S\n", (signed int*) &i);
    uprintf("%S\n", (int signed*) &i);

    // uint32_t
    uprintf("%S\n", (unsigned*) &i);
    uprintf("%S\n", (unsigned int*) &i);
    uprintf("%S\n", (int unsigned*) &i);

    // int64_t
    uprintf("%S\n", (long*) &i);
    uprintf("%S\n", (int long*) &i);
    uprintf("%S\n", (long int*) &i);
    uprintf("%S\n", (signed long*) &i);
    uprintf("%S\n", (long signed*) &i);
    uprintf("%S\n", (int signed long*) &i);
    uprintf("%S\n", (int long signed*) &i);
    uprintf("%S\n", (signed int long*) &i);
    uprintf("%S\n", (long int signed*) &i);
    uprintf("%S\n", (signed long int*) &i);
    uprintf("%S\n", (long signed int*) &i);

    // uint64_t
    uprintf("%S\n", (unsigned long*) &i);
    uprintf("%S\n", (long unsigned*) &i);
    uprintf("%S\n", (int unsigned long*) &i);
    uprintf("%S\n", (int long unsigned*) &i);
    uprintf("%S\n", (unsigned int long*) &i);
    uprintf("%S\n", (long int unsigned*) &i);
    uprintf("%S\n", (unsigned long int*) &i);
    uprintf("%S\n", (long unsigned int*) &i);

    // int64_t
    uprintf("%S\n", (long long*) &i);
    uprintf("%S\n", (int long long*) &i);
    uprintf("%S\n", (long int long*) &i);
    uprintf("%S\n", (long long int*) &i);
    uprintf("%S\n", (signed long long*) &i);
    uprintf("%S\n", (long signed long*) &i);
    uprintf("%S\n", (long long signed*) &i);
    uprintf("%S\n", (int signed long long*) &i);
    uprintf("%S\n", (int long signed long*) &i);
    uprintf("%S\n", (int long long signed*) &i);
    uprintf("%S\n", (signed int long long*) &i);
    uprintf("%S\n", (signed long int long*) &i);
    uprintf("%S\n", (signed long long int*) &i);
    uprintf("%S\n", (long int signed long*) &i);
    uprintf("%S\n", (long int long signed*) &i);
    uprintf("%S\n", (long signed int long*) &i);
    uprintf("%S\n", (long signed long int*) &i);
    uprintf("%S\n", (long long int signed*) &i);
    uprintf("%S\n", (long long signed int*) &i);

    // uint64_t
    uprintf("%S\n", (unsigned long long*) &i);
    uprintf("%S\n", (long unsigned long*) &i);
    uprintf("%S\n", (long long unsigned*) &i);
    uprintf("%S\n", (int unsigned long long*) &i);
    uprintf("%S\n", (int long unsigned long*) &i);
    uprintf("%S\n", (int long long unsigned*) &i);
    uprintf("%S\n", (unsigned int long long*) &i);
    uprintf("%S\n", (unsigned long int long*) &i);
    uprintf("%S\n", (unsigned long long int*) &i);
    uprintf("%S\n", (long int unsigned long*) &i);
    uprintf("%S\n", (long int long unsigned*) &i);
    uprintf("%S\n", (long unsigned int long*) &i);
    uprintf("%S\n", (long unsigned long int*) &i);
    uprintf("%S\n", (long long int unsigned*) &i);
    uprintf("%S\n", (long long unsigned int*) &i);

    return _upf_test_status;
}
