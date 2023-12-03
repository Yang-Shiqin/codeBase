# Dunder in python
> Dunder = double under(underscores), or magic methods

for examples:
- `__init__`
- `__add__`
- `__len__`
- `__repr__`

## 常用Dunder介绍

### `__init__` & `__new__`

- `__new__`用来**创建**实例, 静态方法, 需要return实例
- `__init__`用来**初始化**实例, 实例方法
- `__new__`return值(实例)传给`__init__`第一个参数    (?)

#### `__new__`的用处

当继承int/str/tuple等不可变的class时, `__new__`提供了自定义这些类的实例化过程的途径

```py
class PosInt1(int):
	def __init__(self, value):
		super(PosInt1, self).__init__()
		
class PosInt2(int):
	def __new__(cls, value):
		return super(PosInt2, cls).__new__(cls, abs(value))
		
		
p1 = PosInt1(-3)		# -3
p2 = PosInt2(-3)		# 3
```

==TODO==

new会覆盖init

### `__repr__` & `__str__`

[diff between \_\_repr\_\_and \_\_str\_\_](./str_repr_diff.md)

### `__add__`
重载运算符'+', 即:
    **`x+y` => `x.__add__(y)`** 

```py
class str1():
    def __init__(self, string):
        self.string = string

class str2():
    def __init__(self, string):
        self.string = string
    def __add__(self, string):
        return self.string+string

a = str1("ysq")
b = str2("ysq")
print(a+'lza')	# TypeError: unsupported operand type(s) for +
print(b+'lza')	# ysqlza
```



类似的还有`__sub__`, `__mul__`, `__matmul__`, `__truediv__`, `__floordiv__`, `__mod__`, `__divmod__`, `__pow__`, `__lshift__`, `__rshift__`, `__and__`, `__xor__`, `__or__`



## `__constants__`

==TODO==
