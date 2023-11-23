#include <iostream>

class A{
    friend class B;  // 全局函数做友元
    int a;
};

class B{
    
public:
    B(){
        this->money = A::a;
    }
    int money;
    int c(A aa){
        return aa.a;
    }
};

class Person{
    // 定义友元
    friend void show_money(Person *p);  // 全局函数做友元
    // friend class Puppy;              // 类做友元
public:
    int get_money(){
        return this->money;
    }
    void set_money(int money){
        this->money = money;
    }
private:
    int money;
};



void show_money(Person *p){
    std::cout << p->money << std::endl;
}

int main()
{
    return 0;
}

