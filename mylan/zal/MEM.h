#ifndef __MEM_H__
#define __MEM_H__
#include "MEM/memory.h"

typedef enum{
    MEM_FAIL_EXIT,
    MEM_FAIL_RETURN
}MEM_FailMode;

typedef struct MEM_Controller_tag   MEM_Controller;
typedef struct MEM_Storage_tag      MEM_Storage;
typedef void (*MEM_ErrorHandler)(MEM_Controller*, char *, int, char*)    // 内存管理器, file, line, msg


#ifdef MEM_CONTROLLER 
#define MEM_CURRENT_CONTROLLER MEM_CONTROLLER
#else
#define MEM_CURRENT_CONTROLLER mem_default_controller



MEM_Controller* MEM_create_controller(void);
void MEM_set_err_out_fp(MEM_Controller *ctrl, FILE *err_out_fp);
void MEM_set_err_handler(MEM_Controller *ctrl, MEM_ErrorHandler err_handler);
void MEM_set_fail_mode(MEM_Controller *ctrl, MEM_FailMode fail_mode);


#define MEM_alloc(size) (mem_alloc_func(MEM_CURRENT_CONTROLLER, size, __FILE__, __LINE__))
#define MEM_realloc(old_ptr, size) (mem_realloc_func(MEM_CURRENT_CONTROLLER, old_ptr, size, __FILE__, __LINE__))
#define MEM_free(ptr) (mem_free_func(MEM_CURRENT_CONTROLLER, ptr))

#define MEM_open_storage(page_size) (mem_open_storage_func(MEM_CURRENT_CONTROLLER, page_size, __FILE__, __LINE__))
#define MEM_storage_alloc(storage, size) (mem_storage_alloc_func(MEM_CURRENT_CONTROLLER, storage, size, __FILE__, __LINE__))
#define MEM_close_storage(storage) (mem_close_storage(MEM_CURRENT_CONTROLLER, storage, __FILE__, __LINE__))
#endif