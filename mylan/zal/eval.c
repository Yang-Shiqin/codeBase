#include <math.h>
#include <string.h>
#include "zal_in.h"

#define my_is_digit(x) (((x)->type==ZAL_INT_VALUE)||((x)->type==ZAL_DOUBLE_VALUE))

static void eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static void eval_bool_expr(ZAL_Interpreter *inter, ZAL_Boolean bool_expr);
static void eval_int_expr(ZAL_Interpreter *inter, int int_expr);
static void eval_double_expr(ZAL_Interpreter *inter, double double_expr);
static void eval_null_expr(ZAL_Interpreter *inter);
static void eval_string_expr(ZAL_Interpreter *inter, char *string_expr);
static void eval_array_expr(ZAL_Interpreter *inter, ExpressionList *array_liter_expr);
static void eval_index_expr(ZAL_Interpreter *inter, Expression *array, Expression *index);
static void eval_identifier_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static void eval_assign_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *l_value, Expression *r_value);
static void eval_binary_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right);
static void eval_logical_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right);
static void eval_minus_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static void eval_inc_dec_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *inc_dec_expr);
static void eval_func_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static void eval_method_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);

static void eval_eq_ne_expr         (ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right);
static void eval_binary_int_expr    (ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right);
static void eval_binary_double_expr (ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right);
static void eval_binary_string_expr (ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right);
static void eval_binary_array_expr  (ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right);

static ZAL_LocalEnvironment *alloc_local_env(ZAL_Interpreter *inter);
static void destroy_local_env(ZAL_Interpreter *inter);
static void call_zal_func(ZAL_Interpreter *inter, ZAL_LocalEnvironment *caller_env, ZAL_LocalEnvironment *called_env, Expression *expr, FuncDefList *func);
static void call_native_func(ZAL_Interpreter *inter, ZAL_LocalEnvironment *caller_env, ZAL_LocalEnvironment *called_env, Expression *expr, FuncDefList *func);

static void covert_bool_to_int(ZAL_Value *value);
static ZAL_Value *get_lvalue(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static ZAL_Value *get_array_lvalue(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);
static ZAL_Boolean compare_array(ZAL_Interpreter *inter, ZAL_Value *left, ZAL_Value *right);

/*********************************** 定义区 *******************************************/
ZAL_Value zal_eval_binary_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right){
    eval_binary_expr(inter, env, type, left, right);
    return zal_stack_pop(inter);
}
ZAL_Value zal_eval_minus_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    eval_minus_expr(inter, env, expr);
    return zal_stack_pop(inter);
}
ZAL_Value zal_eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    eval_expr(inter, env, expr);
    return zal_stack_pop(inter);
}


static void eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){ 
    if(!expr){
        ZAL_Value null;
        null.type = ZAL_NULL_VALUE;
        zal_stack_push(inter, &null);
        return;
    } 
    switch (expr->type)
    {
    case BOOL_EXPRESSION:
        eval_bool_expr(inter, expr->u.bool_expr);
        break;
    case INT_EXPRESSION:
        eval_int_expr(inter, expr->u.int_expr);
        break;
    case DOUBLE_EXPRESSION:
        eval_double_expr(inter, expr->u.double_expr);
        break;
    case NULL_EXPRESSION:
        eval_null_expr(inter);
        break;
    case STRING_EXPRESSION:
        eval_string_expr(inter, expr->u.string_expr);
        break;
    case ARRAY_EXPRESSION:
        eval_array_expr(inter, expr->u.array_liter_expr);
        break;
    case INDEX_EXPRESSION:
        eval_index_expr(inter, expr->u.index_expr.array, expr->u.index_expr.index);
        break;
    case IDENTIFIER_EXPRESSION:
        eval_identifier_expr(inter, env, expr); // 需报错信息, 就传了整个
        break;
    case ASSIGN_EXPRESSION:
        eval_assign_expr(inter, env, expr->u.assign_expr.l_value, expr->u.assign_expr.r_value);
        break;
    case ADD_EXPRESSION:
    case SUB_EXPRESSION:
    case MUL_EXPRESSION:
    case DIV_EXPRESSION:
    case MOD_EXPRESSION:
    case EQ_EXPRESSION:
    case NE_EXPRESSION:
    case GT_EXPRESSION:
    case GE_EXPRESSION:
    case LT_EXPRESSION:
    case LE_EXPRESSION:
        eval_binary_expr(inter, env, expr->type, expr->u.binary_expr.left, expr->u.binary_expr.right);
        break;
    case LOGICAL_AND_EXPRESSION:
    case LOGICAL_OR_EXPRESSION:
        eval_logical_expr(inter, env, expr->type, expr->u.binary_expr.left, expr->u.binary_expr.right);
        break;
    case MINUS_EXPRESSION:
        eval_minus_expr(inter, env, expr->u.minus_expr);
        break;
    case INCREMENT_EXPRESSION:
    case DECREMENT_EXPRESSION:
        eval_inc_dec_expr(inter, env, expr->type, expr->u.inc_dec_expr);
        break;
    case FUNC_CALL_EXPRESSION:
        eval_func_call_expr(inter, env, expr);
        break;
    case METHOD_CALL_EXPRESSION:
        eval_method_call_expr(inter, env, expr);
        break;
    default:
        /* TODO: error */
        break;
    }
}


