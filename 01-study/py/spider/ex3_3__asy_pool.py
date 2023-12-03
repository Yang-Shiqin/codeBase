#!/usr/bin/env python
# coding=utf-8
import requests
import time
from multiprocessing.dummy import Pool
from lxml import etree
from utils import mkdir

start_time = time.time()

def get_content(url):
    print(url)
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.content

# 1. 指定url
url = 'https://www.bilibili.com/'
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

# etree都是返回list
############## 标签定位
video_list = e.xpath('//*[@id="i_cecream"]/div[2]/main/div[2]/div/div[1]/div/div/div[2]/a/@href')
print(video_list)
mkdir('data/bilibili')
video_headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36',
			'Accept-Language': 'zh-CN,zh;q=0.9',
			'Accept-Encoding': 'gzip, deflate, br',
			'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'}

#for video_url in video_list:
#    print(video_url)
#    response = requests.get(url=video_url, headers=video_headers)
#    response = requests.get(url=video_url)
#    with open(f'data/bilibili/{video_url.split("/")[-1]}.html', 'w') as f:
#        f.write(response.text)

pool = Pool(10)
# 实例化线程池
# 每个url传给get_content, 每个线程同时运行get_content
content = pool.map(get_content, video_list)
end_time=time.time()
print(end_time-start_time)
