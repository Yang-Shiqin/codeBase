# diff between `__str__` and `__repr__`
- `__str__`: focus on readable
- `__repr__`: focus on ambiguous

## 代码示例
1. python的内置函数repr()和str()
    - repr()就是通过`__repr__`这个特殊方法来得到object->str的
    - str()和print()都可以得到`__str__`

2. 同/关联
    - 都是生成一个对象的str表示
    - 当`__str__`缺省时, `__str__`会调用`__repr__` (str和repr输出相同)

3. 异(例子)
    - str用于用户创建输出
    - repr用于调试开发(尽量准确到可用于reproduce)
    ```py
    >>> x = 4
    >>> str(x)
    '4'
    >>> repr(x)
    '4'
    
    >>> x = '4'
    >>> str(x)
    '4'
    >>> repr(x)
    "'4'"
    
    >>> import datetime
    >>> d = datetime.datetime.now()
    >>> str(d)
    '2020-04-04 20:47:46.525245'
    >>> repr(d)
    'datetime.datetime(2020, 4, 4, 20, 47, 46, 525245)'
    ```

- 就是str()是把对象转换成str, repr()是用str把对象准确地表示出来
- print(obj)的时候是要把对象转换成str, 所以也是`__str__`

```py
class tmp():
    def __init__(self, name):
        self.name = name
    def __str__(self):
        return "str: "+self.name
    def __repr__(self):
        return "repr: "+self.name

a = tmp("lza")
print(tmp)
print(a)
print(str(a))
print(repr(a))
```
