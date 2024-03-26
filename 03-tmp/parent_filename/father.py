#!/usr/bin/env python
# coding=utf-8

from son import son_print

def father_print():
    print('called father_print..')
    son_print()

if __name__ == "__main__":
    father_print()
