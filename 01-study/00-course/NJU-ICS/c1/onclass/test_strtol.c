#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int num;
    char* str[] = {
        "123",
        "0123",
        "0x123d",
        "123d",
        "08",
        "sd",
        "1sd",
    };
    char *ptr_end=NULL;
    int i;
    for(i=0; i<(int)sizeof(str)/8; i++){
        printf("%s\n", str[i]);
        /* sscanf
        int v=0;
        sscanf(str[i], "%x", &v);
        printf("%d\n", v);
        */
        printf("%d\n", strtol(str[i], &ptr_end, 0));
        if(ptr_end!=str[i]+strlen(str[i]))
            printf("%s\n", ptr_end);
    }
    return 0;
}
