# Dunder in python
> Dunder = double under(underscores), or magic methods

for examples:
- `__init__`
- `__add__`
- `__len__`
- `__repr__`

## 常用Dunder介绍
### `__repr__` & `__str__`

[diff between \_\_repr\_\_and \_\_str\_\_](./str_repr_diff.md)

### `__add__`
重载运算符'+', 即:
    **`x+y` => `x.__add__(y)`** 

类似的还有

- `__sub__`
- `__mul__`
- `__matmul__`
- `__truediv__`
- `__floordiv__`
- `__mod__`
- `__divmod__`
- `__pow__`
- `__lshift__`
- `__rshift__`
- `__and__`
- `__xor__`
- `__or__`
