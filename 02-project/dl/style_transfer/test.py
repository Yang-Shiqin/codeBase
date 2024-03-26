#!/usr/bin/env python
# coding=utf-8
import cv2
import matplotlib.pyplot as plt

class CalHist:
    def __init__(self, histSize=256, ranges=[0, 256]):
        self.histSize = histSize
        self.ranges = ranges

    # 输入cv2读取的图片形式
    def hsv_hist(self, img):
        hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        hue, saturation, value = cv2.split(hsv_img)
        print(hue)
        cv2.imwrite("hue.png", hue)
        cv2.imwrite("saturation.png", saturation)
        cv2.imwrite("value.png", value)
    
        # 计算色相直方图
        hue_hist = cv2.calcHist([hue], [0], None, [256], [0, 256])
        # 计算饱和度直方图
        saturation_hist = cv2.calcHist([saturation], [0], None, [256], [0, 256])
        # 计算明度（亮度）直方图
        value_hist = cv2.calcHist([value], [0], None, [256], [0, 256])
        return [hue_hist, saturation_hist, value_hist]

    def rgb_hist(self, img):
        hist = []
        channels = cv2.split(img)
        rgb = ['rr', 'gg', 'bb']
        for i in range(3):
            cv2.imwrite(rgb[i]+'.png', channels[i])
        for i, channel in enumerate(channels):
            hist_item = cv2.calcHist([channel], [0], None, [256], [0,256])
            hist.append(hist_item)
        return hist

def plot_histogram(hist, title):
    plt.figure()
    plt.plot(hist, color='black')
    plt.title(title)
    plt.xlabel('Pixel value')
    plt.ylabel('Frequency')
    plt.savefig(title+".png")
    plt.clf()

if __name__ == "__main__":
    img = cv2.imread("styles.png")
    cal = CalHist()
    hsv = cal.hsv_hist(img)
    rgb = cal.rgb_hist(img)
    hsv_title = ['h', 's', 'v']
    rgb_title = ['r', 'g', 'b']
    for i in range(3):
        plot_histogram(hsv[i], hsv_title[i])
        plot_histogram(rgb[i], rgb_title[i])


