#!/usr/bin/env python
# coding=utf-8
import torch
from torch.autograd import Variable

# 定义自己的网络
class TwoLayerNet(torch.nn.Module): # 继承nn.Module基类
	def __init__(self, D_in, H, D_out):
		super(TwoLayerNet, self).__init__() # 初始化超类
		self.linear1 = torch.nn.Linear(D_in, H)
		self.linear2 = torch.nn.Linear(H, D_out)

	# 前向传播
	def forward(self, x):
		h_relu = self.linear1(x).clamp(min=0)
		y_pred = self.linear2(h_relu)
		return y_pred

# 设置数据
N, D_in, H, D_out = 64, 1000, 100, 10
x = Variable(torch.randn(N, D_in))
y = Variable(torch.randn(N, D_out), requires_grad = False)

# 设置模型
model = TwoLayerNet(D_in, H, D_out)

# 设置loss准则
criterion = torch.nn.MSELoss(size_average = False)
# 设置优化器
optimizer = torch.optim.SGD(model.parameters(), lr=1e-4)
# 训练
for t in range(500):
	y_pred = model(x)            # 预测
	loss = criterion(y_pred, y)  # 计算loss
	print(loss)
	optimizer.zero_grad()        # 清零梯度（这样多个batch的梯度就不会叠加了，每次只用自己这步算的梯度）
	loss.backward()              # 反向传播计算梯度
	optimizer.step()             # 优化
