#!/usr/bin/env python
# coding=utf-8
import requests

# 1. 指定url
url = 'https://sogou.com/web?query=lza'
url = 'https://sogou.com/web'
param = {
    'query' : 'lza'
}
# 2. 发起请求(get)
response = requests.get(url=url, params=param)
# 3. 获取响应数据
print(response.status_code)
text = response.text
# 4. 持久化存储
with open('./data/tb.html', 'w', encoding='utf-8') as f:
    f.write(text)
