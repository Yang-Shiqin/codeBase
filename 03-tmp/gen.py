#!/usr/bin/env python
# coding=utf-8
import torch
# import numpy as np
import pandas as pd

sum = 150       # 调查问卷数
err = 0.03      # 无效问卷最大百分比
choice = [2, 4, 2, 4, 6, 4, 4, 6, 2, 2, 4, 3]   # 题目选项数

choice = torch.tensor(choice)
ti = choice.shape[0]
n_err_sam = int(sum*err)
n_cor_sam = sum-n_err_sam

## 生成废卷
err_sam = (torch.rand(n_err_sam, ti)*choice).int()         # 0为空选项(没填)

## 生成好卷
# 3选A回到4567 选B回答8 后面都要回答

# 下面是各选项概率(类似轮盘赌)
rand = torch.rand(n_cor_sam, ti)
cor_sam =   (rand<(torch.tensor([5.3, 1     , 8 , 3     , 1     , 1 , 5 , 2.5, 7  , 6 , 2, 8  ])/10).expand(n_cor_sam, ti)).int()+\
            (rand<(torch.tensor([10 , 6.5   , 10, 4.5   , 3     , 3 , 6 , 2.8, 10 , 10, 6, 9.5])/10).expand(n_cor_sam, ti)).int()+\
            (rand<(torch.tensor([ 0 , 9     ,  0,   5   , 5     , 4 , 7 , 6.3,  0 ,  0, 9,  10])/10).expand(n_cor_sam, ti)).int()+\
            (rand<(torch.tensor([ 0 , 10    ,  0,  10   ,5.5    , 10, 10, 6.5,  0 ,  0,10,   0])/10).expand(n_cor_sam, ti)).int()+\
            (rand<(torch.tensor([ 0 , 0     ,  0,   0   ,6.5    ,  0, 0 , 7.5,  0 ,  0, 0,   0])/10).expand(n_cor_sam, ti)).int()+\
            (rand<(torch.tensor([ 0 , 0     ,  0,   0   , 10    ,  0, 0 ,  10,  0 ,  0, 0,   0])/10).expand(n_cor_sam, ti)).int()
cor_sam = choice+1-cor_sam
cor_sam[cor_sam[:, 2]==1, 7] = 0
cor_sam[cor_sam[:, 2]==2, 3:7] = 0

torch.set_printoptions(profile="full")
# print(cor_sam)

# all = torch.cat((err_sam, cor_sam), dim=0)
# # print(all)

# save = all.numpy()

# data = pd.DataFrame(save)

# writer = pd.ExcelWriter('A.xlsx')		# 写入Excel文件
# data.to_excel(writer, 'page_1', float_format='%d')		# ‘page_1’是写入excel的sheet名
# writer.save()

# writer.close()


print(cor_sam.shape)
print("A: ", (cor_sam==1).int().sum(dim=0))
print("B: ", (cor_sam==2).int().sum(dim=0))
print("C: ", (cor_sam==3).int().sum(dim=0))
print("D: ", (cor_sam==4).int().sum(dim=0))
print("E: ", (cor_sam==5).int().sum(dim=0))
print("F: ", (cor_sam==6).int().sum(dim=0))
