#include <iostream>
using namespace std;

class P{
public:
    int get_a(){
        return a;
    }
private:
    int a;
    int c;
    static int b;
};

int P::b = 10;

int main()
{
    P p;
    cout << sizeof(p) << endl;
    return 0;
}

