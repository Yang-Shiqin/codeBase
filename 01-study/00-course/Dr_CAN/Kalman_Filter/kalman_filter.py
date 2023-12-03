#!/usr/bin/env python
# coding=utf-8
'''
DrCAN的KalmanFilter第五节示例实现，只测试过这一个例子（即无控制，输出和状态大小一样）
'''

import numpy as np

class KalmanFilter:
    def __init__(self, x0, A, B, H, P0=None, Q=None, R=None):
        '''
        x0  : 状态变量，nx1
        A   : 状态转移矩阵，nxn
        B   : 控制矩阵，nxk，k为控制变量个数
        H   : 输出矩阵，rxn，r为输出变量/观测变量个数
        P0  : 估计误差协方差矩阵初始值，nxn
        Q   : 过程噪声协方差矩阵，nxn
        R   : 测量误差协方差矩阵，rxr
        都是np.array
        '''
        assert(len(x0.shape)==2 and x0.shape[1]==1)
        assert(len(A.shape)==2)
        assert(len(H.shape)==2)
        self.n = x0.shape[0]
        self.r = H.shape[0]

        self.A = A
        self.B = B
        self.H = H
        self.Q = Q
        self.R = R
        self.x_post = x0
        self.P_post = P0
        if self.P_post is None:
            self.P_post = np.identity(self.n)
        if self.Q is None:
            self.Q = 0.1*np.identity(self.n)
        if self.R is None:
            self.R = np.identity(self.r)
        if (self.B is None) or (self.B==0):
            self.B = np.zeros((self.n, 1))
        assert(self.A.shape[0]==self.A.shape[1]==self.B.shape[0]==self.n)
        assert(self.H.shape[1]==self.P_post.shape[0]==self.P_post.shape[1]==self.n)
        assert(self.Q.shape[0]==self.Q.shape[1]==self.n)
        assert(self.R.shape[0]==self.R.shape[1]==self.r)

    def predict(self, uk):
        self.x_prior = self.A.dot(self.x_post) + self.B.dot(uk)
        self.P_prior = self.A.dot(self.P_post).dot(A.T) + self.Q

    def update(self, zk):
        self.K = self.P_prior.dot(self.H.T).dot(np.linalg.inv(self.H.dot(self.P_prior.dot(self.H.T)) + self.R))
        self.x_post = self.x_prior+self.K.dot(zk-self.H.dot(self.x_prior))
        self.P_post = (np.identity(self.n)-self.K.dot(self.H)).dot(self.P_prior)

    def run(self, z, u=None):
        '''
        z   : 观测变量list，Nxr，N为序列数量
        u   : 控制变量list，Nxk，为None则控制量为0
        '''
        assert(len(z.shape)==2)
        N = z.shape[0]
        if u is None:
            u = np.zeros((N, 1))
        assert(u.shape[0]==N)
        res = dict({
            'x_prior' : np.zeros((N, self.n)),    # 方便表示，少了一维，nx1->n
            'P_prior' : np.zeros((N, self.n, self.n)),
            'K'       : np.zeros((N, self.n, self.r)),
            'P_post'  : np.zeros((N, self.n, self.n)),
            'x_post'  : np.zeros((N, self.n)),
        })
        for i in range(len(z)):
            self.predict(u[i].reshape(-1, 1))
            res['x_prior'][i] = self.x_prior.T.copy()
            res['P_prior'][i] = self.P_prior.copy()
            self.update(z[i].reshape(-1, 1))
            res['K'][i] = self.K.copy()
            res['x_post'][i] = self.x_post.T.copy()
            res['P_post'][i] = self.P_post.copy()
        return res


if __name__ == "__main__":
    x0 = np.array([[0.0], [1.0]])
    A = np.array([
        [1, 1.], 
        [0, 1]
    ])
    B = 0
    H = np.array([
        [1, 0], 
        [0, 1.]
    ])
    Q = np.array([
        [0.1, 0], 
        [0, 0.1]
    ])
    R = np.array([
        [1, 0], 
        [0, 1.]
    ])

    z = np.array([
        [-0.761, 0.411],
        [3.613, 1.877],
        [1.757, -2.188],
        [0.273, 1.072],
        [4.197, 0.476],
        [2.963, 0.402],
        [2.193, -1.090],
        [2.921, -0.964],
        [1.766, -0.442],
        [2.063, 0.376],
        [3.456, -1.190],
        [2.700, -0.605],
        [0.220, -0.299],
        [1.062, 0.609],
        [-0.574, 0.003],
        [-1.731, 0.396],
        [0.179, -0.465],
        [-1.483, -0.464],
        [-2.157, -1.005],
        [-2.633, -0.994],
        [-4.738, -2.077],
        [-0.919, 1.043],
        [-3.289, -1.082],
        [-1.988, 1.201],
        [-3.986, -0.253],
        [-2.454, -0.621],
        [-4.587, 0.133],
        [-2.728, -0.149],
        [-4.290, 0.114],
        [-4.254, -0.074]])
    K = KalmanFilter(x0=x0, A=A, B=B, H=H, Q=Q, R=R)
    res = K.run(z)
    print(res)
