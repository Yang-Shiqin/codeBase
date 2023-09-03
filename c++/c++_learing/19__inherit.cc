#include <iostream>
using namespace std;

class Person{
public:
    Person(){}
    Person(int age, string name): age(age), name(name){}
    int get_age(){
        cout << "age: " << age << endl;
        return age;
    }
    string get_name(){
        cout << "name: " << name << endl;
        return name;
    }
private:
    int age;
    string name;
};

// 公共继承
class Student: public Person{
public:
    Student(){}
    Student(int No, int age=18, string name="liming"): No(No), Person(age, name){}
    int get_No(){
        cout << "No: " << No << endl;
        return No;
    }
private:
    int No;
};

int main()
{
    Student s(1);
    s.get_name();
    s.get_age();
    s.get_No();
    cout << sizeof(string) << endl;
    cout << sizeof(int) << endl;
    // 40应该是8字节对齐，子类不可见父类private但也会继承下来，48应该是40+4后再对齐
    cout << "sizeof(Person): " << sizeof(Person) << ", sizeof(Student): " << sizeof(Student) << endl;
    return 0;
}

