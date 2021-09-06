#!/usr/bin/env python
# coding=utf-8
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
