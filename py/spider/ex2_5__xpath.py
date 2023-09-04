#!/usr/bin/env python
# coding=utf-8
import requests
from lxml import etree

# 1. 指定url
url = 'https://www.douban.com/'
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
# 标签定位：绝对路径
print(e.xpath('/html/head/title'))
print(e.xpath('/html//title'))
# 属性定位：
print(e.xpath('//div[@class="title"]'))
# 索引定位：
print(e.xpath('//div[@class="title"]'))
#print(soup.find_all('img'))    # 返回list
# 属性值选择器，属性=title的就选择，返回符合条件的所有标签放进一个list
#print(soup.select('.title'))
# 层级选择器，进一步从上行选择出来的选择，直系子标签为a（可链式选择
#print(soup.select('.title > a'))
#print(soup.select('.movie-list > ul > li .title > a'))

############## 数据提取
l = soup.select('.movie-list > ul > li .title > a')
for i in l:
    print(i.text)
    print(i['href'])
