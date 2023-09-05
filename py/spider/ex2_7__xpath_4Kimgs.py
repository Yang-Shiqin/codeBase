#!/usr/bin/env python
# coding=utf-8
import json
import requests
from lxml import etree
from helper import mkdir
# 解析彼岸图网4K动漫图片并下载

# 1. 指定url
root_url = 'https://pic.netbian.com'
url = f'{root_url}/4kdongman/'
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
text = response.text
# 创建BS对象并赋值网页源代码
e = etree.HTML(text)

mkdir('data/4Kimg')
# etree都是返回list
############### 标签定位
get_thumb = 0
if get_thumb:   # 缩略图
    imgs = e.xpath('//ul[@class="clearfix"]/li//img/@src')
    for count,i in enumerate(imgs):
        if count>20: break
        link = root_url+i
        print(link)
        name = i.split('/')[-1]
        response = requests.get(url=link, headers=headers)
        content = response.content
        with open(f'data/4Kimg/{name}', 'wb') as f:
            f.write(content)
else:   # 稍微清楚一点，但也不是4K（暂时不知道怎么破解登录
    imgs = e.xpath('//ul[@class="clearfix"]/li/a')
    for count,ei in enumerate(imgs):
        if count>20: break
        link = root_url+ei.xpath('./@href')[0]
        response = requests.get(url=link, headers=headers)
        texti = response.text
        eii = etree.HTML(texti)
        high_img_link = root_url+eii.xpath('//div[@class="view"]/div[@class="photo-pic"]//img/@src')[0]
        print(high_img_link)
        name = high_img_link.split('/')[-1]
        response = requests.get(url=high_img_link, headers=headers)
        content = response.content
        with open(f'data/4Kimg/{name}', 'wb') as f:
            f.write(content)
