#ifndef __MEMORY_H__
#define __MEMORY_H__
#include "zal_in.h"
#include <stdio.h>



// 内存管理器
struct MEM_Controller_tag{
    FILE                *err_out_fp;
    MEM_ErrorHandler    err_handler;
    MEM_FailMode        fail_mode;
    Header              *block_header;
};

typedef struct Header_tag{
    int                 size;
    char                *file;
    int                 line;
    struct Header_tag   *prev;
    struct Header_tag   *next;
}Header;


void* mem_alloc_func(MEM_Controller* ctrl, int size, char* file, int line);

void* mem_realloc_func(MEM_Controller* ctrl, void* old_ptr, int size, char* file, int line);

void mem_free_func(MEM_Controller* ctrl, void* ptr);



/************ 内存存储块 ************/

typedef union{
    long    l_dummy;
    double  d_dummy;
    void*   v_dummy;
}Cell;
typedef MemoryPageList_tag  MemoryPageList;

#define CELL_SIZE (sizeof(Cell))
#define DEFAULT_PAGE_SIZE (1024) 

struct MEM_Storage_tag{
    MemoryPageList      *page_list;
    int                 page_size;      // 用来保证每次alloc的页大小不太小
};

struct MemoryPageList_tag{
    int             cell_num;
    int             use_cell_num;
    MemoryPageList  *next;
    Cell            cell[0];        // 柔性数组
};

MEM_Storage *mem_open_storage_func(MEM_Controller* ctrl, int page_size, char *file, int line);
void* mem_storage_alloc_func(MEM_Controller* ctrl, MEM_Storage *storage, int size, char* file, int line);
void mem_close_storage(MEM_Controller* ctrl, MEM_Storage *storage, char* file, int line);
#endif