static void eval_bool_expr(ZAL_Interpreter *inter, ZAL_Boolean bool_expr){
    ZAL_Value ret;
    ret.type = ZAL_BOOL_VALUE;
    ret.u.bool_value = bool_expr;
    zal_stack_push(inter, &ret);
}
static void eval_int_expr(ZAL_Interpreter *inter, int int_expr){
    ZAL_Value ret;
    ret.type = ZAL_INT_VALUE;
    ret.u.int_value = int_expr;
    zal_stack_push(inter, &ret);
}
static void eval_double_expr(ZAL_Interpreter *inter, double double_expr){
    ZAL_Value ret;
    ret.type = ZAL_DOUBLE_VALUE;
    ret.u.double_value = double_expr;
    zal_stack_push(inter, &ret);
}
static void eval_null_expr(ZAL_Interpreter *inter){
    ZAL_Value ret;
    ret.type = ZAL_NULL_VALUE;
    zal_stack_push(inter, &ret);
}

/* TODO */
static void eval_string_expr(ZAL_Interpreter *inter, char *string_expr){
    ZAL_Value ret;
    ret.type = ZAL_STRING_VALUE;
    ret.u.object = zal_literal_to_string(inter, string_expr);
    zal_stack_push(inter, &ret);
}

/* TODO */
static void eval_array_expr(ZAL_Interpreter *inter, ExpressionList *array_liter_expr){
    ZAL_Value ret;
    ret.type = ZAL_ARRAY_VALUE;
    zal_stack_push(inter, &ret);
}

/* TODO */
static void eval_index_expr(ZAL_Interpreter *inter, Expression *array, Expression *index){
    ZAL_Value ret;
    zal_stack_push(inter, &ret);
}
static void eval_identifier_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    VariableList *var=NULL;
    ZAL_Value ret;
    if(env==NULL){  // 顶层结构
        var = zal_search_global_variable(inter, expr->u.identifier_expr);
        if(var==NULL){
            var = zal_add_global_variable(inter, expr->u.identifier_expr);
        }
    }else{
        var = zal_search_local_variable(inter, env, expr->u.identifier_expr);
        if(var==NULL){
            var = zal_add_local_variable(env, expr->u.identifier_expr);
        }
    }
    ret = var->value;
    zal_stack_push(inter, &ret);
}

static void eval_assign_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *l_value, Expression *r_value){
    ZAL_Value *ret;
    ZAL_Value *dest = get_lvalue(inter, env, l_value);

    eval_expr(inter, env, r_value);
    ret = zal_stack_peek(inter, 0);

    *dest = *ret;
}


/* TODO: 更完善的类型转换 */
// int->double->string, x->string
static void eval_binary_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right){
    ZAL_Value l_v, r_v;
    eval_expr(inter, env, left);
    eval_expr(inter, env, right);
    r_v = zal_stack_pop(inter);
    l_v = zal_stack_pop(inter);
    covert_bool_to_int(&r_v);
    covert_bool_to_int(&l_v);
    if((type==EQ_EXPRESSION) || (type==NE_EXPRESSION)){ // 返回bool
        eval_eq_ne_expr(inter, type, &l_v, &r_v);
    }else if(r_v.type==ZAL_INT_VALUE && l_v.type==ZAL_INT_VALUE){
        eval_binary_int_expr(inter, type, &l_v, &r_v);
    }else if(my_is_digit(&l_v)&&(r_v.type==ZAL_DOUBLE_VALUE) || my_is_digit(&r_v)&&(l_v.type==ZAL_DOUBLE_VALUE)){
        eval_binary_double_expr(inter, type, &l_v, &r_v);
    }else if((l_v.type==ZAL_STRING_VALUE) || (r_v.type==ZAL_STRING_VALUE)){
        eval_binary_string_expr(inter, type, &l_v, &r_v);
    }else if((l_v.type==ZAL_ARRAY_VALUE) && (r_v.type==ZAL_ARRAY_VALUE)){
        eval_binary_array_expr(inter, type, &l_v, &r_v);
    }else{
        /* TODO: error */
    }

}

