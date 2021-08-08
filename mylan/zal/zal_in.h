#ifndef __ZAL_IN_H__
#define __ZAL_IN_H__
#include "ZAL.h"
#include "MEM.h"
#include "DBG.h"

#define LINE_BUF_SIZE       (1024)
#define STACK_ALLOC_SIZE    (256)
#define HEAP_THRESHOLD      (1024*256)  // 堆gc阈值

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
    NE_EXPRESSION,
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
    NORMAL_STAT_RESULT = 1,
    BREAK_STAT_RESULT,
    CONTINUE_STAT_RESULT,
    RETURN_STAT_RESULT
}StatementResultType;

typedef enum{
    PARSE_ERR = 1,
    CHARACTER_INVALID_ERR,
    FUNC_MUT_DEF_ERR, 
    COMPILE_ERR_INDEX_PLUS_ONE
}CompileErrorType;  // 可能也不能叫编译, 应该是分析

typedef enum{
    FUNCTION_NOT_FOUND_ERR = 1,
    ARGUMENT_TOO_MANY_ERR,
    ARGUMENT_TOO_FEW_ERR,
    NOT_BOOL_TYPE_ERR,
    MINUS_OPERAND_TYPE_ERR,
    BINARY_OPERAND_TYPE_ERR,
    FOPEN_ARG_TYPE_ERR,
    FCLOSE_ARG_TYPE_ERR,
    FGETS_ARG_TYPE_ERR,
    FPUTS_ARG_TYPE_ERR,
    NOT_NULL_OPERATOR_ERR,
    DIVISION_BY_ZERO_ERR,
    NOT_STRING_OPERATOR_ERR,
    NOT_LVALUE_ERR,
    INDEX_OPERAND_NOT_ARRAY_ERR,
    INDEX_OPERAND_NOT_INT_ERR,
    ARRAY_INDEX_OUT_OF_BOUNDS_ERR,
    NEW_ARRAY_ARG_TYPE_ERR,
    INC_DEC_OPERAND_TYPE_ERR,
    RESIZE_ARG_TYPE_ERR,
    RUNTIME_ERR_INDEX_PLUS_ONE
}RuntimeErrorType;

typedef struct{
    char    *format;
}ErrorMsgFormat;

// str外包一层, free掉还有
typedef struct{
    char    *string;
}VStr;

typedef struct ZAL_String_tag              ZAL_String;
typedef struct ZAL_Array_tag               ZAL_Array;
typedef struct ZAL_Object_tag              ZAL_Object;
typedef struct Stack_tag                   Stack;
typedef struct ZAL_NativePointer_tag       ZAL_NativePointer;
typedef struct ZAL_Value_tag               ZAL_Value;
typedef struct VariableList_tag            VariableList;
typedef struct FuncDefList_tag             FuncDefList;
typedef struct ParameterList_tag           ParameterList;
typedef struct ZAL_LocalEnvironment_tag    ZAL_LocalEnvironment;
typedef struct Expression_tag              Expression;
typedef struct ExpressionList_tag          ExpressionList;
typedef struct AssignExpression_tag        AssignExpression;
typedef struct BinaryExpression_tag        BinaryExpression;
typedef struct FuncCallExpression_tag      FuncCallExpression;
typedef struct MethodCallExpression_tag    MethodCallExpression;
typedef struct IndexExpression_tag         IndexExpression;
typedef struct ArgumentList_tag            ArgumentList;
typedef struct Statement_tag               Statement;
typedef struct StatementList_tag           StatementList;
typedef struct StatementResult_tag         StatementResult;
typedef struct Block_tag                   Block;
typedef struct IfStatement_tag             IfStatement;
typedef struct ForStatement_tag            ForStatement;
typedef struct WhileStatement_tag          WhileStatement;
typedef struct ReturnStatement_tag         ReturnStatement;
typedef struct Heap_tag                    Heap;

/********** 变量 ***********/

typedef struct{
    char *info;
}ZAL_NativePointerInfo;


// 原生指针
struct ZAL_NativePointer_tag{
    ZAL_NativePointerInfo   *info;
    void                    *pointer; 
};


