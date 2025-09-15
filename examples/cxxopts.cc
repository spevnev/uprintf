#define CXXOPTS_NO_REGEX
#include "cxxopts.hpp"  // https://github.com/jarro2783/cxxopts
#include <cstdlib>

#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(int argc, char **argv) {
    cxxopts::Options options("test", "A brief description");

    options.add_options()                                                                //
        ("b,bar", "Param bar", cxxopts::value<std::string>())                            //
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))  //
        ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))               //
        ("h,help", "Print usage");
    uprintf("Options: %S\n", &options);

    auto result = options.parse(argc, argv);
    uprintf("Result: %S\n", &result);

    return EXIT_SUCCESS;
}