// TODO: 只有bool能&& ||
static void eval_logical_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right){
    ZAL_Value ret, l_v, r_v;
    eval_expr(inter, env, left);
    eval_expr(inter, env, right);
    r_v = zal_stack_pop(inter);
    l_v = zal_stack_pop(inter);
    if(r_v.type!=ZAL_BOOL_VALUE || r_v.type!=ZAL_BOOL_VALUE){
        /* TODO: error */
    }
    ret.type = ZAL_BOOL_VALUE;
    if(type == LOGICAL_AND_EXPRESSION){
        ret.u.bool_value = l_v.u.bool_value && r_v.u.bool_value;
    }else{
        ret.u.bool_value = l_v.u.bool_value || r_v.u.bool_value;
    }
    zal_stack_push(inter, &ret);
}

/* TODO */
static void eval_minus_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    ZAL_Value value;
    value = zal_eval_expr(inter, env, expr);
    int tmp;
    switch (value.type)
    {
    case ZAL_BOOL_VALUE:
        tmp = value.u.bool_value;
        value.type = ZAL_INT_VALUE;
        value.u.int_value = -tmp;
        break;
    case ZAL_INT_VALUE:
        value.u.int_value = -value.u.int_value;
        break;
    case ZAL_DOUBLE_VALUE:
        value.u.double_value = -value.u.double_value;
        break;
    default:
        /* TODO: error */
        break;
    }
    zal_stack_push(inter, &value);
}

static void eval_inc_dec_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *inc_dec_expr){
    ZAL_Value *ret = get_lvalue(inter, env, inc_dec_expr);
    if(ret->type != ZAL_INT_VALUE){
        /* TODO: error */
    }
    if(type==INCREMENT_EXPRESSION){
        ret->u.int_value++;
    }else{
        ret->u.int_value--;
    }
    zal_stack_push(inter, ret);
}

/* TODO: error */
static void eval_func_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    FuncDefList *func = zal_search_function(expr->u.func_call_expr.identifier);
    ZAL_LocalEnvironment *sub_env = alloc_local_env(inter);
    if(func==NULL){
        // zal_runtime_error(); // TODO: 找不到函数
    }
    if(func->type == ZAL_FUNC_DEF){
        call_zal_func(inter, env, sub_env, expr, func);
    }else{
        // DBG_assert(func->type==NATIVE_FUNC_DEF);
        call_native_func(inter, env, sub_env, expr, func);
    }
    destroy_local_env(inter);
}

/* TODO */
static void eval_method_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    ZAL_Value ret;
    zal_stack_push(inter, &ret);
}


/***************************** eval_binary_expr *********************************/

