#!/usr/bin/env python
# coding=utf-8
import warnings
warnings.filterwarnings("error")        # warning处理成error

def func(n):
    if n < 0:
        warnings.warn('n<0')

func(0)
try:
    func(-1)
except Exception as e:
    print(e.args)
print('touch here')
