#!/usr/bin/env python
# coding=utf-8
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim

from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms

"""tldr
1. model
    1. 继承nn.Module
    2. 定义网络结构
    3. 定义前向传播
2. data
    1. 下载数据集
    2. 数据预处理
    3. 划分数据集
    4. 生成DataLoader
3. train/val
    - 组成
        1. model
        2. data
        3. optimizer
        4. loss
    - 过程
        1. for epoch(每轮迭代整个数据集)
        2. for batch(每批迭代数据集的一块)
        3. forward
        4. cal loss
        5. loss.backward()(计算权重)
        6. optimizer.step()(更新权重)
"""

class LeNet(nn.Module):     # 继承pytorch网络基类(这个是所有网络的基类)
    def __init__(self, Cin, H, W, K):
        """初始化LeNet网络结构和权重
        参数: 
            Cin: 输入样本通道数
            H: 输入样本高度
            W: 输入样本宽度
            K: 输出分类数
        """
        super().__init__()  # 初始化基类
        self.net = nn.Sequential(   # 有序容器，存储网络层，顺序执行
            nn.Conv2d(in_channels=Cin, out_channels=6, kernel_size=5, stride=1, padding=2), # (N,Cin,H,W)->(N,6,H,W)
            nn.AvgPool2d(kernel_size=2, stride=2, padding=0),   # ->(N,6,H/2,W/2)
            nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5, stride=1, padding=0),  # ->(N,16,H/2-4,W/2-4)
            nn.Sigmoid(),
            nn.AvgPool2d(kernel_size=2, stride=2, padding=0),   # ->(N,16,H/4-2,W/4-2)
            nn.Flatten(),                                       # ->(N,16*(H/4-2)*(W/4-2))
            nn.Linear(int(16*(H/4-2)*(W/4-2)), 120),            # ->(N,120)
            nn.Dropout(0.3),
            nn.Sigmoid(),
            nn.Linear(120, 64),                                 # ->(N,64)
            nn.Sigmoid(),
            nn.Linear(64, K),                                   # ->(N,K)
            nn.Softmax(),
        )
        # 初始化网络层参数
        for m in self.net:
            if isinstance(m, nn.Linear):
                nn.init.xavier_normal_(m.weight)
                nn.init.constant_(m.bias, 0)
            elif isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, a=0, mode='fan_out', nonlinearity='relu')
                nn.init.constant_(m.bias, 0)
            elif isinstance(m, (nn.BatchNorm2d, nn.GroupNorm)):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)

    def forward(self, x):   # nn.Module的子类必须重写forward函数
        """前向传播，计算输入样本每个分类得分
        参数:
            x: 输入样本, 大小为(N, Cin, H, W)
        输出:
            score: 分类得分, 大小为(N, K)
        """
        x = x.float()
        score = self.net(x)
        return score

    def predict(self, x):   # 自己的函数
        """预测输入样本类别
        参数:
            x: 输入样本, 大小为(N, Cin, H, W)
        输出:
            y: 预测类别, y in [0, .., K-1]
        """
        score = self.forward(x)
        y = torch.argmax(score, dim=1)
        return y