static void eval_eq_ne_expr(ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right){
    ZAL_Value ret;
    ret.type = ZAL_BOOL_VALUE;
    covert_bool_to_int(left);
    covert_bool_to_int(right);
    if(my_is_digit(left) || my_is_digit(right)){
        if(my_is_digit(left) && my_is_digit(right)){
            double l_v, r_v;
            // TODO: 写成宏?
            if(left->type==ZAL_INT_VALUE) l_v = left->u.int_value;
            else l_v = left->u.double_value;
            if(right->type==ZAL_INT_VALUE) r_v = right->u.int_value;
            else r_v = right->u.double_value;
            ret.u.bool_value = (fabs(l_v-r_v)<=1e-6);
        }else{
            ret.u.bool_value = ZAL_FALSE;
        }
    }else{
        if(left->type==right->type){
            switch (left->type)
            {
            case ZAL_STRING_VALUE:
                ret.u.bool_value = !strcmp(left->u.object->u.string.string, right->u.object->u.string.string);
                break;
            case ZAL_ARRAY_VALUE:
                ret.u.bool_value = compare_array(inter, left, right);
                break;
            case ZAL_NATIVE_POINTER_VALUE:
                ret.u.bool_value = left->u.pointer.pointer==right->u.pointer.pointer;
                break;
            case ZAL_NULL_VALUE:
                ret.u.bool_value = ZAL_TRUE;
                break;
            default:
                /* TODO: error */
                break;
            }
        }else{
            ret.u.bool_value = ZAL_FALSE;
        }
    }
    if(type==NE_EXPRESSION) ret.u.bool_value = !ret.u.bool_value;
    zal_stack_push(inter, &ret);
}
// TODO: 补充c语言溢出不报错
static void eval_binary_int_expr(ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right){
    // DBG_assert(left->type==ZAL_INT_VALUE);
    // DBG_assert(right->type==ZAL_INT_VALUE);
    ZAL_Value ret;
    ret.type = ZAL_INT_VALUE;
    int l, r;
    l = left->u.int_value;
    r = right->u.int_value;
    switch (type)
    {
    case ADD_EXPRESSION:
        ret.u.int_value = l+r;
        break;    
    case SUB_EXPRESSION:
        ret.u.int_value = l-r;
        break;
    case MUL_EXPRESSION:
        ret.u.int_value = l*r;
        break;
    case DIV_EXPRESSION:
        ret.u.int_value = l/r;  // 除0 c会报错?
        break;
    case MOD_EXPRESSION:
        ret.u.int_value = l%r;
        break;
    case EQ_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l==r);
        break;
    case NE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l!=r);
        break;
    case GT_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l>r);
        break;
    case GE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l>=r);
        break;
    case LT_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l<r);
        break;
    case LE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l<=r);
        break;
    default:
        /* TODO: error */
        break;
    }
    zal_stack_push(inter, &ret);
}
static void eval_binary_double_expr(ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right){
    // DBG_assert(my_is_digit(left));
    // DBG_assert(my_is_digit(right));
    ZAL_Value ret;
    ret.type = ZAL_DOUBLE_VALUE;
    double l, r;
    // TODO: 写成宏?
    if(left->type==ZAL_INT_VALUE) l = left->u.int_value;
    else l = left->u.double_value;
    if(right->type==ZAL_INT_VALUE) r = right->u.int_value;
    else r = right->u.double_value;
    switch (type)
    {
    case ADD_EXPRESSION:
        ret.u.double_value = l+r;
        break;    
    case SUB_EXPRESSION:
        ret.u.double_value = l-r;
        break;
    case MUL_EXPRESSION:
        ret.u.double_value = l*r;
        break;
    case DIV_EXPRESSION:
        ret.u.double_value = l/r;  // 除0 c会报错?
        break;
    case MOD_EXPRESSION:
        /* TODO: error: mod只能用于int */
    case EQ_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l==r);
        break;
    case NE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l!=r);
        break;
    case GT_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l>r);
        break;
    case GE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l>=r);
        break;
    case LT_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l<r);
        break;
    case LE_EXPRESSION:
        ret.type = ZAL_BOOL_VALUE;
        ret.u.bool_value = (l<=r);
        break;
    default:
        /* TODO: error */
        break;
    }
    zal_stack_push(inter, &ret);
}
static void eval_binary_string_expr(ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right){
    if(type==NE_EXPRESSION || type==EQ_EXPRESSION){
        eval_eq_ne_expr(inter, type, left, right);
        return;
    }else if(type==ADD_EXPRESSION){
        // conver
        // TODO: 字符串连接
    }else{
        /* TODO: error: string操作符错误 */
    }
}
static void eval_binary_array_expr(ZAL_Interpreter *inter, ExpressionType type, ZAL_Value *left, ZAL_Value *right){
    // DBG_assert(left->type==ZAL_ARRAY_VALUE);
    // DBG_assert(right->type==ZAL_ARRAY_VALUE);
    if(type==NE_EXPRESSION || type==EQ_EXPRESSION){
        eval_eq_ne_expr(inter, type, left, right);
        return;
    }else if(type==ADD_EXPRESSION){
        // TODO: 数组连接
    }else{
        /* TODO: error: array操作符错误 */
    }
}


