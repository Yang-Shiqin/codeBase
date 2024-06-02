#include <iostream>
using namespace std;

int f(int a []){
    return sizeof(a);
}

int main()
{
    int a [] = {1,2,3,4,5};
    int s1, s2, s3;
    s1 = sizeof(a);
    int *p = a;
    s2 = sizeof(p);
    s3 = f(a);
    printf("%d %d %d", s1, s2, s3);
    return 0;
}

