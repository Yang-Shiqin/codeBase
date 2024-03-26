#!/usr/bin/env python
# coding=utf-8

def func(a):
    return a+1

if __name__ == "__main__":
    a = 123
    b = func(a)
    c = func(func(a))
