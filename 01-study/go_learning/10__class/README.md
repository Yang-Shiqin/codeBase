
面向对象特征：
- 封装
- 继承
    - 父类
    - 子类
- 多态
    - 基本要素
        - 有父类(接口)
        - 有子类(实现了父类的全部接口方法)
        - 父类类型的变量(指针)指向(引用)子类的具体数据变量: interface变量 = 子类struct对象
    - interface
        - 定义接口
        - 通用万能类型: `interface{}`, 空接口
            - int、string、float32、struct等都实现了 `interface{}`(类似object)
            - 就可以用 `interface{}` 类型引用任意的数据类型
            - 判断类型："类型断言"，`<interface{}>.(<type>)`，即 `<interface{}实体>.(<判断类型>)`


实现go面向对象：类、对象。通过struct实现类


