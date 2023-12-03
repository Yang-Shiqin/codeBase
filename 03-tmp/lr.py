#!/usr/bin/env python
# coding=utf-8
import torch
import math
from torch.optim.lr_scheduler import _LRScheduler, CosineAnnealingWarmRestarts
from torchvision.models import resnet18
import matplotlib.pyplot as plt

model = resnet18(pretrained=True)
optimizer = torch.optim.AdamW(params = model.parameters(), lr=0.01)
T_0 = 2
T_mult = 2
scheduler = CosineAnnealingWarmRestarts(optimizer, T_0=T_0, T_mult= T_mult, eta_min=0.001)
lr = []
for epoch in range(500):
    lr.append(optimizer.param_groups[0]['lr'])
    optimizer.step()
    scheduler.step()
plt.plot([i for i in range(500)], lr)
plt.savefig('./test.png')
