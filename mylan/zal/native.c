
#include <string.h>
#include "zal_in.h"

static ZAL_NativePointerInfo s_native_lib_info = {
    "zal.lang.file"
};

static void check_argc(int argc, int should_be){
    if(argc<should_be){
        zal_runtime_error(0, ARGUMENT_TOO_FEW_ERR); // 实参太少
    }else if(argc>should_be){
        zal_runtime_error(0, ARGUMENT_TOO_MANY_ERR); // 实参太多
    }
}

ZAL_Value zal_nv_print(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    char *to_be_printed=NULL;
    check_argc(argc, 1);    // print一个zal_value(计算简化后)
    to_be_printed = zal_value_to_str(argv);
    printf("%s", to_be_printed);
    MEM_free(to_be_printed);
    ret.type = ZAL_NULL_VALUE;
    return ret;
}
ZAL_Value zal_nv_fopen(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    FILE *fp=NULL;
    check_argc(argc, 2);
    if(!(argv[0].type==ZAL_STRING_VALUE && argv[1].type==ZAL_STRING_VALUE)){
        zal_runtime_error(0, FOPEN_ARG_TYPE_ERR);
    }
    fp = fopen(argv[0].u.object->u.string.string, argv[1].u.object->u.string.string);
    if(fp){
        ret.u.pointer.pointer = fp;
        ret.u.pointer.info=&s_native_lib_info;
        ret.type = ZAL_NATIVE_POINTER_VALUE;
    }else{
        ret.type = ZAL_NULL_VALUE;  
    }
    return ret;
}
int check_native_pointer_info(ZAL_NativePointerInfo *info){
    return info==&s_native_lib_info;
}
ZAL_Value zal_nv_fclose(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    FILE *fp=NULL;
    ZAL_Value ret;
    ret.type = ZAL_NULL_VALUE;
    check_argc(argc, 1);
    if(argv[0].type==ZAL_NATIVE_POINTER_VALUE && check_native_pointer_info(argv[0].u.pointer.info)){
        fp = argv[0].u.pointer.pointer;
        fclose(fp);
    }else{
        zal_runtime_error(0, FCLOSE_ARG_TYPE_ERR);
    }
    return ret;
}
ZAL_Value zal_nv_fgets(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    char *str=NULL;
    FILE *fp=NULL;
    check_argc(argc, 2);
    if(!(argv[0].type==ZAL_NATIVE_POINTER_VALUE && check_native_pointer_info(argv[0].u.pointer.info)\
     && argv[1].type==ZAL_INT_VALUE)){
        zal_runtime_error(0, FGETS_ARG_TYPE_ERR);
    }

    if(argv[1].u.int_value==0){
        ret.type = ZAL_NULL_VALUE;
    }else{
        fp = argv[0].u.pointer.pointer;
        str = MEM_alloc(argv[1].u.int_value+1);
        fgets(str, argv[1].u.int_value, fp);
        ret.u.object = zal_env_non_literal_to_string(inter, env, str);
    }
    return ret;
}
ZAL_Value zal_nv_fget_line(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    char buf[LINE_BUF_SIZE];
    int len=0;
    char *str=NULL;
    FILE *fp=NULL;
    check_argc(argc, 1);
    if(!(argv[0].type==ZAL_NATIVE_POINTER_VALUE && check_native_pointer_info(argv[0].u.pointer.info))){
        zal_runtime_error(0, FGET_LINE_ARG_TYPE_ERR);
    }

    fp = argv[0].u.pointer.pointer;
    while(fgets(buf, LINE_BUF_SIZE, fp)){
        len += strlen(buf);
        str = MEM_realloc(str, len+1);
        if(len){
            strcat(str, buf);
        }else{
            strcpy(str, buf);
        }
        if(str[len-1]=='\n') break;
    }
    if(len){
        ret.type = ZAL_STRING_VALUE;
        ret.u.object = zal_env_non_literal_to_string(inter, env, str);
    }else{
        ret.type = ZAL_NULL_VALUE;
    }
    return ret;
}
ZAL_Value zal_nv_fputs(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    FILE *fp=NULL;
    char *str=NULL;
    ZAL_Value ret;
    ret.type = ZAL_NULL_VALUE;
    check_argc(argc, 2);
    if(!(argv[0].type==ZAL_NATIVE_POINTER_VALUE && check_native_pointer_info(argv[0].u.pointer.info))){
        zal_runtime_error(0, FPUTS_ARG_TYPE_ERR);
    }
    fp = argv[0].u.pointer.pointer;
    str = zal_value_to_str(&argv[1]);
    fputs(str, fp);
    MEM_free(str);
    return ret;
}

ZAL_Value create_array_sub(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv, int dimension){
    if(argv[dimension].type != ZAL_INT_VALUE){
        zal_runtime_error(0, NEW_ARRAY_ARG_TYPE_ERR);
    }
    ZAL_Value ret;
    int size = argv[dimension].u.int_value;
    int i;
    ret.type = ZAL_ARRAY_VALUE;
    ret.u.object = zal_env_create_array_obj(inter, env, size);  // 加入local env防止被GC释放
    for(i=0; i<size; i++){
        if(dimension==argc-1){
            ret.u.object->u.array.array[i].type = ZAL_NULL_VALUE;
        }else{
            ret.u.object->u.array.array[i] = create_array_sub(inter, env, argc, argv, dimension+1);
        }
    }
    return ret;
}
// 创建数组
ZAL_Value zal_nv_array(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    if(argc<1){
        zal_runtime_error(0, ARGUMENT_TOO_FEW_ERR);
    }
    ret = create_array_sub(inter, env, argc, argv, 0);
    return ret;
}
void zal_add_std_fp(ZAL_Interpreter* inter){
    ZAL_Value fp;
    VariableList *var;
    fp.type = ZAL_NATIVE_POINTER_VALUE;
    fp.u.pointer.info = &s_native_lib_info;
    fp.u.pointer.pointer = stdin;
    var = zal_add_global_variable(inter, "STDIN");
    var->value = fp;
    fp.u.pointer.pointer = stdout;
    var = zal_add_global_variable(inter, "STDOUT");
    var->value = fp;
    fp.u.pointer.pointer = stderr;
    var = zal_add_global_variable(inter, "STDERR");
    var->value = fp;
}