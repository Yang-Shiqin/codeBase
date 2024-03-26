#!/usr/bin/env python
# coding=utf-8
import cv2
import numpy as np
import torch
from cal_hist import CalHist
import matplotlib.pyplot as plt

# LUT转换为图片

# 计算与5000个LUT距离(相似度)的函数, 记为5000相似度
def cal_5000LUT_sim(img, histSize=10):
    cal_hist = CalHist()
    sims = []

    LUTs = pass

    for i in LUTs:
        sim = cal_hist3_sim(img, i, histSize)
        sims.append(sim)
    return sims

# 计算两两图片的5000相似度的相似度, 前多少名或阈值代表风格相似度高的图片
def cal_img_sim(img1, img2, histSize=10):
    sim1 = np.array(cal_5000LUT_sim(img1, histSize))
    sim2 = np.array(cal_5000LUT_sim(img2, histSize))
    sim = np.dot(sim1, sim2)/(np.linalg.norm(sim1)*np.linalg.norm(sim2))
    return sim

# 计算两张图片的3维直方图cos相似度
def cal_hist3_sim(img1, img2, histSize=10):
    cal_hist = CalHist()
    h1, w1, _ = img1.shape
    h2, w2, _ = img2.shape
    hist1 = cal_hist.cal_rgb3_hist(img1, histSize)/h1/w1*histSize**3
    hist2 = cal_hist.cal_rgb3_hist(img2, histSize)/h2/w2*histSize**3
    # cos距离
    sim = np.dot(hist1, hist2)/(np.linalg.norm(hist1)*np.linalg.norm(hist2))
    return sim



if __name__ == "__main__":
    # 滤镜LUT图
    #random_array = np.random.randint(0, 256, (3, 512, 512), dtype=np.uint8)
    #img = cv2.merge([random_array[0], random_array[1], random_array[2]])
    img = cv2.imread('saturation.png')
    img2 = cv2.imread('hue.png')
    h, w, _ = img.shape
    cal_hist = CalHist(256, [0, 256])
    hist = cal_hist.cal_rgb3_hist(img, 10)/h/w*10**3
    print(hist)
    plt.plot(hist)
    plt.savefig("hist3.png")
    out = cal_hist3_sim(img, img2)
    print(out)


