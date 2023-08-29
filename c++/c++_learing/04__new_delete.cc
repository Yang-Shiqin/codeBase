#include <iostream>

int * create_arr(int len){
    int *arr = new int[len];
    return arr;
}

void destroy_arr(int * arr){
    delete[] arr;
}

int main()
{
    int * p = new int(30);
    int * arr = create_arr(30);
    std::cout << "create one element point, sizeof(p)=" << sizeof(p) << ", *p=" << *p << std::endl;
    std::cout << "create 30 elements point, sizeof(arr)=" << sizeof(arr) << ", arr=" << arr << std::endl;
    delete p;
    destroy_arr(arr);
    return 0;
}

