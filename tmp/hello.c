#include <stdio.h>
enum w{
    aa,
    bb
};
typedef struct b hh;
struct b{
    int qqq;
    int qq;
};
struct a{
    struct b aaa;
    int aaaa;
};

int main()
{
    struct a qqqqq;
    printf("%u", sizeof(struct a));
    return 0;
}

