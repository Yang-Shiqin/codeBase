
#include "zal_in.h"

static ZAL_NativePointerInfo s_native_lib_info = {
    "zal.lang.file"
};

static void check_argc(int line, int argc, int should_be){
    if(argc<should_be){
        zal_runtime_error(line, ARGUMENT_TOO_FEW_ERR); // 实参太少
    }else if(argc>should_be){
        zal_runtime_error(line, ARGUMENT_TOO_MANY_ERR); // 实参太多
    }
}

ZAL_Value zal_nv_print(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    char *to_be_printed=NULL;
    check_argc(inter->line_number, argc, 1);    // print一个zal_value(计算简化后)
    to_be_printed = zal_value_to_str(argv);
    printf("%s", to_be_printed);
    MEM_free(to_be_printed);
    ret.type = ZAL_NULL_VALUE;
    return ret;
}
ZAL_Value zal_nv_scan(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    ZAL_Value ret;
    return ret;
    // TODO
}
ZAL_Value zal_nv_fopen(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    // TODO
    ZAL_Value ret;
    return ret;
}
ZAL_Value zal_nv_fclose(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    // TODO
    ZAL_Value ret;
    return ret;
}
ZAL_Value zal_nv_fgets(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    // TODO
    ZAL_Value ret;
    return ret;
}
ZAL_Value zal_nv_fputs(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    // TODO
    ZAL_Value ret;
    return ret;
}
ZAL_Value zal_nv_array(ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv){
    // TODO
    ZAL_Value ret;
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