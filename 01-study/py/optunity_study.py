#!/usr/bin/env python
# coding=utf-8
import optunity
import optunity.metrics
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0,1,100)
y = x*10+20+np.random.rand(100)

def para(k, b):
    haty = x*k+b
    loss = sum((haty-y)**2)
    return loss

plt.plot(x, y)
print(y)
search_space = {'k':[0, 100], 'b':[-20, 100]}
optimal_configuration, info, _ = optunity.minimize(para, num_evals=1000, **search_space)
best_k = optimal_configuration['k']
best_b = optimal_configuration['b']
print(optimal_configuration)
best_performance = info.optimum
print(best_k, best_b, best_performance)
haty = best_k*x+best_b
plt.plot(x, haty)
plt.savefig('tmp.png')
