#!/usr/bin/env python
# coding=utf-8
import matplotlib.pyplot as plt
import torch

xx = torch.linspace(-6.5,-6,100)
yy = 3*xx**6+30.8*xx**5+127.4*xx**4+318.4*xx**3+531.2*xx**2+328.8*xx+96
plt.plot(xx, yy)
print(yy)
plt.savefig('./ysq.png')
