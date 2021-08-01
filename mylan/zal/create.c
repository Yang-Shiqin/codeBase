#include "zal_in.h"
#include <string.h>


Expression* zal_alloc_expr(ExpressionType type){
    Expression *expr = zal_alloc(sizeof(Expression));
    expr->type = type;
    expr->line_number = zal_get_current_inter()->line_number;
    return expr;
}

Expression* zal_create_bool_expr(ZAL_Boolean value){
    Expression *expr = zal_alloc_expr(BOOL_EXPRESSION);
    expr->u.bool_expr = value;
    return expr;
}

Expression* zal_create_identifier_expr(char *identifier){
    Expression *expr = zal_alloc_expr(IDENTIFIER_EXPRESSION);
    expr->u.identifier_expr = identifier;
    return expr;
}

Expression* zal_create_null_expr(void){
    Expression *expr = zal_alloc_expr(NULL_EXPRESSION);
    return expr;
}

Expression* zal_create_assign_expr(Expression *l_value, Expression *r_value){
    Expression *expr = zal_alloc_expr(ASSIGN_EXPRESSION);
    expr->u.assign_expr.l_value = l_value;
    expr->u.assign_expr.r_value = r_value;
    return expr;
}

Expression* zal_create_binary_expr(ExpressionType operator, Expression *left, Expression *right){
    Expression *expr = zal_alloc_expr(operator);
    expr->u.binary_expr.left = left;
    expr->u.binary_expr.right = right;
    return expr;
}

Expression* zal_create_minus_expr(Expression *expr){
    Expression *expr = zal_alloc_expr(MINUS_EXPRESSION);
    expr->u.minus_expr = expr;
    return expr;
}

Expression* zal_create_inc_dec_expr(Expression *expr, ExpressionType type){
    Expression *expr = zal_alloc_expr(type);
    expr->u.inc_dec_expr = expr;
    return expr;
}

Expression* zal_create_func_call_expr(char *identifier, ArgumentList *argv){
    Expression *expr = zal_alloc_expr(FUNC_CALL_EXPRESSION);
    expr->u.func_call_expr.identifier = identifier;
    expr->u.func_call_expr.argv = argv;
    return expr;
}

Expression* zal_create_method_call_expr(Expression *expr, char *identifier, ArgumentList *argv){
    Expression *expr = zal_alloc_expr(METHOD_CALL_EXPRESSION);
    expr->u.method_call_expr.expr = expr;
    expr->u.method_call_expr.identifier = identifier;
    expr->u.method_call_expr.argv = argv;
    return expr;
}

// 创建实参链表
ArgumentList* zal_create_argument_list(Expression *expr){
    ArgumentList *argument = zal_alloc(sizeof(ArgumentList));
    argument->argv = expr;
    argument->next = NULL;
    return argument;
}

ArgumentList* zal_add_argument_to_list(ArgumentList* arg_list, Expression *expr){
    if(arg_list==NULL){
        arg_list = zal_create_argument_list(expr);
        return arg_list;
    }
    ArgumentList *pos = arg_list;
    for(; pos->next; pos = pos->next) continue; // 到尾结点
    pos->next = zal_create_argument_list(expr);
    return arg_list;
}

ParameterList* zal_create_parameter_list(char *name){
    ParameterList *para = zal_alloc(sizeof(ParameterList));
    para->name = name;
    para->next = NULL;
    return para;
}

ParameterList* zal_add_parameter_to_list(ParameterList* para_list, char *name){
    if(para_list==NULL){
        para_list = zal_create_parameter_list(name);
        return para_list;
    }
    ParameterList *pos = para_list;
    for(; pos->next; pos = pos->next) continue; // 到尾结点
    pos->next = zal_create_parameter_list(name);
    return para_list;
}






char* zal_create_identifier(char *str){
    char *ret = zal_alloc(strlen(str)+1);
    strcpy(ret, src);
    return ret;
}