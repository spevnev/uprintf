#include "taskflow/taskflow.hpp"  // https://github.com/taskflow/taskflow
#include <cstdlib>

#define UPRINTF_IMPLEMENTATION
#define UPRINTF_MAX_DEPTH -1
#include "uprintf.h"

int main() {
    tf::Executor executor;
    tf::Taskflow taskflow;

    auto [A, B, C, D] = taskflow.emplace(  //
        []() { std::cout << "TaskA\n"; },  //
        []() { std::cout << "TaskB\n"; },  //
        []() { std::cout << "TaskC\n"; },  //
        []() { std::cout << "TaskD\n"; });

    A.precede(B, C);  // A runs before B and C
    D.succeed(B, C);  // D runs after  B and C

    executor.run(taskflow).wait();

    uprintf("executor: %S\n", &executor);
    uprintf("taskflow: %S\n", &taskflow);

    return EXIT_SUCCESS;
}
