#!/usr/bin/env python
# coding=utf-8
import fire

def add(a, b):
    print(a+b)
    return a+b

if __name__ == "__main__":
    fire.Fire(add)
