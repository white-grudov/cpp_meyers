#include <iostream>

#include "include/07Concurrency.h"

int main()
{
    std::cout << incrementVolatile(5000) << '\n';

    return 0;
}