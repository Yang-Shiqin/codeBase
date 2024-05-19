#!/usr/bin/env python
# coding=utf-8
import matplotlib.pyplot as plt
import pytorch_lightning as pl
import torch
import torch.nn as nn
import torch.optim as optim

from torch.nn import functional as F
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms

# 1. the model
class LeNet(pl.LightningModule):    # nn.Module->pl.LightningModule
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

    def forward(self, x):  # LightningModule API
        """前向传播，计算输入样本每个分类得分
        参数:
            x: 输入样本, 大小为(N, Cin, H, W)
        输出:
            score: 分类得分, 大小为(N, K)
        """
        x = x.float()
        score = self.net(x)
        return score

    def predict(self, x):
        """预测输入样本类别
        参数:
            x: 输入样本, 大小为(N, Cin, H, W)
        输出:
            y: 预测类别, y in [0, .., K-1]
        """
        score = self.forward(x)
        y = torch.argmax(score, dim=1)
        return y

    # 3. the optimization: 自给自足的思想，所以定义在model里
    def configure_optimizers(self): # LightningModule API
        optimizer = optim.Adam(self.parameters(), lr=1e-3)
        return optimizer

    # 4. the loss function
    def cross_entropy_loss(self, logit, y): # 未必定义在model里
        return F.nll_loss(logit, y)

    # 5. training
    def training_step(self, batch, batch_idx):  # LightningModule API
        x, y = batch
        logit = self.forward(x)
        loss = self.cross_entropy_loss(logit, y)
        self.log('train_loss', loss)
        return loss
    
    def validation_step(self, batch, batch_idx):  # LightningModule API
        x, y = batch
        logit = self.forward(x)
        loss = self.cross_entropy_loss(logit, y)
        self.log('val_loss', loss)

# 2. the data
class MNISTDataModule(pl.LightningDataModule):  # 或者用DataLoader(Trainer.fit的参数要改)
    """mnist数据集模块
    Examples:
        >>> mnist = MNISTDataModule(data_dir='./data/')
        >>> model = xxModel()
        >>> trainer = Trainer()
        >>> trainer.fit(model, mnist)
    """
    def __init__(self, data_dir: str = "./data/"):
        super().__init__()
        self.data_dir = data_dir
        self.transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.1307,), (0.3081,))])

    ## 1. 下载数据集
    def prepare_data(self): # LightningDataModule API
        datasets.MNIST(self.data_dir, train=True, download=True)
        datasets.MNIST(self.data_dir, train=False, download=True)

    ## 2. 数据集预处理和划分
    def setup(self, stage: str = "fit"):    # LightningDataModule API
        if stage == "fit":
            mnist_full = datasets.MNIST(self.data_dir, train=True, transform=self.transform)
            self.mnist_train, self.mnist_val = random_split(
                mnist_full, 
                [len(mnist_full)-int(len(mnist_full)//6), int(len(mnist_full)//6)], 
                generator=torch.Generator().manual_seed(42),
            )

        # Assign test dataset for use in dataloader(s)
        if stage == "test":
            self.mnist_test = datasets.MNIST(self.data_dir, train=False, transform=self.transform)

        if stage == "predict":
            self.mnist_predict = datasets.MNIST(self.data_dir, train=False, transform=self.transform)

    ## 3. 生成DataLoader
    def train_dataloader(self): # LightningDataModule API
        return DataLoader(self.mnist_train, batch_size=32)

    def val_dataloader(self):   # LightningDataModule API
        return DataLoader(self.mnist_val, batch_size=32)

    def test_dataloader(self):  # LightningDataModule API
        return DataLoader(self.mnist_test, batch_size=32)

    def predict_dataloader(self):   # LightningDataModule API
        return DataLoader(self.mnist_predict, batch_size=32)

# 6. train loop
trainer = pl.Trainer(max_epochs=1)
mnist = MNISTDataModule(data_dir='./data/')
model = LeNet(1, 28, 28, 10)
trainer.fit(model, mnist)