#include <stdio.h>

#define NAMES(X) X(lza) X(ysq)

int main(){
    #define PRINT(x) puts("Hello, " #x "!");
    NAMES(PRINT)
}