/***************************** eval_func_call_expr *********************************/
// TODO: env扩展要变
static ZAL_LocalEnvironment *alloc_local_env(ZAL_Interpreter *inter){
    ZAL_LocalEnvironment *ret = MEM_alloc(sizeof(ZAL_LocalEnvironment));
    ret->local_variable = NULL;
    ret->next = inter->last_env;
    inter->last_env = ret;
    return ret;
}
static void destroy_local_env(ZAL_Interpreter *inter){
    // TODO: free其他引用, 有对象的时候
    ZAL_LocalEnvironment *env=inter->last_env;
    inter->last_env = inter->last_env->next;
    MEM_free(env);
}
static void call_zal_func(ZAL_Interpreter *inter, ZAL_LocalEnvironment *caller_env,\
 ZAL_LocalEnvironment *called_env, Expression *expr, FuncDefList *func){
    ZAL_Value ret;
    StatementResult result;
    ParameterList *para=func->u.zal_f.para;
    ArgumentList *argv_expr=expr->u.func_call_expr.argv;
    for(; argv_expr; argv_expr=argv_expr->next, para=para->next){
        if(!para){
            // zal_runtime_error();  // TODO: 实参过多
        }
        VariableList *var=zal_add_local_variable(called_env, para->name);
        eval_expr(inter, caller_env, argv_expr->argv);
        var->value = zal_stack_pop(inter);
    }
    if(para){
        // zal_runtime_error();  // TODO: 实参过少
    }
    result = zal_exe_stat_list(inter, called_env, func->u.zal_f.block->state_list);
    if(result.type == RETURN_STAT_RESULT){
        ret = result.u.result_v;
    }else{
        ret.type = ZAL_NULL_VALUE;
    }
    zal_stack_push(inter, &ret);
}
static void call_native_func(ZAL_Interpreter *inter, ZAL_LocalEnvironment *caller_env,\
 ZAL_LocalEnvironment *called_env, Expression *expr, FuncDefList *func){
    ZAL_Value ret;
    ZAL_Value *argv_value=NULL;
    ArgumentList *argv_expr=expr->u.func_call_expr.argv;
    int argc=0;
    for(; argv_expr; argv_expr=argv_expr->next, argc++){
        eval_expr(inter, caller_env, argv_expr->argv);
    }
    argv_value = &(inter->stack.stack[inter->stack.top-argc]);
    ret = func->u.native_f.proc(inter, called_env, argc, argv_value);
    zal_stack_shrink(inter, argc);
    zal_stack_push(inter, &ret);
}

/*****************************  *********************************/

/* TODO */
static ZAL_Value *get_array_lvalue(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){;}

/* TODO: 函数返回等 */
static ZAL_Value *get_lvalue(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    ZAL_Value *ret=NULL;
    if(expr->type == IDENTIFIER_EXPRESSION){
        VariableList *var=NULL;
        if(NULL==env){
            var = zal_search_global_variable(inter, expr->u.identifier_expr);
            if(NULL==var){
                var = zal_add_global_variable(inter, expr->u.identifier_expr);
            }
        }else{
            var = zal_search_local_variable(inter, env, expr->u.identifier_expr);
            if(NULL==var){
                var = zal_add_local_variable(env, expr->u.identifier_expr);
            }
        }
        ret = &(var->value);
    }else if(expr->type == INDEX_EXPRESSION){
        // TODO: ret = get_array_lvalue(inter, env, expr);
    }else{
        /* TODO: error */
    }
    return ret;
}

static void covert_bool_to_int(ZAL_Value *value){
    if(value->type==ZAL_BOOL_VALUE){
        int tmp = value->u.bool_value;
        value->type = ZAL_INT_VALUE;
        value->u.int_value = tmp;
    }
}

static ZAL_Boolean compare_array(ZAL_Interpreter *inter, ZAL_Value *left, ZAL_Value *right){
    // DBG_assert(left->type==ZAL_ARRAY_VALUE);
    // DBG_assert(right->type==ZAL_ARRAY_VALUE);
    ZAL_Array *l = &left->u.object->u.array;
    ZAL_Array *r = &right->u.object->u.array;
    if(l==r) return ZAL_TRUE;
    if(l->size!=r->size){
        return ZAL_FALSE;
    }else{
        int i;
        for(i=0; i<l->size; i++){
            eval_eq_ne_expr(inter, EQ_EXPRESSION, &(l->array[i]), &(r->array[i])); // TODO: 循环引用
            if(zal_stack_pop(inter).u.bool_value==ZAL_FALSE) return ZAL_FALSE;
        }
        return ZAL_TRUE;
    }
}