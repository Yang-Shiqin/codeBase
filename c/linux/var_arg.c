#include <stdio.h>
#include <stdarg.h>

int add(int argc, ...){
    va_list arg_ptr;        // 指向参数
    int i, nArgValue;
    int sum=0;
    
    va_start(arg_ptr, argc);    // 初始化va_list为参数列表, 第二个参数是可变参数第一个named参数(在栈?寄存器?里就是最后一个参数)
    for(i=0; i<argc; i++){
        nArgValue = va_arg(arg_ptr, int);   // 访问下一个参数, 大小为int
        sum += nArgValue;
    }
    va_end(arg_ptr);            // 设va_list为null(清理)

    return sum;
}

int main()
{
    int a = add(3, 1,1,1);
    printf("%d\n", a);
    return 0;
}

