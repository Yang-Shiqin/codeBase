def singleton(cls):   # cls是类而不是对象
    _instance = {}

    def inner():
        if cls not in _instance:      # 没有类
            _instance[cls] = cls()    # 则创建一个对象，{类:对象}
        return _instance[cls]
    return inner
    
@singleton
class Cls(object):
    def __init__(self):
        pass

cls1 = Cls()   # singleton(Cls)(), 即inner()
cls2 = Cls()
print(id(cls1) == id(cls2))    # True
