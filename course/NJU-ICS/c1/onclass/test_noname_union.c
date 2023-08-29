#include <stdio.h>
typedef unsigned int        uint32_t;
typedef unsigned short int  uint16_t;
typedef unsigned char       uint8_t;
struct B{
union C{
    union A{
        uint32_t _32;
        uint16_t _16;
        uint8_t _8[2];
    }a[8];
    struct {uint32_t x1, x2, x3, x4, x5, x6, x7, x8;};
}c;
}b;
int main(){
    b.a[0]._32 = 0x12345678;
    b.a[1]._32 = 0x90901212;
    printf("%x \n%hx \n%x", b.a[0]._32, b.a[0]._16, b.a[0]._8[0]);
    printf("%x\n%x\n", b.x1, b.x2);
    return 0;
}
