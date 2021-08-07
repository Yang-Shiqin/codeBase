#ifndef __ZAL_H__
#define __ZAL_H__

#include <stdio.h>

// 内部不可见解释器
typedef struct ZAL_Interpreter_tag ZAL_Interpreter;



typedef enum{
    ZAL_FALSE,
    ZAL_TRUE
}ZAL_Boolean;


/**** interface.c ****/

ZAL_Interpreter *ZAL_create_interpreter(void);

void ZAL_compile(ZAL_Interpreter *inter, FILE *fp);

void ZAL_interpret(ZAL_Interpreter *inter);

void ZAL_destroy_interpreter(ZAL_Interpreter *inter);

#endif
