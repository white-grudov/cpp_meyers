#include <iostream>

#include "include/07Concurrency.h"

int main()
{
    auto tooManyThreads = []()
    {
        std::size_t numTasks = 50000;
        createTasksThread(numTasks, add);
    };

    tooManyThreads();

    return 0;
}