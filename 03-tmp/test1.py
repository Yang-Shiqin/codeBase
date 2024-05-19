import pandas as pd
import numpy as np

def mae(y_true: pd.DataFrame, y_pred: pd.DataFrame, clean=True):
    if clean:
        y_true = y_true.replace(0, np.nan)        # 0值一般为异常值
        y_true = y_true.replace(1e-6, np.nan)     # 1e-6在数据库一般为？值
    mae = (y_true - y_pred).abs().dropna().mean()
    return mae

def mape(y_true: pd.DataFrame, y_pred: pd.DataFrame, clean=True):
    if clean:
        y_true = y_true.replace(0, np.nan)
        y_true = y_true.replace(1e-6, np.nan)
    mape = ((y_true - y_pred) / (y_true+1e-6)).abs().dropna().mean()
    return mape

def rmse(y_true: pd.DataFrame, y_pred: pd.DataFrame, clean=True):
    if clean:
        y_true = y_true.replace(0, np.nan)
        y_true = y_true.replace(1e-6, np.nan)
    mse = ((y_true - y_pred) ** 2).dropna().mean()
    rmse = mse ** 0.5
    return rmse

def pso_loss(y_true: pd.DataFrame, y_pred: pd.DataFrame, clean=True):
    if clean:
        y_true = y_true.replace(0, np.nan)
        y_true = y_true.replace(1e-6, np.nan)
    std = y_true.dropna().std()
    loss = ((y_pred - y_true) / (std+1e-6)).abs().dropna().sum()
    return loss

# 生成第一对DataFrame
data1 = {
    'A': [1,0,3],
    'B': [2,3,4],
}
df1 = pd.DataFrame(data1)

# 生成第二对DataFrame，数据不同于第一对
data2 = {
    'A': [2,3,2],
    'B': [2,3,2],
}
df2 = pd.DataFrame(data2)

print(df1, '\n', df2)
print((df1-df2)**2)
print(mae(df1['A'], df2['B']))
print(mape(df1, df2))
print(rmse(df1, df2))
loss = pso_loss(df1, df2)
print(loss)
print(loss.sum())
