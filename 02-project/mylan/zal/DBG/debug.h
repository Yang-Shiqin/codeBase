#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include "../DBG.h"

struct DGB_Controller_tag{
    FILE    *err_out_fp;
    int     level;
};

#endif