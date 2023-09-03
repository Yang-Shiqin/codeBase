#include <iostream>
using namespace std;

class MyPrint{
public:
    // 仿函数
    void operator()(string s){
        cout << s << endl;
    }
};

class MyAdd{
public:
    int operator()(int a, int b){
        return a+b;
    }
};

int main()
{
    MyPrint p;
    p("sth1");
    MyPrint()("sth2");
    int a = MyAdd()(1,2);
    cout << a << endl;
    return 0;
}

