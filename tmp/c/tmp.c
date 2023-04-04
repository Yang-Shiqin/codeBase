//#include <stdio.h>
int printf (const char * __format , ...);
int printf (const char * __format , ...){}
int main()
{
    printf("Hello world\n");
    printf("Hello world%d\n");
    printf("Hello world%d\n", 2);
    return 0;
}

