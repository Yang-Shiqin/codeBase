#include <stdio.h>
#include <string.h>

struct s{
    int a;
    union{
        int b;
    }u;
    int aaaa[0];
};

struct s func(){
    struct s res;
    res.u.b = 3;
    return res;
}

struct s func2(){
    struct s res;
    int c = 40;
    res.a = 22;
    return res;
}

int main()
{
    int a;

    short s = 1;
    s = s+1;
    return 0;
}


