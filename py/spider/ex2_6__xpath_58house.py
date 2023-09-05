#!/usr/bin/env python
# coding=utf-8
import json
import requests
from lxml import etree
# 解析58同城二手房房源信息并记录在json文件中

# 1. 指定url
url = 'https://wh.58.com/ershoufang/'
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
with open('data/58.html', 'w') as f:
    f.write(text)
# 创建BS对象并赋值网页源代码
e = etree.HTML(text)

# etree都是返回list
############### 标签定位
house_list = '//section[@class="list-body"]/section[@class="list-main"]/section[@class="list-left"]//div[@class="property-content"]'
house_list = e.xpath(house_list)
house_dict = {}
for ei in house_list:
    # 要加.
    house_dict[ei.xpath('.//h3/text()')[0]] = {
        'name'      : ei.xpath('.//p[@class="property-content-info-comm-name"]/text()')[0],
        'address'   : ei.xpath('.//p[@class="property-content-info-comm-address"]//text()'),
        'info'      : ei.xpath('.//p[@class="property-content-info-text"]/text()'),
    }

json_str = json.dumps(house_dict, ensure_ascii=False)
with open('data/58.json', 'w', encoding='utf-8') as json_file:
    json_file.write(json_str)
