#!/usr/bin/env python
# coding=utf-8
import multiprocessing as mp

def f(x):
    return x, x*x

pool = mp.Pool()
fit_arr = pool.map(f, [1,2,3])
print(fit_arr)
