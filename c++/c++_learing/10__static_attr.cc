#include <iostream>
using namespace std;

class Person{
public:
    static int number;
    static int get_country(){
        return country;
    }
private:
    static int country;
};

int Person:: number = 100;
int Person:: country = 10;

int main()
{
    cout << Person::number << endl;
    cout << Person::get_country() << endl;
    Person p1;
    Person p2;
    cout << p1.number << endl;
    p2.number = 2;
    cout << p1.number << endl;
    return 0;
}

