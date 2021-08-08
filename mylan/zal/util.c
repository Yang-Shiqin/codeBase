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

FuncDefList *zal_search_function(char *indentifier){
    ZAL_Interpreter *inter = zal_get_current_inter();
    FuncDefList *ret=inter->func_list;
    for(; ret; ret=ret->next){
        if(strcmp(ret->name, indentifier)==0) return ret;
    }
    return NULL;
}

VariableList *zal_search_local_variable(ZAL_Interpreter* inter, ZAL_LocalEnvironment *env, char *identifier){
    DBG_assert(env!=NULL, (NULL));
    VariableList *pos = env->local_variable;
    for(; pos; pos = pos->next){
        if(0==strcmp(pos->name, identifier)) return pos;
    }
    pos = zal_search_global_variable(inter, identifier);
    return pos;
}

VariableList *zal_search_global_variable(ZAL_Interpreter* inter, char *identifier){
    VariableList *pos = inter->g_varible;
    for(; pos; pos = pos->next){
        if(0==strcmp(pos->name, identifier)) return pos;
    }
    return NULL;
}

VariableList *zal_add_global_variable(ZAL_Interpreter* inter, char *identifier){
    VariableList *var = MEM_alloc(sizeof(VariableList));
    var->value.type = ZAL_NULL_VALUE;
    var->name = identifier;
    var->next = inter->g_varible;
    inter->g_varible = var;
    return var;
}

VariableList *zal_add_local_variable(ZAL_LocalEnvironment *env, char *identifier){
    VariableList *var = MEM_alloc(sizeof(VariableList));
    var->value.type = ZAL_NULL_VALUE;
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
        res = value->u.object->u.string.string!=NULL;
        break;
    case ZAL_ARRAY_VALUE:
        res = value->u.object->u.array.array!=NULL;
        break;
    case ZAL_NATIVE_POINTER_VALUE:
        res = value->u.pointer.pointer!=NULL;
        break;
    case ZAL_NULL_VALUE:
        res = 0;
        break;
    default:
        DBG_panic(("bad value type: %s\n", zal_value_type_to_str(value->type)));
        break;
    }
    return res;
}

static int my_strlen(char *str){
    if(!str) return 0;
    return strlen(str);
}

void zal_vstr_append_ch(VStr *string, char ch){
    int len = my_strlen(string->string);
    string->string = MEM_realloc(string->string, len+2);
    string->string[len] = ch;
    string->string[len+1] = '\0';
}


void zal_vstr_append_string(VStr *string, char *add_str){
    int old_len = my_strlen(string->string);
    int new_len = old_len+my_strlen(add_str)+1;
    string->string = MEM_realloc(string->string, new_len);
    strcpy(&(string->string[old_len]), add_str);
}

char *zal_value_to_str(ZAL_Value *value){
    VStr v_str={NULL};
    char buf[LINE_BUF_SIZE];
    ZAL_Value *array=NULL;
    int pos;
    switch (value->type)
    {
    case ZAL_BOOL_VALUE:
        if(value->u.bool_value){
            zal_vstr_append_string(&v_str, "true");
        }else{
            zal_vstr_append_string(&v_str, "false");
        }
        break;
    case ZAL_INT_VALUE:
        sprintf(buf, "%d", value->u.int_value);
        zal_vstr_append_string(&v_str, buf);
        break;
    case ZAL_DOUBLE_VALUE:
        sprintf(buf, "%lf", value->u.double_value);
        zal_vstr_append_string(&v_str, buf);
        break;
    case ZAL_STRING_VALUE:
        zal_vstr_append_string(&v_str, value->u.object->u.string.string);
        break;
    case ZAL_ARRAY_VALUE:
        zal_vstr_append_ch(&v_str, '[');
        for(array=value->u.object->u.array.array, pos=0; pos<value->u.object->u.array.size; pos++){
            if(pos)
                zal_vstr_append_string(&v_str, ", ");
            char *tmp=zal_value_to_str(&array[pos]);
            zal_vstr_append_string(&v_str, tmp);
            MEM_free(tmp);
        }
        zal_vstr_append_ch(&v_str, ']');
        break;
    case ZAL_NATIVE_POINTER_VALUE:
        sprintf(buf, "(%s: %p)", value->u.pointer.info->info, value->u.pointer.pointer);
        zal_vstr_append_string(&v_str, buf);
        break;
    case ZAL_NULL_VALUE:
        zal_vstr_append_string(&v_str, "null");
        break;
    default:
        DBG_panic(("bad value type: %s\n", zal_value_type_to_str(value->type)));
    }
    return v_str.string;
}

char *zal_operator_to_str(ExpressionType type){
    char *str=NULL;
    switch (type)
    {
    case BOOL_EXPRESSION:
    case INT_EXPRESSION:
    case DOUBLE_EXPRESSION:
    case NULL_EXPRESSION:
    case STRING_EXPRESSION:
    case ARRAY_EXPRESSION:
    case INDEX_EXPRESSION:
    case IDENTIFIER_EXPRESSION:
        DBG_panic(("bad expression type: %d\n", type));
    case ASSIGN_EXPRESSION:
        str = "=";
        break;
    case ADD_EXPRESSION:
        str = "+";
        break;
    case SUB_EXPRESSION:
        str = "-";
        break;
    case MUL_EXPRESSION:
        str = "*";
        break;
    case DIV_EXPRESSION:
        str = "/";
        break;
    case MOD_EXPRESSION:
        str = "%";
        break;
    case LOGICAL_AND_EXPRESSION:
        str = "&&";
        break;
    case LOGICAL_OR_EXPRESSION:
        str = "||";
        break;
    case EQ_EXPRESSION:
        str = "==";
        break;
    case NE_EXPRESSION:
        str = "!=";
        break;
    case GT_EXPRESSION:
        str = ">";
        break;
    case GE_EXPRESSION:
        str = ">=";
        break;
    case LT_EXPRESSION:
        str = "<";
        break;
    case LE_EXPRESSION:
        str = "<=";
        break;
    case MINUS_EXPRESSION:
        str = "-";
        break;
    case INCREMENT_EXPRESSION:
    case DECREMENT_EXPRESSION:
    case FUNC_CALL_EXPRESSION:
    case METHOD_CALL_EXPRESSION:
    default:
        DBG_panic(("bad expression type: %d\n", type));
    }
    return str;
}


char *zal_value_type_to_str(ZAL_ValueType type){
    char *str=NULL;
    switch (type)
    {
    case ZAL_BOOL_VALUE:
        str = "ZAL_BOOL_VALUE";
        break;
    case ZAL_INT_VALUE:
        str = "ZAL_INT_VALUE";
        break;
    case ZAL_DOUBLE_VALUE:
        str = "ZAL_DOUBLE_VALUE";
        break;
    case ZAL_STRING_VALUE:
        str = "ZAL_STRING_VALUE";
        break;
    case ZAL_ARRAY_VALUE:
        str = "ZAL_ARRAY_VALUE";
        break;
    case ZAL_NATIVE_POINTER_VALUE:
        str = "ZAL_NATIVE_POINTER_VALUE";
        break;
    case ZAL_NULL_VALUE:
        str = "ZAL_NULL_VALUE";
        break;
    default:
        DBG_panic(("bad value type: %d\n", type));
    }
    return str;
}