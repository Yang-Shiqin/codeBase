#include <iostream>

void func(int &a){
    a = 99;
}

int & func2(){
    static int a = 10;
    return a;
}

int main()
{
    int a = 10;
    int &b = a;
    std::cout << "a=" << a << ", b=" << b << std::endl;
    b = 20;
    std::cout << "a=" << a << ", b=" << b << std::endl;
    // 引用做参数
    func(a);
    std::cout << "a=" << a << ", b=" << b << std::endl;
    // 引用做返回值
    int & c = func2();
    std::cout << "c=" << c << std::endl;
    func2() = 100;
    std::cout << "c=" << c << std::endl;
    return 0;
}

