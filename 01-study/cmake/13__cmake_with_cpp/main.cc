#include <iostream>
#include "dog.h"
#include "cat.h"
#include "config.h"

int main()
{
    dog d;
    d.bark();
    cat c;
    c.bark();
    std::cout << CMAKE_CXX_STANDARD << std::endl;
    return 0;
}

