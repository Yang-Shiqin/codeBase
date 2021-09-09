#!/usr/bin/env python
# coding=utf-8
class PosInt1(int):
	def __init__(self, value):
		super(PosInt1, self).__init__()


class PosInt2(int):
	def __new__(cls, value):
		return super(PosInt2, cls).__new__(cls, abs(value))
		
p2 = PosInt2(-3)
p = PosInt1(-3)
print(p2, p)
