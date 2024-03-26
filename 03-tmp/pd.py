#!/usr/bin/env python
# coding=utf-8
import pandas as pd

d1 = {'1':12, '2':23}
d2 = {'1':22, '2':32}
pd.DataFrame([d1]).to_csv('aa.csv', index=False)
pd.DataFrame([d2]).to_csv('aa.csv', mode='a', header=False, index=False)
