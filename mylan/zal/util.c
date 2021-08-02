#include "zal_in.h"
#include <string.h>

// 报错时
static ZAL_Interpreter *s_current_inter;



ZAL_Interpreter* zal_get_current_inter(void){
    return s_current_inter;
}

void zal_set_current_inter(ZAL_Interpreter* inter){
    s_current_inter = inter;
}

// 语法分析时在解释器alloc一块空间
void* zal_alloc(size_t size){
    /* TODO */
}

VariableList *zal_search_local_variable(ZAL_Interpreter* inter, ZAL_LocalEnvironment *env, char *identifier){
    DBG_ASSERT(env);
    VariableList *pos = env->local_variable;
    for(; pos; pos = pos->next){
        if(0==strcpy(pos->name, identifier)) return pos;
    }
    pos = zal_search_global_variable(inter, identifier);
    return pos;
}

VariableList *zal_search_global_variable(ZAL_Interpreter* inter, char *identifier){
    VariableList *pos = inter->g_varible;
    for(; pos; pos = pos->next){
        if(0==strcpy(pos->name, identifier)) return pos;
    }
    return NULL;
}

VariableList *zal_add_global_variable(ZAL_Interpreter* inter, char *identifier){
    VariableList *var = MEM_alloc(sizeof(VariableList));
    var->ZAL_Value.type = ZAL_NULL_VALUE;
    var->name = identifier;
    var->next = inter->g_varible;
    inter->g_varible = var;
    return var;
}

VariableList *zal_add_local_variable(ZAL_LocalEnvironment *env, char *identifier){
    VariableList *var = MEM_alloc(sizeof(VariableList));
    var->ZAL_Value.type = ZAL_NULL_VALUE;
    var->name = identifier;
    var->next = env->local_variable;
    env->local_variable = var;
    return var;
}

