import cv2
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.colors import ListedColormap

# 读取图像
image = cv2.imread('1.png')
image_lab = cv2.cvtColor(image, cv2.COLOR_BGR2LAB)

# 提取ab通道
ab = image_lab[:,:,1:].reshape(-1, 2)

# 统计直方图
hist, x_edges, y_edges = np.histogram2d(ab[:,0], ab[:,1], bins=(30, 30), range=[[0, 256], [0, 256]])

# 创建网格
xpos, ypos = np.meshgrid(x_edges[:-1] + 0.25, y_edges[:-1] + 0.25, indexing="ij")
xpos = xpos.ravel()
ypos = ypos.ravel()
zpos = 0

# 设置柱状图大小
dx = dy = 5 * np.ones_like(zpos)
dz = hist.ravel()

# 创建 Lab 到 RGB 的颜色映射
def lab_to_rgb(a, b):
    L = 180  # 设置 L 通道的值为 50（可以根据需要调整）
    lab = np.zeros((1, 1, 3), dtype=np.uint8)
    lab[:,:,0] = L
    lab[:,:,1] = a
    lab[:,:,2] = b
    return cv2.cvtColor(lab, cv2.COLOR_LAB2RGB).squeeze()/ 255.0

colors = [lab_to_rgb(a, b) for a, b in zip(xpos, ypos)]
cmap = ListedColormap(colors)

# 绘制三维柱状图
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
ax.bar3d(xpos, ypos, zpos, dx, dy, dz, zsort='average', color=colors)

# 设置坐标轴标签
ax.set_xlabel('a')
ax.set_ylabel('b')
ax.set_zlabel('Frequency')

# 显示图像
plt.savefig('h.png')

