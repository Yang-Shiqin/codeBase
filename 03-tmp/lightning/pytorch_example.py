#!/usr/bin/env python
# coding=utf-8
# 导入包
import os
import torch
from torch import optim, nn, utils
from torchvision.datasets import MNIST
from torchvision import transforms
import torch.nn.functional as F
from torch.utils.data.dataloader import DataLoader

# 定义一个线性层和激活函数组成的编码器-解码器，你可以使用任何nn.Module创建的模块
encoder = nn.Sequential(nn.Linear(28 * 28, 64), nn.ReLU(), nn.Linear(64, 3))
decoder = nn.Sequential(nn.Linear(3, 64), nn.ReLU(), nn.Linear(64, 28 * 28))

# if global_rank == 0:
#     mnist_train = MNIST(os.getcwd(), train=True, download=True)

transform=transforms.Compose([transforms.ToTensor(),
                             transforms.Normalize(0.5, 0.5)])
mnist_train = MNIST(os.getcwd(), train=True, download=True, transform=transform)

mnist_train, mnist_val = utils.data.random_split(mnist_train, [55000, 5000])

mnist_train = DataLoader(mnist_train, batch_size=64)
mnist_val = DataLoader(mnist_val, batch_size=64)

params = [encoder.parameters(), decoder.parameters()]
optimizer = torch.optim.Adam(params, lr=1e-3)

model.train()
num_epochs = 1
for epoch in range(num_epochs):
    for train_batch in mnist_train:
        x, y = train_batch
        x = x.view(x.size(0), -1)
        z = encoder(x)
        x_hat = decoder(z)
        # 计算损失
        loss = nn.functional.mse_loss(x_hat, x)
        print('train loss: ', loss.item())

        loss.backward()
        optimizer.step()
        optimizer.zero_grad()

    model.eval()
    with torch.no_grad():
        val_loss = []
        for val_batch in mnist_val:
            x, y = val_batch
            x = x.view(x.size(0), -1)
            z = encoder(x)
            x_hat = decoder(z)
            loss = F.mse_loss(x_hat, x)
            val_loss.append(loss)
        val_loss = torch.mean(torch.tensor(val_loss))
        model.train()