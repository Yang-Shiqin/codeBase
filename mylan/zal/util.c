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
    ZAL_Interpreter *inter = zal_get_current_inter();
    return MEM_storage_alloc(inter->inter_storage, size);
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

int zal_is_true(ZAL_Value *value){
    int res;
    switch (value->type)
    {
    case ZAL_BOOL_VALUE:
        res = value->u.bool_value;
        break;
    case ZAL_INT_VALUE:
        res = value->u.int_value;
        break;
    case ZAL_DOUBLE_VALUE:
        res = value->u.double_value;
        break;
    case ZAL_STRING_VALUE:
        res = value->u.object->u.string!=NULL;
        break;
    case ZAL_ARRAY_VALUE:
        res = value->u.object->u.array!=NULL;
        break;
    case ZAL_NATIVE_POINTER_VALUE:
        res = value->u.pointer.pointer!=NULL;
        break;
    case ZAL_NULL_VALUE:
        res = 0;
        break;
    default:
    /* TODO: error */
    }
    return res;
}