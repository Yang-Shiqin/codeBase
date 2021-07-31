

MessageFormat zal_compile_error_msg_format[] = {
    {"dummy"},
    {"($(token))附近有语法错误"},
    {"错误的字符($(bad_char))"},
    {"函数名重复($(name))"},
    {"dummy"}
}

MessageFormat zal_runtime_error_msg_format[] = {
    {"dummy"},
    {"未找到变量$(name)"},
    {"未找到函数$(name)"},
    {"对象没有方法$(name)"},
    {"参数过多"},
    {"参数过少"},
    {"条件式必须是boolean型"},
    {"减法运算符的操作数必须是数字类型"},
    {"2项运算符$(operator)的操作数类型不正确"},
    {"$(operator)不能用于boolean型"},
    {"在fopen函数中输入文件路径和模式（都是字符串类型）"},
    {"在fclose函数中输入文件路径和模式（都是字符串类型）"},
    {"fgets函数接收文件指针"},
    {"fputs函数接收文件指针和字符串"},
    {"对null只能应用==和!=运算符"},
    {"除零"},
    {"$(operator)不能用于string型"},
    {"操作数不能是左值"},
    {"索引运算符左边不是数组类型"},
    {"索引运算符中间不是int类型"},
    {"超出数组范围"},
    {"new_array()方法接收int参数"},
    {"自增自减运算符对象不是整形变量"},
    {"resize()参数应为整形"},
    {"dummy"}
};