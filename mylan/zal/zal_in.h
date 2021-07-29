#ifndef __ZAL_IN_H__
#define __ZAL_IN_H__
#include "ZAL.h"
#include "MEM.h"

typedef enum{
    ZAL_BOOL_VALUE = 1,
    ZAL_INT_VALUE,
    ZAL_DOUBLE_VALUE,
    ZAL_STRING_VALUE,
    ZAL_ARRAY_VALUE,
    ZAL_NATIVE_POINTER_VALUE,
    ZAL_NULL_VALUE
}ZAL_ValueType;

typedef enum{
    STRING_OBJ = 1,
    ARRAY_OBJ
}ObjectType;

typedef enum{
    ZAL_FUNC_DEF = 1,
    NATIVE_FUNC_DEF
}FuncDefType;

typedef enum{
    BOOL_EXPRESSION = 1,                        /* 常量begin */
    INT_EXPRESSION,
    DOUBLE_EXPRESSION,
    NULL_EXPRESSION,
    STRING_EXPRESSION,
    ARRAY_EXPRESSION,                           /* 常量end */
    INDEX_EXPRESSION,                           /* 变量begin */
    IDENTIFIER_EXPRESSION,                      /* 变量end */
    ASSIGN_EXPRESSION,              // 赋值
    ADD_EXPRESSION,                             /* 双目begin */
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    EQ_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,                              /* 双目end */
    MINUS_EXPRESSION,                           
    INCREMENT_EXPRESSION,
    DECREMENT_EXPRESSION,
    FUNC_CALL_EXPRESSION,
    METHOD_CALL_EXPRESSION
}ExpressionType;

typedef enum{
    EXPRESSION_STAT = 1,
    IF_STAT,
    FOR_STAT,
    WHILE_STAT,
    BREAK_STAT,
    CONTINUE_STAT,
    RETURN_STAT
}StatementType;

typedef enum{
    a
}ErrorType;


typedef ZAL_String_tag              ZAL_String;
typedef ZAL_Array_tag               ZAL_Array;
typedef ZAL_Object_tag              ZAL_Object;
typedef Stack_tag                   Stack;
typedef ZAL_NativePointer_tag       ZAL_NativePointer;
typedef ZAL_Value_tag               ZAL_Value;
typedef VariableList_tag            VariableList;
typedef FuncDefList_tag             FuncDefList;
typedef ParameterList_tag           ParameterList;
typedef ZAL_LocalEnvironment_tag    ZAL_LocalEnvironment;
typedef Expression_tag              Expression;
typedef AssignExpression_tag        AssignExpression;
typedef BinaryExpression_tag        BinaryExpression;
typedef FuncCallExpression_tag      FuncCallExpression;
typedef MethodCallExpression_tag    MethodCallExpression;
typedef ArgumentList_tag            ArgumentList;
typedef Statement_tag               Statement;
typedef StatementList_tag           StatementList;
typedef StatementList               Block;
typedef IfStatement_tag             IfStatement;
typedef ForStatement_tag            ForStatement;
typedef WhileStatement_tag          WhileStatement;
typedef ReturnStatement_tag         ReturnStatement;


/********** 变量 ***********/

// TODO
// 无类型变量类型(就是一种类型, 和Variable比更抽象)
struct ZAL_Value_tag{
    ZAL_ValueType           type;
    union{
        ZAL_Boolean         bool_vaule;
        int                 int_value;
        double              double_value;
        ZAL_Object          *object;        // 对象(str/arr)
        ZAL_NativePointer   pointer;
    }u;
};

// 具体变量链表(更容易增减变量)
struct VariableList_tag{
    char            *name;
    ZAL_Value       value;
    VariableList    *next;
};

struct ZAL_String_tag{
    ZAL_Boolean is_literal;
    char* string;
};

// 数组
struct ZAL_Array_tag{
    int         size;
    int         alloc_size;
    ZAL_Value   *array;
};

// TODO
// 对象
struct ZAL_Object_tag{
    ObjectType      type;
    union{
        ZAL_String  *string;
        ZAL_Array   *array;
    }u;
};

// 原生指针
struct ZAL_NativePointer_tag{
    char * info;
    void * pointer; 
};

struct FuncDefList_tag{
    char            *name;
    FuncDefType     type;
    union{
        struct{
            ParameterList   *para;
            Block           *block;
        }   zal_f;
        struct{
            NativeFuncProc  *proc;
        }   native_f;
    }u;
    FuncDefList     *next;
};

// 形参列表(无类型, 所以不同类型传入不一定有事, 只要函数内操作对传入变量合法即可)
struct ParameterList_tag{
    char            *name;
    ArgumentList    *next;
};


struct ZAL_LocalEnvironment_tag{
    VariableList    *local_variable;
    VariableList    *global_variable;
};

/************ 表达式 ************/

struct Expression_tag{
    ExpressionType              type;
    union{
        ZAL_Boolean             bool_expr;
        int                     int_expr;
        double                  double_expr;
        char                    *string_expr;
        char                    *identifier_expr;
        AssignExpression        *assign_expr;
        BinaryExpression        *Binary_expr;
        Expression              *minus_expr;
        Expression              *inc_dec_expr;
        FuncCallExpression      *func_call_expr;
        MethodCallExpression    *method_call_expr;
    }u;
};

struct AssignExpression_tag{
    Expression      *l_value;
    Expression      *r_value;
};

struct BinaryExpression_tag{
    Expression      *left;
    Expression      *right;
};

struct FuncCallExpression_tag{
    char            *identifier;
    ArgumentList    *argv;
};

struct MethodCallExpression_tag{
    Expression      *expr;          // .左边的?
    char            *identifier;
    ArgumentList    *argv;
};

struct ArgumentList_tag{
    Expression      *argv;
    ArgumentList    *next;
};


/************ 句子 ************/
struct Statement_tag{
    StatementType   type;
    union{
        Expression              *expression_s;
        IfStatement             *if_s;
        ForStatement            *for_s;
        WhileStatement          *while_s;
        ReturnStatement         *return_s;
    }u;
};

struct StatementList_tag{
    Statement       *statement;
    StatementList   *next;
};

struct IfStatement_tag{
    Expression  *condition;     // if
    Block       *then_block;
    Elif        *elif;
    Block       *else_block;
};

typedef struct Elif_tag{
    Expression          *condition;
    Block               *block;
    struct Elif_tag     *next;
}Elif;

struct ForStatement_tag{
    Expression  *init;
    Expression  *condition;
    Expression  *post;
    Block       *block;
};

struct WhileStatement_tag{
    Expression  *condition;
    Block       *block;
};

struct ReturnStatement_tag{
    Expression  *result;
};

/************ 解释器 ************/
// TODO
// 解释器
struct ZAL_Interpreter_tag{
    MEM_Storage         inter_storage;
    MEM_Storage         exe_storage;
    FuncDefList         *func_list;
    VariableList        *g_varible;
    StatementList       *state_list;
    Stack               stack;
    Heap                heap;
};


/************stack.c***************/    // TODO

/*  |xxx|
 *  |xxx|
 *  |   | <-- top
 */
// 栈
struct Stack_tag{
    int         size;
    int         top;
    ZAL_Value   *stack;
};

void stack_init(ZAL_Interpreter* inter, int size);
void stack_push(ZAL_Interpreter* inter, ZAL_Value *value);
ZAL_Value * stack_pop(ZAL_Interpreter* inter);
void stack_shrink(ZAL_Interpreter* inter, int size);
ZAL_Value * stack_peek(ZAL_Interpreter* inter, int mov);

#endif
