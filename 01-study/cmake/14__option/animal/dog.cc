#include "dog.h"
#ifdef USE_CAT
    #include "cat.h"
#endif
#include <iostream>

void dog::bark(){
#ifdef USE_CAT
    cat c;
    c.bark();
#else
    std::cout << "wang" << std::endl;
#endif
}
