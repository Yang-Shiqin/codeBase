#ifndef __MEM_H__
#define __MEM_H__
#include "MEM/memory.h"

typedef MEM_Storage_tag     MEM_Storage;


#ifdef MEM_CONTROLLER 
#define MEM_CURRENT_CONTROLLER MEM_CONTROLLER
#else
#define MEM_CURRENT_CONTROLLER mem_default_controller





#define MEM_alloc(size) (mem_alloc_func(MEM_CURRENT_CONTROLLER, size, __FILE__, __LINE__))
#define MEM_realloc(old_ptr, size) (mem_realloc_func(MEM_CURRENT_CONTROLLER, old_ptr, size, __FILE__, __LINE__))
#define MEM_free(ptr) (mem_free_func(MEM_CURRENT_CONTROLLER, ptr, __FILE__, __LINE__))

#endif