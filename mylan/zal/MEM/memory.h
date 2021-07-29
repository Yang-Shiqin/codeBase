#ifndef __MEMORY_H__
#define __MEMORY_H__
#include "zal_in.h"
#include <stdio.h>

typedef struct MEM_Controller_tag{
    FILE* out;

}MEM_Controller;




void* mem_alloc_func(MEM_Controller* ctrl, int size, char* file, int line);

void* mem_realloc_func(MEM_Controller* ctrl, void* old_alloc, int size, char* file, int line);



#endif