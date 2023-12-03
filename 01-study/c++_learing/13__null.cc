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
    int get_num(){
        return this->num;
    }
    int get_n(){
        return num;
    }
    void print(){
        std::cout << "sdf" << std::endl;
    }

private:
    int age;
    static int num;
};

int P::num = 0;

int main()
{
    P * n = NULL;
    n->get_n();
    n->print();
    n->get_num();
    //n->get_age();     // 可编译通过，但运行报错
    P p(10);
    std::cout << p.get_age() << std::endl;
    p.age_inc().age_inc();
    std::cout << p.get_age() << std::endl;
    std::cout << p.get_num() << std::endl;
    return 0;
}