// TODO
// 无类型变量类型(就是一种类型, 和Variable比更抽象)
struct ZAL_Value_tag{
    ZAL_ValueType           type;
    union{
        ZAL_Boolean         bool_value;
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
    int             marked;     // gc
    union{
        ZAL_String  string;
        ZAL_Array   array;
    }u;
    ZAL_Object      *prev;      // 以链表形式存堆里, 用以回收
    ZAL_Object      *next;
};


// 内置函数定义函数
typedef ZAL_Value (* ZAL_NativeFuncProc) (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);


struct FuncDefList_tag{
    char            *name;
    FuncDefType     type;
    union{
        struct{
            ParameterList   *para;
            Block           *block;
        }   zal_f;
        struct{
            ZAL_NativeFuncProc  proc;
        }   native_f;
    }u;
    FuncDefList     *next;
};

// 形参列表(无类型, 所以不同类型传入不一定有事, 只要函数内操作对传入变量合法即可)
struct ParameterList_tag{
    char            *name;
    ParameterList   *next;
};


struct ZAL_LocalEnvironment_tag{
    VariableList            *local_variable;
    ZAL_LocalEnvironment    *next;
};

/************ 表达式 ************/
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
    Expression      *expr;          // .左边的
    char            *identifier;
    ArgumentList    *argv;
};

struct ArgumentList_tag{
    Expression      *argv;
    ArgumentList    *next;
};

struct IndexExpression_tag{
    Expression      *array;     // 数组名
    Expression      *index;     // 数组下标
};

struct Expression_tag{
    ExpressionType              type;
    int                         line_number;    // 新语言脚本中句子行号(报错提示用)
    union{
        ZAL_Boolean             bool_expr;
        int                     int_expr;
        double                  double_expr;
        char                    *string_expr;
        char                    *identifier_expr;
        AssignExpression        assign_expr;
        BinaryExpression        binary_expr;
        Expression              *minus_expr;
        Expression              *inc_dec_expr;
        FuncCallExpression      func_call_expr;
        MethodCallExpression    method_call_expr;
        IndexExpression         index_expr;         // 数组元素
        ExpressionList          *array_liter_expr;  // 字面量数组
    }u;
};

struct ExpressionList_tag{
    Expression      *expr;
    ExpressionList  *next;
};



/************ 句子 ************/
struct StatementList_tag{
    Statement       *statement;
    StatementList   *next;
};


struct Block_tag{
    StatementList   *state_list;
};


typedef struct Elif_tag{
    Expression          *condition;
    Block               *block;
    struct Elif_tag     *next;
}Elif;


struct IfStatement_tag{
    Expression  *condition;     // if
    Block       *then_block;
    Elif        *elif;
    Block       *else_block;
};


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

struct Statement_tag{
    StatementType               type;
    int                         line_number;    // 新语言脚本中句子行号(报错提示用)
    union{
        Expression              *expression_s;
        IfStatement             if_s;
        ForStatement            for_s;
        WhileStatement          while_s;
        ReturnStatement         return_s;
    }u;
};

struct StatementResult_tag{
    StatementResultType         type;
    union{
        ZAL_Value               result_v;
    }u;
};


/************ stack.c ***************/ 

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

void zal_stack_init(ZAL_Interpreter* inter);
void zal_stack_push(ZAL_Interpreter* inter, ZAL_Value *value);
ZAL_Value zal_stack_pop(ZAL_Interpreter* inter);
void zal_stack_shrink(ZAL_Interpreter* inter, int size);
ZAL_Value * zal_stack_peek(ZAL_Interpreter* inter, int mov);

/************ heap.c ***************/    // TODO
struct Heap_tag{
    int         size;
    int         threshold;
    ZAL_Object  *heap;
};

ZAL_Object* zal_literal_to_string(ZAL_Interpreter *inter, char *str);
ZAL_Object* zal_non_literal_to_string(ZAL_Interpreter *inter, char *str);
ZAL_Object* zal_create_array_obj(ZAL_Interpreter *inter, int size);


void zal_mark_sweep_gc(ZAL_Interpreter* inter);

/************ create.c(给语法解析器用) ***************/
Expression* zal_alloc_expr(ExpressionType type);
Expression* zal_create_identifier_expr(char *identifier);
Expression* zal_create_bool_expr(ZAL_Boolean value);
Expression* zal_create_null_expr(void);
Expression* zal_create_assign_expr(Expression *l_value, Expression *r_value);
Expression* zal_create_binary_expr(ExpressionType operator, Expression *left, Expression *right);
Expression* zal_create_minus_expr(Expression *expr);
Expression* zal_create_inc_dec_expr(Expression *expr, ExpressionType type);
Expression* zal_create_func_call_expr(char *identifier, ArgumentList *argv);
Expression* zal_create_method_call_expr(Expression *expr, char *identifier, ArgumentList *argv);
Expression* zal_create_array_expr(ExpressionList *elem_list);
Expression* zal_create_index_expr(Expression *array, Expression *index);
ExpressionList* zal_create_expr_list(Expression* expr);
ExpressionList* zal_add_expr_to_list(ExpressionList* expr_list, Expression* expr);

