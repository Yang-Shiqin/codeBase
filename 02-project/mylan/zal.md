# 自制zal

时间: 2021/7/29

作者: ysq

---

无类型解释语言

- 无复杂指针(地址)
- 函数返回是个值
- bug: 数组循环引用比较会死循环

---

![命名规范](img\命名规范.png)

## ZAL模块

运行起来都是get_current_inter

### lex/yacc
- 终结符: 
  - **标识符**和**标点**(FUNCTION IF ELIF ELSE WHILE FOR RETURN CONTINUE BREAK NULL_T TRUE_T FALSE_T LP RP LC RC LB RB SEMICOLON COMMA LOGIC_AND LOGIC_OR ASSIGN EQ GE LE GT LT ADD SUB MUL DIV MOD INC DEC DOT)
  - **字面常量**(INT_LITERAL, DOUBLE_LITERAL, STRING_LITERAL)
  - **变量**
- 非终结符
  - **形参链**
  - **实参链**
  - **表达式**(ARRAY_LITERAL, expr(不能为空), expr_list(不算一种expr, 可空))(尽量不要让太底层的为空)
  - 表达式链(expr, expr, expr)
  - 语句
  - 语句块(语句链)
- 运算符优先级

#### 代码结构(通过栈的思想写完不容易漏)

```txt
script{
	func_def
	state
}
func_def{
	FUNCTION IDENTIFIER ( para_list ) block
	FUNCTION IDENTIFIER ( ) block
}
state{
	expr_lisr ;
	if_state
	for_state
	while_state
	return_state
	break_state				# 加标签啥的 容易扩展
	continue_state
}
para_list{
	IDENTIFIER
	para_list , IDENTIFIER
}
block{
	{ state_list }
	{ }
}
expr_list{
	/*null*/
	expr
	expr_list , expr
}
if_state{	# 强制要加花括号
	IF ( condition ) block
	IF ( condition ) block elsif
	IF ( condition ) block ELSE block
	IF ( condition ) block elsif ELSE block
}
for_state{
	FOR ( expr_list ; condition ; expr_list ) block
}
while_state{
	WHILE ( condition ) block
}
return_state{
	RETURN expr ;
}
break_state{
	BREAK ;
}
continue_state{
	CONTINUE ;
}
elsif{
	ELIF ( condition ) block
	elsif ELIF ( condition ) block
}
condition{		# 不能为空的expr_list
	expr
	expr_list , expr
}
state_list{
	state
	state_list state
}
# 下面的expr大头根据运算符优先级, 先低后高
expr{
	l_expr ASSIGN expr
	logical_or_expr
}
l_expr{
	IDENTIFIER
	l_expr [ expr ]
}
logical_or_expr{
	logical_and_expr
	logical_or_expr || logical_and_expr
}
logical_and_expr{
	eq_ne_expr
	logical_and_expr && eq_ne_expr
}
eq_ne_expr{
	compare_expr
	eq_ne_expr == compare_expr
	eq_ne_expr != compare_expr
}
compare_expr{		# 但一个表达式应该不可能有两个>?因为真假无法比较, 这里要写吗
					# (还是这里不算词法分析内容, 虽然不用语法分析依然能判断是错的
	add_sub_expr
	compare_expr > add_sub_expr
	compare_expr >= add_sub_expr
	compare_expr < add_sub_expr
	compare_expr <= add_sub_expr
}
add_sub_expr{
	math_expr
	add_sub_expr + math_expr
	add_sub_expr - math_expr
}
math_expr{
	minus_in_de_expr
	math_expr * minus_in_de_expr
	math_expr / minus_in_de_expr
	math_expr % minus_in_de_expr
}
minus_in_de_expr{	# 拆分成++/--比-优先级高(因为结合顺序从右到左, 我们只有右++/--
	pri_expr
	- pri_expr
	pri_expr ++
	pri_expr --
}
pri_expr{
	int
	string
	double
	array_literal
	IDENTIFIER(argv_list)
	IDENTIFIER()
	l_expr . IDENTIFIER ( argv_list )
	l_expr . IDENTIFIER ( )
	(expr)
	IDENTIFIER
	null
	true
	false
}
argv_list{
	expr
	argv_list , expr
}
array_literal{	# 元素不能为空, null也要填null
	{ expr_list }
	{ expr_list , }
}
```

#### 运算符注册流程

1. lex/yacc注册token, 补全运算符关系
2. create里创造该表达式
3. 补全该表达式宏
4. 计算过程
5. 运行过程

- 如?:  运算后赋值 位运算 前置自增自减
- TODO: expr_list很有问题

### enum

| 语句类型 | 变量类型 |
| -------- | -------- |
| 表达式   | int      |
| if       | double   |
| for      | char     |
| while    | string   |
| break    | bool     |
| continue | point    |
| return   | arr      |
|          | null     |

