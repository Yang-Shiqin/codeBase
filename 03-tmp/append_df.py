#!/usr/bin/env python
# coding=utf-8

import pandas as pd

df = pd.DataFrame([{'a': 12, 'b': 34}, {'a':55, 'b': 66}])
df.to_csv("tmp.csv")
df = df._append({'a':222, 'b':2222}, ignore_index=True)
df.to_csv('tmp.csv', mode='a', header=False)
