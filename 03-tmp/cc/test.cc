#include <iostream>
using namespace std;

class A{
public:
    int a;
    void f(){
        cout<<a<<endl;
    }
};

int main()
{
    class A aa, bb;
    aa.a = 1;
    aa.f();
    cout << aa.f << " " << bb.f << endl;
    return 0;
}

