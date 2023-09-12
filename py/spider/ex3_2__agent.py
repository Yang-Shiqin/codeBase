#!/usr/bin/env python
# coding=utf-8
# [ ] todo: 似乎用不了
import requests
from lxml import etree
from utils import mkdir

# 1. 指定url
url = 'https://www.python.org'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
}
# 2. 发起请求(get)
ip = '182.34.33.108:9999'
response = requests.get(url=url, headers=headers, proxies={"http":ip, 'https':ip})
# 3. 获取响应数据
print(response.status_code)
text = response.text