| 表达式类型           | 符号类型                   |
| -------------------- | -------------------------- |
| 常量                 | int                        |
| 变量                 | double                     |
| 赋值                 | bool                       |
| 双目数学+ - * / %    | string(char可以作为string) |
| 双目逻辑&& \|\| !    | null                       |
| 双目比较> == < >= <= | point                      |
| 单目-                | arr                        |
| 函数调用             |                            |
| 方法调用             |                            |

| 运行错误类型              | 错误信息                                           |
| ------------------------- | -------------------------------------------------- |
| VARIABLE_NOT_FOUND        | 未找到变量$(name)                                  |
| FUNCTION_NOT_FOUND        | 未找到函数$(name)                                  |
| METHOD_NOT_FOUND          | 对象没有方法$(name)                                |
| ARGUMENT_TOO_MANY         | 参数过多                                           |
| ARGUMENT_TOO_FEW          | 参数过少                                           |
| NOT_BOOL_TYPE             | 条件式必须是boolean型                              |
| MINUS_OPERAND_TYPE        | 减法运算符的操作数必须是数字类型                   |
| BINARY_OPERAND_TYPE       | 2项运算符$(operator)的操作数类型不正确             |
| NOT_BOOLEAN_OPERATOR      | $(operator)不能用于boolean型                       |
| FOPEN_ARG_TYPE            | 在fopen函数中输入文件路径和模式（都是字符串类型）  |
| FCLOSE_ARG_TYPE           | 在fclose函数中输入文件路径和模式（都是字符串类型） |
| FGETS_ARG_TYPE            | fgets函数接收文件指针                              |
| FPUTS_ARG_TYPE            | fputs函数接收文件指针和字符串                      |
| NOT_NULL_OPERATOR         | 对null只能应用==和!=运算符                         |
| DIVISION_BY_ZERO          | 除零                                               |
| NOT_STRING_OPERATOR       | $(operator)不能用于string型                        |
| NOT_LVALUE                | 操作数不能是左值                                   |
| INDEX_OPERAND_NOT_ARRAY   | 索引运算符左边不是数组类型                         |
| INDEX_OPERAND_NOT_INT     | 索引运算符中间不是int类型                          |
| ARRAY_INDEX_OUT_OF_BOUNDS | 超出数组范围                                       |
| NEW_ARRAY_ARG_TYPE        | new_array()方法接收int参数                         |
| INC_DEC_OPERAND_TYPE      | 自增自减运算符对象不是整形变量                     |
| RESIZE_ARG_TYPE           | resize()参数应为整形                               |

| 编译错误类型      | 错误信息                 |
| ----------------- | ------------------------ |
| PARSE             | ($(token))附近有语法错误 |
| CHARACTER_INVALID | 错误的字符($(bad_char))  |
| FUNC_MUT_DEF      | 函数名重复($(name))      |

| 语句返回类型 |      |
| ------------ | ---- |
| 一般语句     |      |
| break        |      |
| continue     |      |
| return       |      |

### 结构体

| 解释器                                                  |      |
| ------------------------------------------------------- | ---- |
| 解释器内存(MEM_Storage)(用来给解释器自己开空间, 给函数) |      |
| 运行时内存(MEM_Storage)(用来给新语言全局变量开空间)     |      |
| 全局变量(Variable)                                      |      |
| 函数(FuncDefList)                                       |      |
| 要运行的脚本语句链表(.zal)(StatementList)               |      |
| 编译时当前行(只运行一个脚本?)(先考虑此)                 |      |
| 栈(动态作用域?GC?)(我们改成不用声明global就能用吧)      |      |
| 堆(GC)(用来存对象的)                                    |      |
| top level local env                                     |      |

| 语句链表        | 语句               |
| --------------- | ------------------ |
| 语句(Statement) | 类型               |
| next            | 各种语句结构体enum |

| 表达式(Expression) | 变量 |
| ------------------ | ---- |
| 类型               | 类型 |
| 各种类型结构       | 值   |

| 栈           | 堆(对象)     |
| ------------ | ------------ |
| 总大小       | 总大小       |
| 用了多少     | 用了多少     |
| 指向栈的指针 | 指向堆的指针 |

| 语句结果                            |      |
| ----------------------------------- | ---- |
| 类型                                |      |
| 语句返回携带信息结构体(一般就value) |      |
|                                     |      |

### 函数

#### 栈

- 初始
- 压栈
- 出栈
- 只看不操作
- 栈收缩

#### 堆

- check_gc(每次创建对象前都检查是否超出阈值, 超出就gc释放,设置新阈值)
- gc(mark, sweep)
  - mark(所有对象都在堆里, 从全局局部和栈里判断是否可达, 标记)
  - free不可达的(堆(对象)是个双向链表)
- alloc_obj(alloc前会check_gc, alloc会加入heap里, 字面常量str不在堆里, 非字面在但不是直接加heap里, size++之后释放的时候判断非字面 然后从obj里释放)
- 

#### 计算(eval)

传参(inter用来压栈返结果, 有变量传env, 传expr里有用的)

- 表达式
  - 各种表达式
  - 方法调用

