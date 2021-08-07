#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "zal_in.h"

extern ErrorMsgFormat zal_compile_error_msg_format[];
extern ErrorMsgFormat zal_runtime_error_msg_format[];


static void format_msg(ErrorMsgFormat *format, VStr *string, va_list ap){
    int i;
    char buf[LINE_BUF_SIZE];
    for(i=0; i<strlen(format->format); i++){
        if(format->format[i]=='$'){
            i++;
            switch (format->format[i])
            {
            case 'd':
                sprintf(buf, "%d", va_arg(ap, int));
                zal_vstr_append_string(string, buf);
                break;
            case 'c':
                zal_vstr_append_ch(string, (char)va_arg(ap, int));
            case 's':
                zal_vstr_append_string(string, va_arg(ap, char*));
            default:
                break;
            }
        }else{
            zal_vstr_append_ch(string, format->format[i]);
        }
    }
}




void zal_compile_error(CompileErrorType ceid, ...){
    VStr message;
    va_list ap;
    ZAL_Interpreter *inter=zal_get_current_inter();
    message.string=NULL;
    va_start(ap, ceid);
    format_msg(&zal_compile_error_msg_format[ceid], &message, ap);
    fprintf(stderr, "%6d : %s\n", inter->line_number, message.string);
    va_end(ap);
    exit(1);
}


void zal_runtime_error(int line, RuntimeErrorType reid, ...){
    VStr message;
    va_list ap;
    message.string=NULL;
    va_start(ap, reid);
    format_msg(&zal_runtime_error_msg_format[reid], &message, ap);
    fprintf(stderr, "%6d : %s\n", line, message.string);
    va_end(ap);
    exit(1);
}