ArgumentList* zal_create_argument_list(Expression *expr);
ArgumentList* zal_add_argument_to_list(ArgumentList* arg_list, Expression *expr);
ParameterList* zal_create_parameter_list(char *name);
ParameterList* zal_add_parameter_to_list(ParameterList* para_list, char *name);

Statement* zal_alloc_stat(StatementType type);
Statement* zal_create_expr_statement(Expression* expr);
Statement* zal_create_if_statement(Expression *condition, Block *then_block, Elif *elif, Block *else_block);
Elif* zal_create_elif(Expression *condition, Block *block);
Elif* zal_add_elif_to_list(Elif *elif, Expression *condition, Block *block);
Statement* zal_create_for_statement(Expression *init, Expression *condition, Expression *post, Block *block);
Statement* zal_create_while_statement(Expression *condition, Block *block);
Statement* zal_create_return_statement(Expression *result);
Statement* zal_create_break_statement(void);
Statement* zal_create_continue_statement(void);
StatementList* zal_create_statement_list(Statement* state);
StatementList* zal_add_statement_to_list(StatementList* state_list, Statement* state);

Block* zal_create_block(StatementList* state_list);

void zal_inter_create_function(char* identifier, ParameterList *para, Block *block);

char* zal_create_identifier(char *str);


/************ eval.c ***************/    // TODO: 很多函数都不完整
ZAL_Value zal_eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
// 用以create里常量折叠
ZAL_Value zal_eval_binary_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right);
ZAL_Value zal_eval_minus_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);

/************ execute.c ***************/    // TODO
StatementResult zal_exe_stat_list(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, StatementList *list);

/************ util.c ***************/    // TODO

ZAL_Interpreter* zal_get_current_inter(void);
void zal_set_current_inter(ZAL_Interpreter* inter);

void* zal_alloc(size_t size);

FuncDefList *zal_search_function(char *indentifier);
VariableList *zal_search_local_variable(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, char *identifier);
VariableList *zal_search_global_variable(ZAL_Interpreter *inter, char *identifier);
VariableList *zal_add_global_variable(ZAL_Interpreter *inter, char *identifier);
VariableList *zal_add_local_variable(ZAL_LocalEnvironment *env, char *identifier);

int zal_is_true(ZAL_Value *value);

void zal_vstr_append_ch(VStr *string, char ch);
void zal_vstr_append_string(VStr *string, char *add_str);
char *zal_value_to_str(ZAL_Value *value);
char *zal_operator_to_str(ExpressionType type);
char *zal_value_type_to_str(ZAL_ValueType type);

/************ string.c(保存解析时字符串字面常量) ***************/
void zal_open_str_literal(void);
void zal_cat_str_literal(char ch);      // 解析时一个字符一个字符解析
void zal_reset_str_literal_buf(void);
char* zal_close_str_literal(void);

/************ interface.c ***************/    // TODO
void ZAL_add_native_function(ZAL_Interpreter *inter, char *identifier, ZAL_NativeFuncProc func);



/************ native.c ***************/    // TODO
// ZAL_NativeFuncProc的函数形式
ZAL_Value zal_nv_print  (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_scan   (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_fopen  (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_fclose (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_fgets  (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_fputs  (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
ZAL_Value zal_nv_array  (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
void zal_add_std_fp(ZAL_Interpreter* inter);

/************ error.c ***************/    // TODO
void zal_compile_error(CompileErrorType ceid, ...);     // ...为报错信息
void zal_runtime_error(int line, RuntimeErrorType reid, ...);

/************ 解释器 ************/
// TODO
// 解释器
struct ZAL_Interpreter_tag{
    MEM_Storage             *inter_storage;
    MEM_Storage             *exe_storage;
    int                     line_number;
    FuncDefList             *func_list;
    VariableList            *g_varible;     // (根)
    StatementList           *state_list;
    Stack                   stack;          // (根)
    Heap                    heap;
    ZAL_LocalEnvironment    *last_env;      // 栈的调用层次(根)
};


#endif
