#!/usr/bin/env python
# coding=utf-8
import requests
import streamlit as st

query = st.text_input("搜索..")

# 1. 指定url
url = 'https://www.douban.com/'
# UA伪装
headers = {
    'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
}
params = {
    'query' : query
}
# 2. 发起请求(get)
response = requests.get(url=url, headers=headers, params=params)
# 3. 获取响应数据
print(response.status_code)
text = response.text
st.markdown(text, unsafe_allow_html=True)
