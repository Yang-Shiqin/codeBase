#!/usr/bin/env python
# coding=utf-8
import collections
import collections.abc
import numpy as np
def _ntuple(n):
	def parse(x):
		if isinstance(x, collections.abc.Iterable):
			return x
		return tuple(np.repeat(x, n))
	return parse
to_1tuple = _ntuple(1)
to_2tuple = _ntuple(2)
to_3tuple = _ntuple(3)
to_4tuple = _ntuple(4)
to_ntuple = _ntuple


print(to_2tuple(222))
print(to_2tuple((222, 21)))
print(to_2tuple((222, 21,23)))
print(to_4tuple((222)))
