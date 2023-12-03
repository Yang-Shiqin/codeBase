#!/usr/bin/env python
# coding=utf-8
# [ ] todo: 爬取站长网站(chinaz)的免费简历模板
import requests
from lxml import etree
from utils import mkdir
# 研究生系统（hub系统有点难，似乎还有握手挥手那个号，而且不知道哪个请求）
# code为验证码https://pass.hust.edu.cn/cas/code
# login开头的为登录请求，ul应该是username加密后，pl应该是password加密后（每次不一样）
#   lt每次也不一样
#   https://pass.hust.edu.cn/cas/login?service=http%3A%2F%2Fyjspy.hust.edu.cn%2FULoginST.aspx
# 1. 指定url
url = 'http://yjspy.hust.edu.cn/ULoginST.aspx'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54',
}
# 2. 发起请求(get)
response = requests.get(url=url, headers=headers)
# 3. 获取响应数据
print(response.status_code)
text = response.text
print(text)
## 创建BS对象并赋值网页源代码
#e = etree.HTML(text)
#
## etree都是返回list
############### 标签定位
#ei_list = e.xpath('//div[@id="main"]//p/a')
#mkdir('data/bio')
#for ei in ei_list:
#    link = ei.xpath('./@href')[0]
#    name = ei.xpath('./text()')[0].encode("iso-8859-1").decode('utf-8')
#    response = requests.get(url=link, headers=headers)
#    eii = etree.HTML(response.text)
#    download_link = eii.xpath('//div[@class="down_wrap"]/div[@class="clearfix mt20 downlist"]//li/a/@href')[0]
#    response = requests.get(url=download_link, headers=headers)
#    rar = response.content
#    with open(f'data/bio/{name}.rar', 'wb') as f:
#        f.write(rar)
