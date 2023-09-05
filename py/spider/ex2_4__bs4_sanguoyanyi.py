#!/usr/bin/env python
# coding=utf-8
# 爬取诗词名句网的三国演义每章的标题和内容
import requests
from bs4 import BeautifulSoup
from helper import mkdir


if __name__ == "__main__":
    main_dir = 'data/sanguoyanyi'
    # 1. 指定url
    url = 'https://www.shicimingju.com/book/sanguoyanyi.html'
    # UA伪装
    headers = {
        'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
                ' AppleWebKit/537.36 (KHTML, like Gecko)'
                ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
    }
    # 2. 发起请求(get)
    response = requests.get(url=url, headers=headers)
    # 3. 获取响应数据
    text = response.text     
    
    # 创建BS对象并赋值网页源代码
    soup = BeautifulSoup(text, 'lxml')
    
    # 标签定位
    chapter_list = soup.select('.book-mulu > ul > li > a')    # 每章标签

    mkdir(main_dir)
    # 提取数据
    for i in chapter_list:
        title = i.text.encode("iso-8859-1").decode('utf-8')
        body_url = 'https://www.shicimingju.com'+i['href']
        response = requests.get(body_url, headers=headers)
        text = response.text
        soupi = BeautifulSoup(text, 'lxml')
        body = soupi.find('div', class_="chapter_content")
        with open(f'{main_dir}/{title}.txt', 'w', encoding='iso-8859-1') as f:
            f.write(body.text)

