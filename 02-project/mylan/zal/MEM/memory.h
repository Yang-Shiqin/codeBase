#ifndef __MEMORY_H__
#define __MEMORY_H__
#include "../zal_in.h"
#include "../MEM.h"
#include <stdio.h>



typedef struct Header_tag{
    int                 size;
    char                *file;
    int                 line;
    struct Header_tag   *prev;
    struct Header_tag   *next;
}Header;

// 内存管理器
struct MEM_Controller_tag{
    FILE                *err_out_fp;
    MEM_ErrorHandler    err_handler;
    MEM_FailMode        fail_mode;
    Header              *block_header;
};





/************ 内存存储块 ************/

typedef union{
    long    l_dummy;
    double  d_dummy;
    void*   v_dummy;
}Cell;

typedef struct MemoryPageList_tag  MemoryPageList;
#define CELL_SIZE (sizeof(Cell))
#define DEFAULT_PAGE_SIZE (1024) 

struct MemoryPageList_tag{
    int             cell_num;
    int             use_cell_num;
    MemoryPageList  *next;
    Cell            cell[0];        // 柔性数组
};

struct MEM_Storage_tag{
    MemoryPageList      *page_list;
    int                 page_size;      // 用来保证每次alloc的页大小不太小
};


#endif