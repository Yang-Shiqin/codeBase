#!/usr/bin/env python
# coding=utf-8
import cv2
import numpy as np

lut = cv2.applyColorMap(np.arange(256, dtype=np.uint8), cv2.COLORMAP_JET)
cv2.imwrite('LUT.png', lut)
