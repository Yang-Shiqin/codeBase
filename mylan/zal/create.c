#include "zal_in.h"
#include <string.h>

#define my_is_digit(x) (((x)->type == INT_EXPRESSION)||((x)->type == DOUBLE_EXPRESSION))

/*************************** 表达式 ********************************/

Expression* zal_alloc_expr(ExpressionType type){
    Expression *expr = zal_alloc(sizeof(Expression));
    expr->type = type;
    expr->line_number = zal_get_current_inter()->line_number;
    return expr;
}

Expression* zal_create_identifier_expr(char *identifier){
    Expression *expr = zal_alloc_expr(IDENTIFIER_EXPRESSION);
    expr->u.identifier_expr = identifier;
    return expr;
}

Expression* zal_create_bool_expr(ZAL_Boolean value){
    Expression *expr = zal_alloc_expr(BOOL_EXPRESSION);
    expr->u.bool_expr = value;
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

// 常量折叠后的值转变成表达式, 所以种类有限
static Expression convert_value_to_expression(ZAL_Value *value, int line_number){
    Expression expr;
    if(value->type == ZAL_INT_VALUE){
        expr.type = INT_EXPRESSION;
        expr.u.int_expr = value->u.int_value;
    }else if(value->type == ZAL_DOUBLE_VALUE){
        expr.type = DOUBLE_EXPRESSION;
        expr.u.double_expr = value->u.double_value;
    }else{
        DBG_assert(value->type == ZAL_BOOL_VALUE, ("value->type=%s\n", zal_value_type_to_str(value->type)));
        expr.type = BOOL_EXPRESSION;
        expr.u.bool_expr = value->u.bool_value;
    }
    expr.line_number = line_number;
    return expr;
}

Expression* zal_create_binary_expr(ExpressionType operator, Expression *left, Expression *right){
    if(my_is_digit(left) && my_is_digit(right)){
        ZAL_Interpreter *inter = zal_get_current_inter();
        ZAL_Value value = zal_eval_binary_expr(inter, NULL, operator, left, right);
        *left = convert_value_to_expression(&value, left->line_number);
        /*要free right吗*/
        return left;
    }
    Expression *expr = zal_alloc_expr(operator);
    expr->u.binary_expr.left = left;
    expr->u.binary_expr.right = right;
    return expr;
}

Expression* zal_create_minus_expr(Expression *expr){
    if(my_is_digit(expr)){
        ZAL_Interpreter *inter = zal_get_current_inter();
        ZAL_Value value = zal_eval_minus_expr(inter, NULL, expr);
        *expr = convert_value_to_expression(&value, expr->line_number);
        return expr;
    }
    Expression *minus_expr = zal_alloc_expr(MINUS_EXPRESSION);
    minus_expr->u.minus_expr = expr;
    return minus_expr;
}

Expression* zal_create_inc_dec_expr(Expression *expr, ExpressionType type){
    Expression *inc_dec_expr = zal_alloc_expr(type);
    inc_dec_expr->u.inc_dec_expr = expr;
    return inc_dec_expr;
}

Expression* zal_create_func_call_expr(char *identifier, ArgumentList *argv){
    Expression *expr = zal_alloc_expr(FUNC_CALL_EXPRESSION);
    expr->u.func_call_expr.identifier = identifier;
    expr->u.func_call_expr.argv = argv;
    return expr;
}

Expression* zal_create_method_call_expr(Expression *expr, char *identifier, ArgumentList *argv){
    Expression *m_call_expr = zal_alloc_expr(METHOD_CALL_EXPRESSION);
    m_call_expr->u.method_call_expr.expr = expr;
    m_call_expr->u.method_call_expr.identifier = identifier;
    m_call_expr->u.method_call_expr.argv = argv;
    return m_call_expr;
}

// 创建字面数组
Expression* zal_create_array_expr(ExpressionList *elem_list){
    Expression *expr = zal_alloc_expr(ARRAY_EXPRESSION);
    expr->u.array_liter_expr = elem_list;
    return expr;
}

Expression* zal_create_index_expr(Expression *array, Expression *index){
    Expression *idx = zal_alloc_expr(INDEX_EXPRESSION);
    idx->u.index_expr.array = array;
    idx->u.index_expr.index = index;
    return idx;
}

ExpressionList* zal_create_expr_list(Expression* expr){
    ExpressionList *expr_list = zal_alloc(sizeof(ExpressionList));
    expr_list->expr = expr;
    expr_list->next = NULL;
    return expr_list;
}

ExpressionList* zal_add_expr_to_list(ExpressionList* expr_list, Expression* expr){
    if(expr_list==NULL){
        expr_list = zal_create_expr_list(expr);
        return expr_list;
    }
    ExpressionList *pos = expr_list;
    for(; pos->next; pos = pos->next) continue;
    pos->next = zal_create_expr_list(expr);
    return expr_list;
}


/*************************** 参数 ********************************/

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


/*************************** 语句 ********************************/

Statement* zal_alloc_stat(StatementType type){
    Statement *stat = zal_alloc(sizeof(Statement));
    stat->type = type;
    stat->line_number = zal_get_current_inter()->line_number;
    return stat;
}

Statement* zal_create_expr_statement(Expression* expr){
    Statement* stat = zal_alloc_stat(EXPRESSION_STAT);
    stat->u.expression_s = expr;
    return stat;
}

Statement* zal_create_if_statement(Expression *condition, Block *then_block, Elif *elif, Block *else_block){
    Statement* stat = zal_alloc_stat(IF_STAT);
    stat->u.if_s.condition = condition;
    stat->u.if_s.then_block = then_block;
    stat->u.if_s.elif = elif;
    stat->u.if_s.else_block = else_block;
    return stat;
}

Elif* zal_create_elif(Expression *condition, Block *block){
    Elif* elif_list = zal_alloc(sizeof(Elif));
    elif_list->condition = condition;
    elif_list->block = block;
    elif_list->next = NULL;
    return elif_list;
}

Elif* zal_add_elif_to_list(Elif *elif, Expression *condition, Block *block){
    if(elif==NULL){
        elif = zal_create_elif(condition, block);
        return elif;
    }
    Elif *pos = elif;
    for(; pos->next; pos = pos->next) continue;
    pos->next = zal_create_elif(condition, block);
    return elif;
}

Statement* zal_create_for_statement(Expression *init, Expression *condition, Expression *post, Block *block){
    Statement* stat = zal_alloc_stat(FOR_STAT);
    stat->u.for_s.init = init;
    stat->u.for_s.condition = condition;
    stat->u.for_s.post = post;
    stat->u.for_s.block = block;
    return stat;
}

Statement* zal_create_while_statement(Expression *condition, Block *block){
    Statement* stat = zal_alloc_stat(WHILE_STAT);
    stat->u.while_s.condition = condition;
    stat->u.while_s.block = block;
    return stat;
}

Statement* zal_create_return_statement(Expression *result){
    Statement* stat = zal_alloc_stat(RETURN_STAT);
    stat->u.return_s.result = result;
    return stat;
}

Statement* zal_create_break_statement(void){
    Statement* stat = zal_alloc_stat(BREAK_STAT);
    return stat;
}

Statement* zal_create_continue_statement(void){
    Statement* stat = zal_alloc_stat(CONTINUE_STAT);
    return stat;
}

StatementList* zal_create_statement_list(Statement* state){
    StatementList *stat_list = zal_alloc(sizeof(StatementList));
    stat_list->statement = state;
    stat_list->next = NULL;
    return stat_list;
}

StatementList* zal_add_statement_to_list(StatementList* state_list, Statement* state){
    if(state_list==NULL){
        state_list = zal_create_statement_list(state);
        return state_list;
    }
    StatementList *pos = state_list;
    for(; pos->next; pos = pos->next) continue;
    pos->next = zal_create_statement_list(state);
    return state_list;
}


/*************************** 代码块 ********************************/
Block* zal_create_block(StatementList* state_list){
    Block *block = zal_alloc(sizeof(Block));
    block->state_list = state_list;
    return block;
}

/*************************** 函数定义 ********************************/
void zal_inter_create_function(char* identifier, ParameterList *para, Block *block){
    FuncDefList *func = zal_alloc(sizeof(FuncDefList));
    func->name = identifier;
    func->type = ZAL_FUNC_DEF;
    func->u.zal_f.para = para;
    func->u.zal_f.block = block;
    ZAL_Interpreter *inter = zal_get_current_inter();
    if(inter->func_list==NULL){
        inter->func_list = func;
        func->next = NULL;
    }else{
        func->next = inter->func_list;
        inter->func_list = func;
    }
}

/*************************** 标识符 ********************************/
char* zal_create_identifier(char *str){
    char *ret = zal_alloc(strlen(str)+1);
    strcpy(ret, str);
    return ret;
}