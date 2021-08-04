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


#define larger(a, b) (((a)>(b))?(a):(b))

MEM_Storage *mem_open_storage_func(MEM_Controller* ctrl, int page_size, char *file, int line){
    MEM_Storage *stor=mem_alloc_func(ctrl, sizeof(MEM_Storage), file, line);
    stor->page_list=NULL;
    stor->page_size = (page_size>0) ? page_size : DEFAULT_PAGE_SIZE;
    return stor;
}
void* mem_storage_alloc_func(MEM_Controller* ctrl, MEM_Storage *storage, int size, char* file, int line){
    int cell_num;
    cell_num = (size-1)/CELL_SIZE + 1;
    void *p=NULL;
    if(storage->page_list && \
    storage->page_list->use_cell_num+cell_num<storage->page_list->cell_num){
        p = &(storage->page_list->cell[storage->page_list->use_cell_num]);
        storage->page_list->use_cell_num += cell_num;
    }else{
        int alloc_cell_num;
        MemoryPageList *new_page=NULL;
        alloc_cell_num = larger(cell_num, storage->page_size);
        new_page = mem_alloc_func(ctrl, (alloc_cell_num)*CELL_SIZE+sizeof(MemoryPageList), file, line);
        new_page->cell_num=alloc_cell_num;
        new_page->use_cell_num=cell_num;
        new_page->next=storage->page_list;
        storage->page_list = new_page;
        p = &(storage->page_list->cell[0]);
    }
    return p;
}
void mem_close_storage(MEM_Controller* ctrl, MEM_Storage *storage, char* file, int line){
    if(storage==NULL) return;
    MemoryPageList *pos=storage->page_list;
    for(; pos;){
        pos = pos->next;
        mem_free_func(ctrl, storage->page_list);
        storage->page_list = pos;
    }
    mem_free_func(ctrl, storage);
}