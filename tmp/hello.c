#include <stdio.h>
#include <stdlib.h>
typedef enum{
    BAD_TOKEN,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    CR
}TokenType;
int hello(int a){
    if(a>0){
        printf("a>0\n");
        return 1;
    }
    exit(0);
    
}
int main()
{
    double a;
    hello(1);
    printf("%d\n", sizeof(TokenType));
    return 0;
}

