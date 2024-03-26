#!/usr/bin/env python
# coding=utf-8
import sys
import os

def son_print():
    cur_frame = sys._getframe()
    cur_file = os.path.basename(cur_frame.f_code.co_filename)
    back_frame = cur_frame.f_back
    back_file = os.path.basename(back_frame.f_code.co_filename)
    last_frame = cur_frame
    while last_frame.f_back:
        last_frame = last_frame.f_back
    last_file = os.path.basename(last_frame.f_code.co_filename)

    print('called son_print..')
    print('cur: ', cur_file, ', back: ', back_file, ', last: ', last_file)

if __name__ == "__main__":
    son_print()
