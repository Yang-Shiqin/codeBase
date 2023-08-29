#include <iostream>

class P{
public:
    P& age_inc(){   // 不返回引用的话就是P的拷贝，或者也可以返回指针
        this->age++;
        return *this;
    }
    P(){}
    P(int age):age(age){}
    int get_age(){
        return this->age;
    }

private:
    int age;
};


int main()
{
    P p(10);
    std::cout << p.get_age() << std::endl;
    p.age_inc().age_inc();
    std::cout << p.get_age() << std::endl;
    return 0;
}

