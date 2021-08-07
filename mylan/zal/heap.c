#include <string.h>
#include "zal_in.h"

#define is_obj(x) ((x)->type==ZAL_STRING_VALUE || (x)->type==ZAL_ARRAY_VALUE)   // ZAL_Value*类型


ZAL_Object* alloc_obj(ZAL_Interpreter *inter, ObjectType type);

static void check_gc(ZAL_Interpreter *inter);
static void gc_mark_obj(ZAL_Interpreter *inter);
static void gc_sweep_obj(ZAL_Interpreter *inter);
static void gc_reset_mark(ZAL_Object *obj);
static void gc_mark(ZAL_Object *obj);
static void gc_free_obj(ZAL_Interpreter *inter, ZAL_Object *obj);

/*********************************** 定义区 *******************************************/

ZAL_Object* zal_literal_to_string(ZAL_Interpreter *inter, char *str){
    ZAL_Object *str_obj = alloc_obj(inter, STRING_OBJ);
    str_obj->u.string.is_literal = ZAL_TRUE;
    str_obj->u.string.string = str;
    return str_obj;
}

ZAL_Object* zal_non_literal_to_string(ZAL_Interpreter *inter, char *str){
    ZAL_Object *str_obj = alloc_obj(inter, STRING_OBJ);
    str_obj->u.string.is_literal = ZAL_FALSE;
    str_obj->u.string.string = str;
    inter->heap.size += strlen(str)+1;
    return str_obj;
}

void zal_mark_sweep_gc(ZAL_Interpreter* inter){
    gc_mark_obj(inter);
    gc_sweep_obj(inter);
}

ZAL_Object* alloc_obj(ZAL_Interpreter *inter, ObjectType type){
    check_gc(inter);
    ZAL_Object *obj=MEM_alloc(sizeof(ZAL_Object));
    obj->type = type;
    obj->marked = ZAL_FALSE;
    obj->next = inter->heap.heap;
    inter->heap.heap = obj;
    obj->prev = NULL;
    if(obj->next)
        obj->next->prev = obj;
    inter->heap.size += sizeof(ZAL_Object);
    return obj;
}


static void check_gc(ZAL_Interpreter *inter){
    if(inter->heap.size > inter->heap.threshold){
        zal_mark_sweep_gc(inter);
        inter->heap.threshold = inter->heap.size+HEAP_THRESHOLD;
    }
}

static void gc_mark_obj(ZAL_Interpreter *inter){
    ZAL_Object *heap_pos=inter->heap.heap;
    VariableList *var_pos=inter->g_varible;
    ZAL_LocalEnvironment *local_env_pos=inter->last_env;
    ZAL_Value *stack = inter->stack.stack;
    int stack_pos=0;
    // 清除上次记号(因为留下来的肯定是被标记的)(heap里是所有)
    for(; heap_pos; heap_pos=heap_pos->next){
        gc_reset_mark(heap_pos);
    }
    // 全局变量, 局部变量, 栈里(临时的)是可达的
    // 全局变量
    for(; var_pos; var_pos=var_pos->next){
        if(is_obj(&(var_pos->value))){
            gc_mark(var_pos->value.u.object);
        }
    }
    // 局部变量
    for(; local_env_pos; local_env_pos=local_env_pos->next){
        for(var_pos=local_env_pos->local_variable; var_pos; var_pos=var_pos->next){
            if(is_obj(&(var_pos->value))){
                gc_mark(var_pos->value.u.object);
            }
        }
        // TODO: 内置函数的对象
    }
    // 栈
    for(; stack_pos<inter->stack.top; stack_pos++){
        if(is_obj(stack+stack_pos)){
            gc_mark((stack+stack_pos)->u.object);
        }
    }
}
static void gc_sweep_obj(ZAL_Interpreter *inter){
    ZAL_Object *pos = inter->heap.heap;
    ZAL_Object *tmp = NULL;
    while(pos){
        if(!pos->marked){   // to_be_free
            if(pos->prev){
                pos->prev->next=pos->next;
            }
            if(pos->next){
                pos->next->prev=pos->prev;
            }
            tmp=pos->next;
            gc_free_obj(inter, pos);
            pos=tmp;
        }else{
            pos=pos->next;
        }
    }
}

static void gc_reset_mark(ZAL_Object *obj){
    obj->marked = ZAL_FALSE;
}
// 标记对象(str/arr)及可达对象(只有数组会指别的对象)
static void gc_mark(ZAL_Object *obj){
    DBG_assert(obj!=NULL, (NULL));
    if(obj->marked) return;
    obj->marked = ZAL_TRUE;
    if(obj->type==ARRAY_OBJ){
        ZAL_Value *array=obj->u.array.array;
        int pos=0;
        for(; pos<obj->u.array.size; pos++){
            if(is_obj(array+pos)){
                gc_mark((array+pos)->u.object);
            }
        }
    }
}
static void gc_free_obj(ZAL_Interpreter *inter, ZAL_Object *obj){
    if(obj->type==ARRAY_OBJ){
        inter->heap.size -= sizeof(ZAL_Value)*(obj->u.array.alloc_size);
        MEM_free(obj->u.array.array);
    }else{
        DBG_assert(obj->type==STRING_OBJ, (NULL));
        if(!obj->u.string.is_literal){
            inter->heap.size -= strlen(obj->u.string.string)+1;
            MEM_free(obj->u.string.string);
        }
    }
    inter->heap.size -= sizeof(ZAL_Object);
    MEM_free(obj);
}
