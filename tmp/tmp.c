#include <stdio.h>
#include <math.h>

int main()
{
    double a = 3.88;
    char b = (char)a;
    char c = (char)(int)a;
    int d = (int) a;
    printf("%d %d %d", b, c, d);
    return 0;
}

