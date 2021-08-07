#include "zal_in.h"
#include <stdarg.h>

static void format_msg(ErrorMsgFormat *format, VStr *string, va_list ap){
    // TODO
}




void zal_compile_error(CompileErrorType ceid, char *format, ...){
    // TODO

}


void zal_runtime_error(int line, RuntimeErrorType reid, char *format, ...){
    // TODO
}



// // TODO
// int
// yyerror(char const *str)
// {    return 0;
// }
