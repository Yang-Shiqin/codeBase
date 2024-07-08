import numpy as np

def weighted_moving_average(data, window_size):
    """
    计算数据序列的加权移动平均值。
    
    参数:
    data (list/numpy.ndarray) - 待计算移动平均的数据序列
    window_size (int) - 移动窗口的大小
    
    返回:
    numpy.ndarray - 加权移动平均值序列
    """
    n = len(data)
    head = (window_size-1)//2
    tail = head+n-window_size+1
    weighted_avg = np.zeros(n)
    
    # 计算中间部分的加权平均值
    weights = np.ones(window_size) / window_size
    weighted_avg[head:tail] = np.convolve(data, weights, 'valid')
    
    # 计算两边部分的加权平均值
    for i in range(head):
        weighted_avg[i] = data[:1+i].mean()
    for i in range(tail, n):
        weighted_avg[i] = data[tail:1+i].mean()
    return weighted_avg

data = np.array([1,2,3,4,5])
print(weighted_moving_average(data, 2))