| +      | int  | double | bool | string | array | point | null |
| ------ | ---- | ------ | ---- | ------ | ----- | ----- | ---- |
| int    | i    | d      | i    | s      | x     | x     | x    |
| double | d    | d      | d    | s      | x     | x     | x    |
| bool   | i    | d      | i    | s      | x     | x     | x    |
| string | s    | s      | s    | s      | s     | s     | s    |
| array  | x    | x      | x    | s      | a     | x     | x    |
| point  | x    | x      | x    | s      | x     | x     | x    |
| null   | x    | x      | x    | s      | x     | x     | x    |

| -*/    | int  | double | bool | string | array | point | null |
| ------ | ---- | ------ | ---- | ------ | ----- | ----- | ---- |
| int    | i    | d      | i    | x      | x     | x     | x    |
| double | d    | d      | d    | x      | x     | x     | x    |
| bool   | i    | d      | i    | x      | x     | x     | x    |
| string | x    | x      | x    | x      | x     | x     | x    |
| array  | x    | x      | x    | x      | x     | x     | x    |
| point  | x    | x      | x    | x      | x     | x     | x    |
| null   | x    | x      | x    | x      | x     | x     | x    |

> %先只支持int和int

| == !=  | int  | double | bool | string | array | point | null |
| ------ | ---- | ------ | ---- | ------ | ----- | ----- | ---- |
| int    | b    | b      | b    | b      | b     | b     | b    |
| double | b    | b      | b    | b      | b     | b     | b    |
| bool   | b    | b      | b    | b      | b     | b     | b    |
| string | b    | b      | b    | b      | b     | b     | b    |
| array  | b    | b      | b    | b      | b     | b     | b    |
| point  | b    | b      | b    | b      | b     | b     | b    |
| null   | b    | b      | b    | b      | b     | b     | b    |

| ><     | int  | double | bool | string | array | point | null |
| ------ | ---- | ------ | ---- | ------ | ----- | ----- | ---- |
| int    | b    | b      | b    | x      | x     | x     | x    |
| double | b    | b      | b    | x      | x     | x     | x    |
| bool   | b    | b      | b    | x      | x     | x     | x    |
| string | x    | x      | x    | x      | x     | x     | x    |
| array  | x    | x      | x    | x      | x     | x     | x    |
| point  | x    | x      | x    | x      | x     | x     | x    |
| null   | x    | x      | x    | x      | x     | x     | x    |

#### 执行(execute)

- 语句
  - 各种语句

#### 创建(create)

- 表达式
- 语句
- 参数
- 变量名

用zal_alloc(解释器内存, 因为这个阶段是在解析)

#### str

- 字面常量

#### 内置函数(native)

```c
typedef ZAL_Value (* ZAL_NativeFuncProc) (ZAL_Interpreter* inter, ZAL_LocalEnvironment* env, int argc, ZAL_Value* argv);
// argc是eval的时候从list数出来的(list是解析的时候生成的
// argv是expr_list->value_list
```

- 注册内置函数
- 内置函数handler

| 内置函数               |
| ---------------------- |
| print(表达式)          |
| fopen(file_name, mode) |
| fclose(fp)             |
| fgets(fp, len)         |
| fget_line(fp)          |
| fputs(fp, expr)        |
| array(size, size, ...) |



#### interface

- create_interpreter(先创建storage再从里面拿出自己)
- compile
- interpret
- destroy_interpreter

#### error

- 运行时错误
- 解析时错误
- 不定参数$s $d替换

## MEM模块

### 结构体

| 内存控制块                                             | 内存储存块 |
| ------------------------------------------------------ | ---------- |
| 错误输出处                                             | 内存页     |
| 错误handler                                            | 页最小size |
| 错误解决(exit/return)                                  |            |
| 头(每一块内存分配信息(但没有指针))(所以头不直接管肚子) |            |

| 头           | 内存页             |
| ------------ | ------------------ |
| 分配内存大小 | 总大小             |
| file         | 用了多少           |
| line         | 下一页             |
| prev         | cell柔性数组(肚子) |
| next         |                    |

| MEM_alloc(直接malloc)(zal运行所用) | zal_alloc(MEM_storage_malloc(MEM_alloc))(.zal脚本所用) |
| ---------------------------------- | ------------------------------------------------------ |
| str                                | FunctionDefinition                                     |
| local_env                          | ParameterList                                          |
| alloc_obj                          | ArgumentList                                           |
| RefInNativeFunc                    | ExpressionList/Expression                              |
| array                              | StatementList/Statement/Block                          |
| stack                              | Elsif                                                  |
|                                    | IdentifierList                                         |
|                                    | crb_close_string_literal                               |

### 宏

- alloc(直接c堆上)
- realloc
- free
- dump_block(DBG)
- check_block(DBG)
- check_all_block(DBG)
- storage_alloc(解释器里)

## DBG模块

### 宏

(一般DBG用宏, 不要DBG模式容易去掉)

- 宏的#
- vprintf

# ==TODO==

- python一样的交互模式
- 更完善的DBG宏模块
- 循环引用bug
- 大括号(块代码)
- 检查溢出
- 1+0.5
- 字符串常量都打印不出来
