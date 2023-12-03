#include <iostream>
using namespace std;

int add(int x1, int x2=10){
    return x1+x2;
}

double add(double x1, double x2){
    return x1+x2;
}

int main()
{
    int a = add(1,2);
    double b = add(1.,2.);
    cout << a << " " << b << endl;
    return 0;
}

