#include <iostream>
class Singleton {
private:
    // 私有构造函数
    Singleton() {}

public:
    // 获取实例的静态方法
    static Singleton& getInstance() {
        static Singleton instance; // 懒汉式，第一次调用时创建
        return instance;
    }
};

// 使用示例
int main() {
    Singleton& singleton1 = Singleton::getInstance();
    Singleton& singleton2 = Singleton::getInstance();
    Singleton s3 = singleton2;

    // 打印单例对象的地址
    std::cout << "Address of singleton1: " << &singleton1 << std::endl;
    std::cout << "Address of singleton2: " << &singleton2 << std::endl;
    std::cout << "Address of singleton2: " << &s3 << std::endl;
    return 0;
}
