#!/usr/bin/env python
# coding=utf-8
import requests
import time
from multiprocessing.dummy import Pool
from lxml import etree
from utils import mkdir

start_time = time.time()

class Bili:
    def __init__(self):
        self.root_url = 'https://www.bilibili.com/'
        self.playurl = 'https://api.bilibili.com/x/player/playurl'
        self.headers = {
            'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
        }
        

    def get_content(self, url, headers, content_type):
        print(url)
        response = requests.get(url, headers=headers)
        if response.status_code == 200:
            if content_type=='content':
                return response.content
            else:
                return response.text

    # 爬取特定的一个视频的例子，是小拉泽的一个零食视频（因为没登陆所以码率较低
    def example(self):
        params = {
            'bvid'  : 'BV1nN411i7mg',
            'qn'    : 64,
            'cid'   : 1260891578,
            'platform'  : 'html5',
            'high_quality'  : 1,
        }
        session = requests.Session()
        session.headers.update(self.headers)
        response = session.get(self.playurl,params=params).json()
        durl = response['data']['durl']
        for i in durl:
            url = i['url']
            print(url)
            name = url.split('?')[0].split('/')[-1]
            content = session.get(url).content
            with open(f'data/bilibili/{name}', 'wb') as f:
                f.write(content)

b = Bili()
b.example()
#e = etree.HTML(text)
#
## etree都是返回list
############### 标签定位
#video_list = e.xpath('//*[@id="i_cecream"]/div[2]/main/div[2]/div/div[1]/div/div/div[2]/a/@href')
#print(video_list)
#mkdir('data/bilibili')
#video_headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36',
#			'Accept-Language': 'zh-CN,zh;q=0.9',
#			'Accept-Encoding': 'gzip, deflate, br',
#			'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'}
#
#pool = Pool(10)
## 实例化线程池
## 每个url传给get_content, 每个线程同时运行get_content
#content = pool.map(get_content, video_list)
#end_time=time.time()
#print(end_time-start_time)
