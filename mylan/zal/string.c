// 辅助解析时生成字面常量
#include "zal_in.h"
#include <string.h>

#define STR_ALLOC_SIZE  (256)

typedef struct{
    int     size;
    int     alloc_size;
    char*   buf;
}StringLiteral;

static StringLiteral s_str_literal={0,0,NULL};

void zal_open_str_literal(void){
    s_str_literal.size = 0;
}

void zal_cat_str_literal(char ch){
    if(s_str_literal.size==s_str_literal.alloc_size){
        s_str_literal.alloc_size += STR_ALLOC_SIZE;
        s_str_literal.buf = MEM_realloc(s_str_literal.buf, s_str_literal.alloc_size);
    }
    s_str_literal.buf[(s_str_literal.size)++] = ch;
}

void zal_reset_str_literal_buf(void){
    MEM_free(s_str_literal.buf);
    s_str_literal.buf = NULL;
    s_str_literal.size = 0;
    s_str_literal.alloc_size = 0;
}

char* zal_close_str_literal(void){
    char* ret = zal_alloc(s_str_literal.size+1);
    memcpy(ret, s_str_literal.buf, s_str_literal.size); // 最后不是'\0', 别用strcpy
    ret[s_str_literal.size] = '\0';
    return ret;
}
