#include "zal_in.h"


void stack_init(ZAL_Interpreter* inter, int size){
    inter->stack.stack = MEM_alloc(size);
    inter->stack.size = size;
    inter->stack.top = 0;

}


void stack_push(ZAL_Interpreter* inter, ZAL_Value *value){

}

ZAL_Value * stack_pop(ZAL_Interpreter* inter){

}

void stack_shrink(ZAL_Interpreter* inter, int size){

}

ZAL_Value * stack_peek(ZAL_Interpreter* inter, int mov){

}
