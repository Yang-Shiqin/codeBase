#!/usr/bin/env python
# coding=utf-8
# [ ] todo: 爬取站长网站(chinaz)的免费简历模板
import requests
from lxml import etree
from utils import mkdir
import os
from selenium import webdriver

# 1. 指定url
url = 'https://so.gushiwen.cn/user/login.aspx'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
}

# 显示古诗文网验证码图片
#def get_code():
    
def get_viewstate():
    response = requests.get(url=url, headers=headers)
    text = response.text
    e = etree.HTML(text)
    viewstate = e.xpath('//*[@id="__VIEWSTATE"]/@value')
    return viewstate

def get_code():
    driver = webdriver.Chrome()
    driver.get(url)
 
    # 截图，图片后缀最好为.png，如果是其他的执行的时候会有警告，但不会报错
    driver.get_screenshot_as_file('./data/tmp.jpg')
    driver.quit()
#    url = 'https://so.gushiwen.cn/RandCode.ashx'
#    response = requests.get(url=url, headers=headers)
#    text = response.content
#    with open('data/tmp.jpg', 'wb') as f:
#        f.write(text)
    os.system('viu data/tmp.jpg -1')
    code = input("code: ")
    return code

 

data = {
    '__VIEWSTATE': get_viewstate(),
    '__VIEWSTATEGENERATOR': 'C93BE1AE',
    'from': 'http://so.gushiwen.cn/user/collect.aspx',
    'email': '2442494081@qq.com',
    'pwd': '112233445566',
    'code': get_code(),
    'denglu': '登录',
}
# 2. 发起请求(get)
response = requests.post(url=url, headers=headers, data=data)
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
