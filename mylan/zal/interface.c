#include "zal_in.h"
#include <stddef.h>
#include <stdlib.h>

// 注册所有内置函数
void add_native_func(ZAL_Interpreter *inter){
    ZAL_add_native_function(inter, "print", zal_nv_print);
    ZAL_add_native_function(inter, "fopen", zal_nv_fopen);
    ZAL_add_native_function(inter, "fclose", zal_nv_fclose);
    ZAL_add_native_function(inter, "fgets", zal_nv_fgets);
    ZAL_add_native_function(inter, "fget_line", zal_nv_fget_line);
    ZAL_add_native_function(inter, "fputs", zal_nv_fputs);
    ZAL_add_native_function(inter, "fget_line", zal_nv_fget_line);
}

// 创建解释器
ZAL_Interpreter *ZAL_create_interpreter(void){
    MEM_Storage *stor;
    ZAL_Interpreter *inter=NULL;
    stor = MEM_open_storage(0);
    inter = MEM_storage_alloc(stor, sizeof(ZAL_Interpreter));
    inter->inter_storage = stor;
    inter->exe_storage = NULL;
    inter->line_number = 1;
    inter->func_list = NULL;
    inter->g_varible = NULL;
    inter->state_list = NULL;
    inter->stack.size = 0;
    inter->stack.top = 0;
    inter->stack.stack = MEM_alloc(sizeof(ZAL_Value)*STACK_ALLOC_SIZE);
    inter->heap.size = 0;
    inter->heap.threshold = HEAP_THRESHOLD;
    inter->heap.heap = NULL;
    inter->last_env = NULL;
    zal_set_current_inter(inter);
    add_native_func(inter);
    return inter;
}

// 解析zal脚本, 构建分析树
void ZAL_compile(ZAL_Interpreter *inter, FILE *fp){
    extern int yyparse(void);
    extern FILE *yyin;
    zal_set_current_inter(inter);
    yyin = fp;
    if(yyparse()){
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    // TODO
}

// 解释器运行
void ZAL_interpret(ZAL_Interpreter *inter){
    inter->exe_storage = MEM_open_storage(0);
    zal_add_std_fp(inter);
    zal_exe_stat_list(inter, NULL, inter->state_list);
    zal_mark_sweep_gc(inter);
}

// 回收解释器
void ZAL_destroy_interpreter(ZAL_Interpreter *inter){
    VariableList *var = NULL;
    while(inter->g_varible){
        var = inter->g_varible;
        inter->g_varible = var->next;
        MEM_free(var);
    }
    MEM_close_storage(inter->exe_storage);
    MEM_free(inter->stack.stack);
    zal_mark_sweep_gc(inter);       // 回收所有对象
    MEM_close_storage(inter->inter_storage);
}

// 添加一个内置函数
void ZAL_add_native_function(ZAL_Interpreter *inter, char *identifier, ZAL_NativeFuncProc func){
    FuncDefList *func_node = zal_alloc(sizeof(FuncDefList));
    func_node->name = identifier;
    func_node->type = NATIVE_FUNC_DEF;
    func_node->u.native_f.proc = func;
    func_node->next = inter->func_list;
    inter->func_list = func_node;
}