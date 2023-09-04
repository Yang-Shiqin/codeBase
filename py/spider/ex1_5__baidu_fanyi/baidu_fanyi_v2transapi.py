#!/usr/bin/env python
# coding=utf-8
# 聚焦部分数据(翻译结果)
# 百度翻译会局部刷新(输入文字改变url自动改变)   阿贾克斯，刷新的部分就是我们想要的部分
# 查看阿贾克斯包：检查-网络-XHR-负载/预览（可以看到有个post他post了什么，返回的json是我们想要的结果
import requests
import execjs
from helper import c

def get_sign(query):
    with open('get_sign.js', 'r') as f:
        js = f.read()
    exec_obj = execjs.compile(js)
    sign = exec_obj.call('sign', query)
    return sign

if __name__ == "__main__":
    # 1. 指定url
    url = 'https://fanyi.baidu.com/v2transapi'
    # UA伪装
    headers = {
#        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
#                ' AppleWebKit/537.36 (KHTML, like Gecko)'
#                ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54',
        'Cookie'    : c,
    }
    # 参数处理
    query = 'dog'
    params = {
        'query' : query,
        'token' : 'dba02bd3d290ef17bf74765bbc5bb7e2',
        'from'  : 'en',
        'to'    : 'zh',
        'sign'  : get_sign(query)
    }
    # 2. 发起请求(get)
    response = requests.post(url=url, 
                             data=params,     # 相当于get的params（长度不限
                             headers=headers)
                            #)
    # 3. 获取响应数据
    print(response.status_code)
    text = response.text        # 查看响应的content-type是json
    print(text)
