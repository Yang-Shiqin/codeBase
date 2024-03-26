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

class Bottlrneck(torch.nn.Module):
    def __init__(self,In_channel,Out_channel,downsample=False):
        super(Bottlrneck, self).__init__()
        self.stride = 1
        if downsample == True:
            self.stride = 2

        self.layer = torch.nn.Sequential(
            torch.nn.Conv1d(In_channel, Out_channel, 3, self.stride, padding=1),
            torch.nn.BatchNorm1d(Out_channel),
            torch.nn.ReLU(),
            torch.nn.Conv1d(Out_channel, Out_channel, 3, padding=1),
            torch.nn.BatchNorm1d(Out_channel),
            torch.nn.ReLU(),
        )

        if In_channel != Out_channel:
            self.res_layer = torch.nn.Conv1d(In_channel, Out_channel,1,self.stride)
        else:
            self.res_layer = None

    def forward(self,x):
        if self.res_layer is not None:
            residual = self.res_layer(x)
        else:
            residual = x
        return self.layer(x)+residual

# 1. the model: ResNet18改成一维，去掉最后的分类
class ResNet18(pl.LightningModule):    # nn.Module->pl.LightningModule
    def __init__(self, Cin, K):
        """初始化LeNet网络结构和权重
        参数: 
            Cin: 输入样本通道数
            H: 输入样本高度
            W: 输入样本宽度
            K: 输出分类数
        """ 
        super().__init__()  # 初始化基类
        self.net = nn.Sequential(   # 有序容器，存储网络层，顺序执行
            torch.nn.Conv1d(Cin,64,kernel_size=7,stride=2,padding=3),
            torch.nn.MaxPool1d(3,2,1),

            Bottlrneck(64,64,False),
            Bottlrneck(64,64,False),

            Bottlrneck(64,128,True),
            Bottlrneck(128,128,False),

            Bottlrneck(128,256,True),
            Bottlrneck(256,256,False),

            Bottlrneck(256,512,True),
            Bottlrneck(512,512,False),

            torch.nn.AdaptiveAvgPool1d(1),
            torch.nn.Flatten(),
            torch.nn.Linear(2048, K)
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

    def forward(self, x):
        """前向传播，计算输入样本每个分类得分
        参数:
            x: 输入样本, 大小为(N, Cin, H, W)
        输出:
            score: 分类得分, 大小为(N, K)
        """
        x = x.float()
        score = self.net(x)
        return score

    # 3. the optimization: 自给自足的思想，所以定义在model里
    def configure_optimizers(self): # LightningModule API
        optimizer = optim.Adam(self.parameters(), lr=1e-3)
        return optimizer

        # [ ] TODO
    # 4. the loss function
    def cross_entropy_loss(self, logit, y):
        return F.nll_loss(logit, y)

        # [ ] TODO
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
class MNISTDataModule(pl.LightningDataModule):
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