def get_mnist(batch_size_train=64, batch_size_test=64):
    """获取mnist数据
        参数:
            batch_size_train: 训练和验证的批大小
            batch_size_test: 测试的批大小
        输出:
            loader_train: 训练集载入器
            loader_valid: 验证集载入器
            loader_test: 测试集载入器
    """
    # 数据预处理
    transform = transforms.Compose([
        transforms.ToTensor(),  # 转换成tensor
        transforms.Normalize([0.1307],[0.3081]),    # 标准化, 0.1307和0.3081分别是mnist的均值和标准差
    ])

    # 1. 下载数据集
    # 设置dataset
    train_val_dataset = datasets.MNIST(
        root = "./data/",
        transform=transform,    # 2. 数据预处理
        train = True,
        download = True
    )
    test_dataset = datasets.MNIST(
        root="./data/",
        transform = transform,
        train = False
    )

    # 3. 划分数据集为训练集和验证集(和测试集)
    # 将训练集划分为训练集和验证集
    train_dataset, val_dataset = random_split(
        train_val_dataset, 
        [len(train_val_dataset)-int(len(train_val_dataset)//6), int(len(train_val_dataset)//6)]
    )

    # 4. 生成DataLoader
    loader_train = DataLoader(
        dataset=train_dataset,
        batch_size = batch_size_train,
        shuffle = True
    )
    loader_valid = DataLoader(
        dataset=val_dataset,
        batch_size = batch_size_train,
        shuffle = True
    )
    loader_test = DataLoader(
        dataset=test_dataset,
        batch_size = batch_size_test,
        shuffle = True
    )

    return loader_train, loader_valid, loader_test

def acc(model, device, loader):
    """计算模型在数据集上的准确率
    参数:
        model: 网络模型
        device: 设备
        loader: 数据集载入器
    输出:
        rate: 准确率
    """
    model.eval()    # 推理模式
    correct_point = 0
    all_point = 0
    # 不计算梯度
    with torch.no_grad():
        for x, y in loader:
            x = x.float()
            y = y.long()
            x, y = x.to(device), y.to(device)
            pred_y = model.predict(x)
            correct_point += (pred_y==y).sum()
            all_point += pred_y.size(0)
        rate = correct_point.float()/all_point
        print('Got %d / %d correct (%.2f)' % (correct_point, all_point, 100 * rate))
    return rate

def train(model, criterion, optimizer, device, loader_train, lr_decay=None, 
            loader_valid=None, epoch=1):
    """训练模型
    参数:
        model: 网络模型
        criterion: 评判准则, 计算loss用
        optimizer: 优化器
        device: 设备
        loader_train: 训练集载入器
        lr_decay: 学习率衰减率, None为不衰减
        loader_valid: 验证集载入器
        epoch: 训练轮数
    输出:

    """
    loss = 0
    loss_history = torch.zeros(epoch)
    acc_history = torch.zeros(epoch)
    acc_valid_history = torch.zeros(epoch)
    iter_history = torch.zeros(epoch)
    if lr_decay != None:
        scheduler = optim.lr_scheduler.LambdaLR(optimizer, lr_lambda=lambda ep: lr_decay**ep)
    for e in range(epoch):
        for x, y in loader_train:
            model.train()   # 训练模式(用dropout和BN)
            x = x.float()
            y = y.long()
            x, y = x.to(device), y.to(device)
            score = model(x)    # 前向传播预测得分
            loss = criterion(score, y)  # 计算loss

            optimizer.zero_grad()   # 模型所有参数的梯度设为0
            loss.backward()     # 反向传播计算梯度
            optimizer.step()    # 根据梯度更新参数

        tt = (e+1)*len(loader_train)-1
        if lr_decay != None:
            scheduler.step()    # 更新学习率
            print("epoch %d learn rate: %f" % (e, optimizer.param_groups[0]['lr']))
        print('Epoch %d, Iteration %d, loss = %.4f' % (e, tt, loss.item()))
        loss_history[e] = loss.item()
        iter_history[e] = tt
        acc_history[e] = acc(model, device, loader_train)
        if loader_valid != None:
            acc_valid_history[e] = acc(model, device, loader_valid)

    if loader_valid != None:
        return loss_history, iter_history, acc_history, acc_valid_history
    else:
        return loss_history, iter_history, acc_history

def myshow(loss_history, iter_history, acc_history, acc_valid_history=None):
    fig = plt.figure()
    ax = fig.add_subplot(211)
    ax.plot(iter_history, loss_history, 'c-', label='train loss')
    ax.set_ylabel(r"loss")
    ax.legend(loc="best")
    ax = fig.add_subplot(212)
    ax.plot(iter_history, acc_history, 'g-', label='train acc')
    if acc_valid_history != None:
        ax.plot(iter_history, acc_valid_history, 'b-', label='test acc')    
    ax.set_ylabel(r"acc")
    ax.legend(loc="best")
    plt.savefig("tmp.png")
    plt.clf()

if __name__ == "__main__":
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    loader_train, loader_valid, loader_test = get_mnist()
    model = LeNet(1, 28, 28, 10).to(device)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters())

    loss_history, iter_history, acc_history, acc_valid_history = train(
        model, criterion, optimizer, device, loader_train, lr_decay=0.99, loader_valid=loader_valid, epoch=25)
    
    myshow(loss_history, iter_history, acc_history, acc_valid_history)

    print("test acc: ", acc(model, device, loader_test))