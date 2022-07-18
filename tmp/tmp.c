#include <stdio.h>
#include <math.h>

int main()
{
    int a = 3;
    int b = 4;
    int ans = 0;
    ans |= (a<b)<<1;
    printf("%d, %f", ans, sqrt(3));
    return 0;
}

