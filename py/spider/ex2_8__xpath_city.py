#!/usr/bin/env python
# coding=utf-8
import requests
from lxml import etree

# 1. 指定url
url = 'https://www.aqistudy.cn/historydata/'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
}
# 2. 发起请求(get)
response = requests.get(url=url, headers=headers)
# 3. 获取响应数据
print(response.status_code)
text = response.text     # 二进制形式的数据
# 创建BS对象并赋值网页源代码
e = etree.HTML(text)

# etree都是返回list
############## 标签定位
name_list = e.xpath('//div[@class="container"]//div[@class="all"]//li/a/text()')
print(name_list, len(name_list))
