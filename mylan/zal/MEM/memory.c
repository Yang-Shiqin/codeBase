#include "memory.h"
#include <stdlib.h>

static MEM_Controller s_default_mem_controller = {
    NULL; // stderr
    default_error_handler;
    MEM_FAIL_EXIT
};

MEM_Controller* mem_default_controller = &s_default_mem_controller;


static void default_error_handler(MEM_Controller *ctrl, char *file, int line, char *msg){
    fprintf(ctrl->err_out_fp, "MEM:%s failed in %s at line:%d\n", msg, file, line);
}

static void error_handler(MEM_Controller *ctrl, char *file, int line, char *msg){
    if(ctrl->err_out_fp==NULL){
        ctrl->err_out_fp = stderr;
    }
    ctrl->err_handler(ctrl, file, line, msg);
    if(ctrl->fail_mode==MEM_FAIL_EXIT){
        exit(1);
    }
}

MEM_Controller* MEM_create_controller(void){
    MEM_Controller *ctrl = MEM_alloc(sizeof(MEM_Controller));
    *ctrl = s_default_mem_controller;
    return ctrl;
}
void MEM_set_err_out_fp(MEM_Controller *ctrl, FILE *err_out_fp){
    ctrl->err_out_fp = err_out_fp;
}
void MEM_set_err_handler(MEM_Controller *ctrl, MEM_ErrorHandler err_handler){
    ctrl->err_handler = err_handler;
}
void MEM_set_fail_mode(MEM_Controller *ctrl, MEM_FailMode fail_mode){
    ctrl->fail_mode = fail_mode;
}


void* mem_alloc_func(MEM_Controller* ctrl, int size, char* file, int line){
    void *ptr=NULL;
    size_t alloc_size;
    alloc_size = size;
    ptr = malloc(alloc_size);
    if(ptr==NULL){
        error_handler(ctrl, file, line, "malloc");
    }
    return ptr;
}

void* mem_realloc_func(MEM_Controller* ctrl, void* old_ptr, int size, char* file, int line){
    void *new_ptr=NULL, *real_ptr=NULL;
    size_t alloc_size;
    alloc_size = size;
    real_ptr = old_ptr;
    new_ptr = realloc(real_ptr, alloc_size);
    if(new_ptr == NULL){
        if(old_ptr==NULL){
            error_handler(ctrl, file, line, "realloc(malloc)");
        }else{
            error_handler(ctrl, file, line, "realloc");
            free(real_ptr);
        }
    }
    return new_ptr;
}

void mem_free_func(MEM_Controller* ctrl, void* ptr){
    if(ptr==NULL) return;
    void *real_ptr;
    real_ptr = ptr;
    free(real_ptr);
}




MEM_Storage *mem_open_storage_func(MEM_Controller* ctrl, char *file, int line);
void* mem_storage_alloc_func(MEM_Controller* ctrl, MEM_Storage *storage, int size, char* file, int line);
void mem_close_storage(MEM_Controller* ctrl, MEM_Storage *storage, char* file, int line);