#!/usr/bin/env python
# coding=utf-8
class foo():
    __constants__ = ['a']
    def __init__(self):
        self.a = 1
    def forward(self):
        self.a = self.a+1
        return self.a

b = foo()
print(b.forward())
