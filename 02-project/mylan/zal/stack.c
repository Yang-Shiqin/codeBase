// 辅助gc, 保存临时变量; 顺便模拟eval函数值通过栈返回
#include "zal_in.h"

void zal_stack_init(ZAL_Interpreter* inter){
    inter->stack.stack = MEM_alloc(sizeof(ZAL_Value)*STACK_ALLOC_SIZE);
    inter->stack.size = STACK_ALLOC_SIZE;
    inter->stack.top = 0;
}

void zal_stack_push(ZAL_Interpreter* inter, ZAL_Value *value){
    DBG_assert(inter->stack.top<=inter->stack.size, (NULL));
    if(inter->stack.top == inter->stack.size){
        inter->stack.size += STACK_ALLOC_SIZE;
        inter->stack.stack = (ZAL_Value*)MEM_realloc(inter->stack.stack, sizeof(ZAL_Value)*inter->stack.size);
    }
    inter->stack.stack[(inter->stack.top)++] = *value;
}

ZAL_Value zal_stack_pop(ZAL_Interpreter* inter){
    DBG_assert(inter->stack.top>0, (NULL));
    ZAL_Value ret=inter->stack.stack[--(inter->stack.top)];
    return ret;
}

void zal_stack_shrink(ZAL_Interpreter* inter, int size){
    DBG_assert(inter->stack.top-size>=0, (NULL));
    inter->stack.top -= size;
}
// 查看栈内容, 不操作
ZAL_Value * zal_stack_peek(ZAL_Interpreter* inter, int mov){
    DBG_assert(inter->stack.top-mov-1>=0, (NULL));
    return &(inter->stack.stack[inter->stack.top-mov-1]);
}
