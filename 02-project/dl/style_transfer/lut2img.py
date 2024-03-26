#!/usr/bin/env python
# coding=utf-8
import numpy as np
import cv2

def read_cube_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # 寻找 LUT 起始点
    start_index = lines.index([line for line in lines if line.startswith('LUT_3D_SIZE')][0]) + 1
    lut_size = int(lines[start_index - 1].split()[1])

    # 解析 LUT 数据
    lut_data = np.zeros((lut_size, lut_size, lut_size, 3), dtype=np.uint8)
    for i in range(lut_size):
        for j in range(lut_size):
            for k in range(lut_size):
                start_index += 1
                r, g, b = map(float, lines[start_index].split())
                lut_data[i, j, k] = [255*b, 255*g, 255*r]  # OpenCV使用的是BGR格式

    return lut_data, lut_size

def convert_cube_to_image(cube_file_path, output_image_path):
    # 读取 .cube 文件并解析数据
    lut_data, lut_size = read_cube_file(cube_file_path)
    lut_data = lut_data.reshape(lut_size**2, lut_size, -1)

    # 保存图像
    cv2.imwrite(output_image_path, lut_data)

# 调用函数
convert_cube_to_image('Autumn_03.cube', 'Autumn_03.png')

