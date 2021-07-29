#ifndef __MEM_H__
#define __MEM_H__
#define "MEM/memory.h"


#ifdef MEM_CONTROLLER 
#define MEM_CURRENT_CONTROLLER MEM_CONTROLLER
#else
#define MEM_CURRENT_CONTROLLER mem_default_controller





#define MEM_alloc(size) (mem_alloc_func(MEM_CURRENT_CONTROLLER, size, __FILE__, __LINE__))



#endif