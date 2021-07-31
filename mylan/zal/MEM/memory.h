#ifndef __MEMORY_H__
#define __MEMORY_H__
#include "zal_in.h"
#include <stdio.h>



// 内存管理器
typedef struct MEM_Controller_tag{
    FILE                *err_out_fp;
    MEM_ErrorHandler    *err_handler;
    MEM_FailMode        fail_mode;
}MEM_Controller;

void* mem_alloc_func(MEM_Controller* ctrl, int size, char* file, int line);

void* mem_realloc_func(MEM_Controller* ctrl, void* old_ptr, int size, char* file, int line);

void mem_free_func(MEM_Controller* ctrl, void* ptr);



/************ 内存存储块 ************/

struct MEM_Storage_tag{
    /* TODO */
};

void* MEM_storage_alloc_func(MEM_Controller* ctrl, MEM_Storage storage, int size, char* file, int line);

#endif