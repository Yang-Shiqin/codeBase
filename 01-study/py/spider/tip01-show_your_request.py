#!/usr/bin/env python
# coding=utf-8
import requests

# 1. 指定url
url = 'http://httpbin.org/get'      # 工具网站，直接返回请求头的信息
# 2. 发起请求(get)
response = requests.get(url)
# 3. 获取响应数据
print(response.text)
