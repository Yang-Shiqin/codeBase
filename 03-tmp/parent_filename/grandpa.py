#!/usr/bin/env python
# coding=utf-8

from father import father_print

def grandpa_print():
    print('called grandpa_print..')
    father_print()

if __name__ == "__main__":
    grandpa_print()
