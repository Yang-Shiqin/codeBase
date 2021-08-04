#include "zal_in.h"

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
    inter->heap.pointer = 0;
    inter->heap.heap = NULL;

    zal_set_current_inter(inter);
    return inter;
}

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

void ZAL_interpret(ZAL_Interpreter *inter){
    inter->exe_storage = MEM_open_storage(0);
    // TODO
    zal_exe_stat_list(inter, NULL, inter->state_list);

}

void ZAL_destroy_interpreter(ZAL_Interpreter *inter){
    MEM_close_storage(inter->exe_storage);
    // TODO
    MEM_free(inter->stack.stack);
    MEM_close_storage(inter->inter_storage);
}