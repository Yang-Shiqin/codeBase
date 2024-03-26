#!/usr/bin/env python
# coding=utf-8
import cv2
import numpy as np
import matplotlib.pyplot as plt


def hue_same(img, h):
    hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hue, saturation, value = cv2.split(hsv_img)
    # # hue = np.ones(hue.shape, dtype=np.uint8) * h
    # value = np.ones(value.shape, dtype=np.uint8) * h
    # merged_hsv_img = cv2.merge([hue, saturation, value])

    # # 如果需要转换回BGR格式的图像，可以使用 cv2.cvtColor() 函数
    # bgr_img = cv2.cvtColor(merged_hsv_img, cv2.COLOR_HSV2BGR)
    # cv2.imwrite('kk3.png', bgr_img)
    # # hsv_img = cv2.cvtColor(bgr_img, cv2.COLOR_BGR2HSV)
    # # hue, saturation, value = cv2.split(hsv_img)
    cv2.imwrite("hue1.png", hue)
    cv2.imwrite("saturation1.png", saturation)
    cv2.imwrite("value1.png", value)

if __name__ == "__main__":
    img = cv2.imread("5.jpg")
    hue_same(img, 50)
