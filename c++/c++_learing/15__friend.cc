#include <iostream>

class Person;

class Puppy{
public:
    Person *master;
    void eat();
};

class Person{
    // 定义友元
    friend void show_money(Person *p);  // 全局函数做友元
    // friend class Puppy;              // 类做友元
    friend void Puppy::eat();           // 类内成员函数做友元
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

void Puppy::eat(){
    master->money--;
    std::cout << "puppy eats meat, money--. money=" << master->money << std::endl;
}


void show_money(Person *p){
    std::cout << p->money << std::endl;
}

int main()
{
    Person p;
    // 类友元
    Puppy d;
    d.master = &p;

    p.set_money(33);
    // 函数友元
    show_money(&p);
    // 类友元/类成员函数友元
    d.eat();
    return 0;
}

