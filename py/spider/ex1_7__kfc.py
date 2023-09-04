#!/usr/bin/env python
# coding=utf-8
# 爬取kfc某地所有餐厅信息

import requests

if __name__ == '__main__':
    # 得带op不然输不出东西
    url = 'https://www.kfc.com.cn/kfccda/ashx/GetStoreList.ashx?op=keyword'
    params = {
        'cname' : '',
        'pid'   : '',
        'keyword': '浙江',
        'pageIndex': None,
        'pageSize': '10',
    }
    headers = {
            'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
                    ' AppleWebKit/537.36 (KHTML, like Gecko)'
                    ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54',
    }
    for i in range(3):
        params['pageIndex'] = str(i+1)
        response = requests.post(url=url, data=params, headers=headers)
        print(response.text)
