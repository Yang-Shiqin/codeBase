#!/usr/bin/env python
# coding=utf-8
import cv2
import numpy as np
import matplotlib.pyplot as plt

class CalHist:
    def __init__(self, histSize=256, rl=0, rr=256):
        """
        参数:
            histSize: hist长度(精度)
            rl: ranges的左界, ranges是统计的颜色范围
            rr: ranges的右界, ranges是统计的颜色范围
        """
        self.histSize = histSize
        self.ranges = [rl, rr]

    # 输入cv2读取的图片形式, 计算img的hsv直方图list
    def cal_hsv_hist(self, img):
        hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        hue, saturation, value = cv2.split(hsv_img)
    
        # 计算色相直方图
        hue_hist = cv2.calcHist([hue], [0], None, [self.histSize], self.ranges)
        # 计算饱和度直方图
        saturation_hist = cv2.calcHist([saturation], [0], None, [self.histSize], self.ranges)
        # 计算明度（亮度）直方图
        value_hist = cv2.calcHist([value], [0], None, [self.histSize], self.ranges)
        print(len(hue_hist))
        return [hue_hist, saturation_hist, value_hist]

    # 计算img的rgb直方图list
    def cal_rgb_hist(self, img):
        hist = []
        # imread默认为BGR格式
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        channels = cv2.split(img)
        for i, channel in enumerate(channels):
            hist_item = cv2.calcHist([channel], [0], None, [self.histSize], self.ranges)
            hist.append(hist_item)
        return hist

    # 3维的直方图, 输出为1维的np.array
    def cal_rgb3_hist(self, img, histSize=30):
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        hist = cv2.calcHist([img], [0,1,2], None, [histSize]*3, self.ranges*3)
        hist = np.array(hist).reshape(-1)
        return hist

    # 画出img的hsv、rgb直方图
    def plot_hist(self, img):
        hsv_hist = self.cal_hsv_hist(img)
        rgb_hist = self.cal_rgb_hist(img)
        rgb = ['r', 'g', 'b']
        hsv = ['h', 's', 'v']
        hsv_color = ['black', 'orange', 'y']
        plt.figure()
        for i in range(3):
            plt.plot(rgb_hist[i], color=rgb[i], label=rgb[i])
            plt.plot(hsv_hist[i], color=hsv_color[i], label=hsv[i])

        plt.xlabel('Pixel value')
        plt.ylabel('Frequency')
        plt.legend(loc='best')
        plt.savefig('hist.png')
        plt.clf()

    def plot_rgb3_hist(self, img, histSize=30):
        hist = self.cal_rgb3_hist(img, histSize)
        plt.figure()
        plt.plot(hist)
        plt.savefig('hist3.png')
        plt.clf()

if __name__ == "__main__":
    img = cv2.imread("styles.png")
    cal = CalHist(30, [0, 256])
    #cal = CalHist()
    cal.plot_hist(img)




