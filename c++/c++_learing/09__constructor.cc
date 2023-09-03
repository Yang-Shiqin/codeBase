#include <iostream>
using namespace std;

class Person{
public:
    // 无参构造
    Person(){
        name = "ysq";
        age = 22;
    }
    // 有参构造
    Person(string n, int a): name(n), age(a){}
    // 拷贝构造
    Person(const Person & p){
        static int i;
        cout << "copy " << i++ << endl;
        // name = p.get_name();
        // age = p.get_age();
        name = p.name;
        age = p.age;
    }
    string get_name(){
        return name;
    }
    int get_age(){
        return age;
    }
private:
    string name;
    int age;
};

Person func(Person p){
    return p;
}

int main()
{
    Person p1;              // 括号法
    Person p2("lza", 22);   // 括号法
    Person p3(p2);          // 括号法拷贝构造初始化
    Person p4 = Person();   // 显式法
    Person p5 = {"lza",23}; // 隐式法
    Person p6 = func(p5);   // 传参时拷贝构造了，返回时也拷贝构造了
    // Person(p6);          // 不能拷贝构造匿名对象
    Person() = p1;          // 匿名对象做左值
    cout << "p1: " << p1.get_name() << ", " << p1.get_age() << endl;
    cout << "p2: " << p2.get_name() << ", " << p2.get_age() << endl;
    cout << "p3: " << p3.get_name() << ", " << p3.get_age() << endl;
    cout << "p4: " << p4.get_name() << ", " << p4.get_age() << endl;
    cout << "p5: " << p5.get_name() << ", " << p5.get_age() << endl;
    cout << "p6: " << p6.get_name() << ", " << p6.get_age() << endl;
    return 0;
}

