#!/usr/bin/env python
# coding=utf-8
# 聚焦部分数据(翻译结果)
# 百度翻译会局部刷新(输入文字改变url自动改变)   阿贾克斯，刷新的部分就是我们想要的部分
# 查看阿贾克斯包：检查-网络-XHR-负载/预览（可以看到有个post他post了什么，返回的json是我们想要的结果
import requests
import streamlit as st
import time

query = st.text_input("请输入待翻译的合法英文单词: ", key=1)
# 1. 指定url
url = 'https://fanyi.baidu.com/sug'
# 参数处理
params = {
    'kw'    : query,
    'from'  : 'en',
    'to'    : 'zh',
}
# 2. 发起请求(get)
response = requests.post(url=url,
                         data=params,     # 相当于get的params（长度不限
                         )
    
# 3. 获取响应数据
text = response.json()                # 查看响应的content-type是json
if text['errno']==0 and response.status_code==200:
    data = text['data']       
    for di in data:
        st.text(f"{di['k']}: {di['v']}")
