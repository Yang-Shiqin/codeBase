#!/usr/bin/env python
# coding=utf-8
# http无状态，所以登录后到的页面也取不到用户信息，
#   所以用cookie保留状态（cookie可以用登录后响应的header的set_cookie取)
#   或用session登录（session会自己记cookie
import requests
from lxml import etree
from cookie import c

# 1. 指定url
url = 'https://so.gushiwen.cn/user/collect.aspx'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54', 
    'Cookie'    : c
}
# 2. 发起请求(get)
response = requests.get(url=url, headers=headers)
# 3. 获取响应数据
print(response.status_code)
text = response.text
with open('tmp.html', 'w') as f:
    f.write(text)
