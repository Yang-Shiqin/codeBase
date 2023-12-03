#!/usr/bin/env python
# coding=utf-8
import math
import matplotlib.pyplot as plt 
warm_up_iter = 5
lr_min = 1e-7
lr_max = 1e-3
T_max = 100

lambda0 = lambda cur_iter: cur_iter / warm_up_iter if  cur_iter < warm_up_iter else \
            (lr_min + 0.5*(lr_max-lr_min)*(1.0+math.cos( (cur_iter-warm_up_iter)/(T_max-warm_up_iter)*math.pi)))/0.1
b = [lambda0(i) for i in range(1000)]
print(b)
plt.plot([i for i in range(1000)], b)
plt.savefig('./test2.jpg')
