#ifndef __ZAL_H__
#define __ZAL_H__
#define "zal_in.h"

// 内部不可见解释器
typedef struct ZAL_Interpreter_tag ZAL_Interpreter;

// 内置函数定义函数
typedef ZAL_Value (*NativeFuncProc)(ZAL_Interpreter* inter,\
    ZAL_LocalEnvironment *env, int argc, ZAL_Value* argv);


typedef enum{
    ZAL_FALSE,
    ZAL_TRUE
}ZAL_Boolean;




ZAL_Interpreter *ZAL_create_interpreter(void);

void ZAL_compile(ZAL_Interpreter *inter, FILE *fp);

void ZAL_interpret(ZAL_Interpreter *inter);

void ZAL_destroy_interpreter(ZAL_Interpreter *inter);

#endif
