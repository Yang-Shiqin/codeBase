#!/usr/bin/env python
# coding=utf-8
# 爬取豆瓣电影排行榜电影具体信息

import requests

if __name__ == '__main__':
    # 豆瓣排行榜
    url = 'https://movie.douban.com/j/chart/top_list'
    params = {
        'type': '24',
        'interval_id': '100:90',
        'action': '',
        'start'     : '20',
        'limit'     : '20',
    }
    headers = {
            'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
                    ' AppleWebKit/537.36 (KHTML, like Gecko)'
                    ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54',
    }
    response = requests.get(url=url, params=params, headers=headers)
    print(response.json())
