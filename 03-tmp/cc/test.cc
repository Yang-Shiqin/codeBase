#include <iostream>
using namespace std;

class A{
private:
    int a;
public:
    A(){cout<<"A1"<<endl;}
    A(int a): a(a){
        cout<<"A"<<endl;
    }
    void f(){
        cout<<a<<endl;
    }
};

class B: A{
private:
    int b;
    int c;
public:
    B(int a, int b, int c):A(){
        A w(a);
        cout << "B" << endl;
    }
};

int main()
{
    B b(1,2,3);
    return 0;
}

