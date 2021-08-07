#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "debug.h"
#include "MEM.h"

static DGB_Controller   *s_current_dbg_controller;
static char             *s_current_dbg_file;
static int              s_current_dbg_line;
static char             *s_assert_condition;

static DGB_Controller s_default_dbg_controller={
    NULL,
    INT_MAX,
};


DGB_Controller *dbg_default_controller = &s_default_dbg_controller;

void DGB_set(DGB_Controller *ctrl, char *fp, int line){
    s_current_dbg_controller = ctrl;
    s_current_dbg_file = fp;
    s_current_dbg_line = line;
    if(ctrl->err_out_fp==NULL){
        ctrl->err_out_fp=stderr;
    }
}

void DGB_set_condition(char *condition){
    s_assert_condition = condition;
}

DGB_Controller* DBG_create_controller_func(void){
    DGB_Controller *ret = MEM_alloc(sizeof(DGB_Controller));
    ret->err_out_fp = NULL;
    ret->level = INT_MAX;
    return ret;
}
void DBG_set_err_out_fp_func(DGB_Controller *ctrl, FILE *err_out_fp){
    ctrl->err_out_fp = err_out_fp;
}
void DBG_set_err_level_func(DGB_Controller *ctrl, int level){
    ctrl->level = level;
}


void DBG_assert_func(char *format, ...){
    va_list ap;
    va_start(ap, format);
    fprintf(s_current_dbg_controller->err_out_fp, "file: %s, line: %d. Assertion failure (%s)\n", \
        s_current_dbg_file, s_current_dbg_line, s_assert_condition);
    if (format){
        vfprintf(s_current_dbg_controller->err_out_fp, format, ap);
    }
    va_end(ap);
    abort();
}

void DBG_panic_func(char *format, ...){
    va_list ap;
    va_start(ap, format);
    if (format){
        vfprintf(s_current_dbg_controller->err_out_fp, format, ap);
    }
    va_end(ap);
    abort();
}