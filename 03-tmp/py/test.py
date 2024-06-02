class MyClass:
    def __init__(self):
        self.__private_var = 42

    def _private_method(self):
        print("This is a private method")

    def public_method(self):
        self.__private_method()
        print(self.__private_var)

# 实例化对象
obj = MyClass()

# 调用公共方法
print(obj.__dir__())
#
## 直接访问私有方法和属性会报错
## obj.__private_method()  # AttributeError
## print(obj.__private_var)  # AttributeError
#
## 通过名称重整访问私有成员
#obj._MyClass__private_method()
#print(obj._MyClass__private_var)
#
