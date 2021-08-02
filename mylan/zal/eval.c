#include "zal_in.h"

static void eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);


/* TODO */
static ZAL_Value *get_array_lvalue(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr);

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
        ret = get_array_lvalue(inter, env, expr);
    }else{
        /* TODO: error */
    }
    return ret;
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
    ret.u.object->type = STRING_OBJ;
    ret.u.object->u.string->is_literal = ZAL_TRUE;
    ret.u.object->u.string->string = string_expr;
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
    ZAL_Value ret;
    ZAL_Value *dest = get_lvalue(inter, env, expr);

    eval_expr(inter, env, r_value);
    ret = zal_stack_peek(inter, 0);

    *dest = *ret;
}

static void eval_binary_int_expr(ExpressionType type, ZAL_Value *left, ZAL_Value *right);

/* TODO */
static void eval_binary_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, ExpressionType type, Expression *left, Expression *right){
    ZAL_Value *l_v=NULL, *r_v=NULL;
    eval_expr(inter, env, left);
    eval_expr(inter, env, right);
    r_v = zal_stack_pop(inter);
    l_v = zal_stack_pop(inter);
    if(r_v->type==ZAL_INT_VALUE && l_v->type==ZAL_INT_VALUE){
        eval_binary_int_expr(type, l_v, r_v);
    }else if(r_v->type==ZAL_INT_VALUE && l_v->type==ZAL_INT_VALUE)
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

/* TODO */
static void eval_func_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    ZAL_Value ret;
    zal_stack_push(inter, &ret);
}

/* TODO */
static void eval_method_call_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){
    ZAL_Value ret;
    zal_stack_push(inter, &ret);
}


static void eval_expr(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Expression *expr){ 
    DBG_assert(expr);
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
        eval_index_expr(inter, expr->u.array_liter_expr.array, expr->u.array_liter_expr.index);
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
    }
}



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

