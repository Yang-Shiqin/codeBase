// 执行语句
#include "zal_in.h"

static StatementResult exe_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat);
static StatementResult exe_if_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat);
static StatementResult exe_elif_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Elif *elif, int *flag);
static StatementResult exe_for_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat);
static StatementResult exe_while_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat);
static StatementResult exe_break_stat(void);
static StatementResult exe_continue_stat(void);
static StatementResult exe_return_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat);

StatementResult zal_exe_stat_list(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, StatementList *list){
    StatementResult result;
    StatementList *pos=list;
    for(; pos; pos=pos->next){
        result = exe_stat(inter, env, pos->statement);
        if(result.type != NORMAL_STAT_RESULT) break;    // return类型
    }
    return result;
}

static StatementResult exe_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat){
    StatementResult result;
    switch (stat->type)
    {
    case EXPRESSION_STAT:
        result.type = NORMAL_STAT_RESULT;
        result.u.result_v = zal_eval_expr(inter, env, stat->u.expression_s);
        break;
    case IF_STAT:
        result = exe_if_stat(inter, env, stat);
        break;
    case FOR_STAT:
        result = exe_for_stat(inter, env, stat);
        break;
    case WHILE_STAT:
        result = exe_while_stat(inter, env, stat);
        break;
    case BREAK_STAT:
        result = exe_break_stat();
        break;
    case CONTINUE_STAT:
        result = exe_continue_stat();
        break;
    case RETURN_STAT:
        result = exe_return_stat(inter, env, stat);
        break;
    default:
        DBG_panic(("bad statement type: %d\n", stat->type));
    }
    return result;
}

static StatementResult exe_if_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat){
    StatementResult result;
    ZAL_Value condition;
    int flag;
    condition = zal_eval_expr(inter, env, stat->u.if_s.condition);
    if(zal_is_true(&condition)){
        result = zal_exe_stat_list(inter, env, stat->u.if_s.then_block->state_list);
    }else{
        result = exe_elif_stat(inter, env, stat->u.if_s.elif, &flag);
        if(!flag)
            result = zal_exe_stat_list(inter, env, stat->u.if_s.else_block->state_list);
    }
    return result;
}
static StatementResult exe_elif_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Elif *elif, int *flag){
    StatementResult result;
    ZAL_Value condition;
    Elif *pos=elif;
    *flag = 0;
    for(;pos;pos=pos->next){
        condition = zal_eval_expr(inter, env, pos->condition);
        if(zal_is_true(&condition)){
            *flag = 1;
            result = zal_exe_stat_list(inter, env, pos->block->state_list);
            return result;
        }
    }
    
    return result;
}

static StatementResult exe_for_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat){
    StatementResult result;
    ZAL_Value condition;
    zal_eval_expr(inter, env, stat->u.for_s.init);
    condition = zal_eval_expr(inter, env, stat->u.for_s.condition);
    for(; zal_is_true(&condition);
            zal_eval_expr(inter, env, stat->u.for_s.post))
    {
        result = zal_exe_stat_list(inter, env, stat->u.for_s.block->state_list);
        condition = zal_eval_expr(inter, env, stat->u.for_s.condition);
        if(result.type == RETURN_STAT_RESULT) break;
        if(result.type == BREAK_STAT_RESULT){
            result.type = NORMAL_STAT_RESULT;   // 与for相消
            break;
        }
        if(result.type == CONTINUE_STAT_RESULT){
            result.type = NORMAL_STAT_RESULT;   // 与for相消
            continue;
        }
    }
    return result;
}

static StatementResult exe_while_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat){
    StatementResult result;
    ZAL_Value condition;
    condition = zal_eval_expr(inter, env, stat->u.while_s.condition);
    while(zal_is_true(&condition))
    {
        result = zal_exe_stat_list(inter, env, stat->u.while_s.block->state_list);
        condition = zal_eval_expr(inter, env, stat->u.while_s.condition);
        if(result.type == RETURN_STAT_RESULT) break;
        if(result.type == BREAK_STAT_RESULT){
            result.type = NORMAL_STAT_RESULT;   // 与for相消
            break;
        }
        if(result.type == CONTINUE_STAT_RESULT){
            result.type = NORMAL_STAT_RESULT;   // 与for相消
            continue;
        }
    }
    return result;
}

static StatementResult exe_break_stat(){
    StatementResult result;
    result.type = BREAK_STAT;
    return result;
}

static StatementResult exe_continue_stat(){
    StatementResult result;
    result.type = CONTINUE_STAT;
    return result;
}

static StatementResult exe_return_stat(ZAL_Interpreter *inter, ZAL_LocalEnvironment *env, Statement *stat){
    StatementResult result;
    result.type = RETURN_STAT;
    result.u.result_v = zal_eval_expr(inter, env, stat->u.return_s.result);
    return result;
}
