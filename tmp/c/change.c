#include <stdio.h>
#include <stdint.h>

int main()
{
    uint64_t a=0x1020304050607080, b=0,c=0;
    //char ca[8]="12345678";
    //char ca[8]={0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
    //char cb[8]={0};
    int i;
    for(i=0; i<8; i++){
    printf("%lx\n", b);
        //c = ((char*)&a)[i];
        //printf("%lx\n",c);
        b += ((uint64_t)(((char*)&a)[i]&0xff)<<(i*8));
        //b += (c<<(i*8));
    }
    //uint64_t* t=(uint64_t*)cb;
    //*t = ((uint64_t*)ca)[0];
    //printf("%lx", (uint64_t)*t);

    return 0;
}

