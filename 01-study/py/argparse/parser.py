#!/usr/bin/env python
# coding=utf-8
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--name', help='your name. default: str("ysq")', type=str, default='ysq')
args = parser.parse_args()
print('hello, '+args.name)
