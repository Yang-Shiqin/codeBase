#ifndef __DBG_H__
#define __DBG_H__

typedef struct DGB_Controller_tag   DGB_Controller;


extern DGB_Controller *dbg_default_controller; // 默认dbg_controller


#ifdef DBG_NO_DEBUG
#define DBG_create_controller()         ((void)0)
#define DBG_set_err_out_fp(fp)          ((void)0)
#define DBG_set_err_level(level)        ((void)0)
#define DBG_assert(condition, vprint)   ((void)0)
#define DBG_panic(vprint)               ((void)0)

#else
#ifdef DBG_CONTROLLER 
#define DBG_CURRENT_CONTROLLER DBG_CONTROLLER
#else
#define DBG_CURRENT_CONTROLLER dbg_default_controller
#endif

void DGB_set(DGB_Controller *ctrl, char *fp, int line);
void DGB_set_condition(char *condition);
DGB_Controller* DBG_create_controller_func(void);
void DBG_set_err_out_fp_func(DGB_Controller *ctrl, FILE *err_out_fp);
void DBG_set_err_level_func(DGB_Controller *ctrl, int level);

void DBG_assert_func(char *format, ...);
void DBG_panic_func(char *format, ...);


#define DBG_create_controller() (DBG_create_controller_func())
#define DBG_set_err_out_fp(fp) (DBG_set_err_out_fp_func(DBG_CURRENT_CONTROLLER, (fp)))
#define DBG_set_err_level(level) (DBG_set_err_level_func((DBG_CURRENT_CONTROLLER, (level)))
#define DBG_assert(condition, vprint) (\
    (condition) ? (void)(0) : (\
    DGB_set(DBG_CURRENT_CONTROLLER, __FILE__, __LINE__),\
    DGB_set_condition(#condition),\
    DBG_assert_func vprint))
#define DBG_panic(vprint) (DGB_set(DBG_CURRENT_CONTROLLER, __FILE__, __LINE__),DBG_panic_func vprint)
#endif

#endif