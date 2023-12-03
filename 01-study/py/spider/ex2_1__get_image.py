#!/usr/bin/env python
# coding=utf-8
import requests

# 1. 指定url，随便一个图片的url
url = 'https://img03.sogoucdn.com/v2/thumb/crop/xy/ai/x/0/y/0/w/120/h/80/iw/90/ih/60/t/0/ir/3/retype_exclude_gif/ext/auto/q/75?t=2&appid=200997&url=https%3A%2F%2Fimg.alicdn.com%2Fbao%2Fuploaded%2Fi1%2F870052386%2FO1CN01TH1eid1TUpnOEJPZL_!!0-item_pic.jpg_300x300.jpg&sign=5d68d3da5c2529dbd17ede7410df149e'
# 2. 发起请求(get)
response = requests.get(url=url)
# 3. 获取响应数据
print(response.status_code)
text = response.content     # 二进制形式的数据
# 4. 持久化存储
with open('./data/tb.png', 'wb') as f:  # 二进制存储
    f.write(text)
