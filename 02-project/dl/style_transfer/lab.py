#!/usr/bin/env python
# coding=utf-8
import cv2
import numpy as np

# 图像大小
height = 100
width = 256

# 创建空白图像
lab_gradient = 128*np.ones((height, width, 3), dtype=np.uint8)
hsv_gradient = np.zeros((height, width, 3), dtype=np.uint8)

# 生成LAB的L梯度图
for i in range(width):
    lab_gradient[:, i, 0] = i

# 生成HSV的V梯度图
for i in range(width):
    hsv_gradient[:, i, 2] = i

# 转换LAB图像到RGB空间
lab_rgb = cv2.cvtColor(lab_gradient, cv2.COLOR_LAB2RGB)

# 转换HSV图像到RGB空间
hsv_rgb = cv2.cvtColor(hsv_gradient, cv2.COLOR_HSV2RGB)

# 保存图像
cv2.imwrite('lab_gradient.png', lab_rgb)
cv2.imwrite('hsv_gradient.png', hsv_rgb)

