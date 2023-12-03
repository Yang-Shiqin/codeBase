#!/usr/bin/env python
# coding=utf-8
import requests

# 1. 指定url
url = 'https://s.taobao.com/search?q=充电宝'
url = 'https://www.python.org'
# 2. 发起请求(get)
response = requests.get(url)
# 3. 获取响应数据
print(response.status_code)
text = response.text
# 4. 持久化存储
with open('./data/tb.html', 'w', encoding='utf-8') as f:
    f.write(text)
