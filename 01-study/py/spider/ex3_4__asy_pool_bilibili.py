#!/usr/bin/env python
# coding=utf-8
# [ ] todo: 爬bilibili视频
# 随便一个视频详情页，网络xhr中看到playurl?等，里面的响应json：dash-video-baseUrl是m4s下载链接，但具体是哪个视频的不清楚
# 应该是接下来播放的视频
# bzd为啥re找不到，明明我能找到
import requests
import re
import json
import time
from multiprocessing.dummy import Pool
from lxml import etree
from utils import mkdir

start_time = time.time()


def get_text(url):
    print(url)
    video_headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36',
			'Accept-Language': 'zh-CN,zh;q=0.9',
			'Accept-Encoding': 'gzip, deflate, br',
			'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'}
    response = requests.get(url, headers=video_headers)
    if response.status_code == 200:
        return response.text

def get_content(url):
    print(url)
    headers = {
        'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.content

def get_download_link(content):
    pattern = '__playinfo__=(.*)</script><script>window\.__INITIAL_STATE__='
#    pattern = '__playinfo__=(.*)</script><script>'
    try:
        infos = re.findall(pattern, content, re.DOTALL)  # 单行模式，即忽略换行
        infos = infos[0]

    except:
        print('re err')
        return '',''
    html = json.loads(infos)
    durl = html['data']['dash']['video']
    download_url = []
    download_id = []
    for i in range(len(durl)):
        download_url.append(durl[i]['baseUrl'])
        try:
            id = durl[i]['baseUrl'].split('?')[0].split('/')[-1]
        except:
            id = f'{i}.m4s'
        download_id.append(id)
    return download_url, download_id

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
mkdir('data/bilibili')
#for video_url in video_list:
#    print(video_url)
#    response = requests.get(url=video_url, headers=video_headers)
#    response = requests.get(url=video_url)
#    with open(f'data/bilibili/{video_url.split("/")[-1]}.html', 'w') as f:
#        f.write(response.text)

pool = Pool(10)
# 实例化线程池
# 每个url传给get_content, 每个线程同时运行get_content
content = pool.map(get_text, video_list)
video_list = pool.map(get_download_link, content)
video_url = []
video_id = []
for video in video_list:
    video_url += video[0]
    video_id += video[1]
print(video_id)
content = pool.map(get_content, video_url)
for i in range(len(content)):
    if not content[i]: continue
    with open(f'data/bilibili/{video_id[i]}', 'wb') as f:
        f.write(content[i])
end_time=time.time()
print(end_time-start_time)
