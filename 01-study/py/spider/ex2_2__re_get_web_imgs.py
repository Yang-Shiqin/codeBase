#!/usr/bin/env python
# coding=utf-8
import requests
import re

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
text = response.text
pattern = '<div class="pic">.*?<img.*?data-origin="(.*?)" alt=.*?</div>'
img_list = re.findall(pattern, text, re.S) # [ ] todo: py的正则表达式；re.S(单行匹配)和re.M(多行匹配)
for img_url in img_list:
    if not img_url.startswith('http'):
       continue 
    # 生成图片名称
    name = img_url.split('/')[-1]
    # 取图片内容
    response = requests.get(url=img_url, headers = headers)
    img = response.content
    # 4. 持久化存储
    with open(f'./data/{name}', 'wb') as f:
        f.write(img)
    print(f'{name} download!')
