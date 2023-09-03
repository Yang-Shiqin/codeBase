#include <iostream>
using namespace std;

class Person{
private:
    int age;
};

// 公共继承
class Student: public Person{
private:
    int No;
};

int main()
{
    cout << sizeof(string) << endl;
    cout << sizeof(int) << endl;
    cout << "sizeof(Person): " << sizeof(Person) << ", sizeof(Student): " << sizeof(Student) << endl;
    return 0;
}

