#include "zal_in.h"


ZAL_Interpreter *s_current_inter;



ZAL_Interpreter* zal_get_current_inter(void){
    return s_current_inter;
}

void zal_set_current_inter(ZAL_Interpreter* inter){
    s_current_inter = inter;
}

// 语法分析时在解释器alloc一块空间
void* zal_alloc(size_t size){
    /* TODO */
}