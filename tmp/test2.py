#!/usr/bin/env python
# coding=utf-8
import math
import torch
from torchvision.models import resnet18

model = resnet18(pretrained=True)	# 加载模型
optimizer = torch.optim.SGD(params=model.parameters(), lr=0.1)	# base_lr = 0.1

# 设置warm up的轮次为100次
warm_up_iter = 10
T_max = 50	# 周期
lr_max = 0.1	# 最大值
lr_min = 1e-5	# 最小值

# 为param_groups[0] (即model.layer2) 设置学习率调整规则 - Warm up + Cosine Anneal
lambda0 = lambda cur_iter: cur_iter / warm_up_iter if  cur_iter < warm_up_iter else \
        (lr_min + 0.5*(lr_max-lr_min)*(1.0+math.cos( (cur_iter-warm_up_iter)/(T_max-warm_up_iter)*math.pi)))/0.1

#  param_groups[1] 不进行调整

# LambdaLR
scheduler = torch.optim.lr_scheduler.LambdaLR(optimizer, lr_lambda=lambda0)

for epoch in range(200):
    print(optimizer.param_groups[0]['lr'])
    optimizer.step()
    scheduler.step()

