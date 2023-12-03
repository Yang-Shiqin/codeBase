#!/usr/bin/env python
# coding=utf-8
import asyncio
import re
import requests
import sys
import time
from loguru import logger
from pathlib import Path
# [ ] todo: aiohttp改成异步请求

start_time = time.time()
logger.remove()
logger.add(sys.stderr, level='INFO')

class Bili:
    def __init__(self):
        # 保存视频到该文件夹下
        self.save_dir = Path('./data/bilibili').absolute()
        # 根url，即b站首页
        self.root_url = 'https://www.bilibili.com/'
        # 获取特定视频下载链接api，要加params
        self.play_url = 'https://api.bilibili.com/x/player/playurl'
        # 获取视频搜索结果的信息api（包含avid、bvid、详情页url、名字，不含cid
        self.search_url = ('https://api.bilibili.com/x/web-interface/search/type'
                        '?jsonp=jsonp&search_type=video&keyword={}&page={}')
        # 根据bvid获取cid的api
        self.pagelist_url = ('https://api.bilibili.com/x/player/pagelist?'
                        'bvid={}&jsonp=jsonp')
        self.headers = {
            'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
            ' AppleWebKit/537.36 (KHTML, like Gecko)'
            ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
        }
        # 创建会话对象
        self.session = requests.Session()
        self.session.headers.update(self.headers)
        # 第一次get获取cookie，不然后面可能会失败（忘了是这个原因还是page=0失败
        self.session.get(self.root_url)
        
    # 根据搜索keyword下载从前开始page页的视频
    # asy==1为异步(线程池)，否则串行
    def download_search(self, keyword, page=1, asy=1):
        bvid_list, name_list, cid_list = self.get_all_search_info(keyword, page)
        logger.debug('bvid_list:{}\nname_list:{}\ncid_list:{}', 
                     bvid_list, name_list, cid_list)
        logger.debug('len:{},{},{}', len(bvid_list), len(name_list), len(cid_list))
        assert(len(bvid_list)==len(name_list)==len(cid_list))
        l = len(bvid_list)
        info = [(bvid_list[i], name_list[i], cid_list[i]) for i in range(l)]
        if asy:
            # 不知道这样写对不对，还是得写到函数里再run
            # 这个还是用的同步的请求，应该改成aiohttp，但不是很好改暂时还没改
            asyncio.run(asyncio.gather(*[self.download_one(i) for i in info]))
        else:
            for i in info:
                self.download_one(i)

    # 根据info下载一个视频
    # info为tuple(bvid, name, cid)
    def download_one(self, info):
        bvid, name, cid = info
        if bvid=='' or bvid==None or cid==0: return
        params = {                       
            'bvid'  : bvid,    
            'qn'    : 64,                
            'cid'   : cid[0],        
            'platform'  : 'html5',       
            'high_quality'  : 1,         
        }                                
        if not self.save_dir.exists():
            self.save_dir.mkdir()
        # 没分p的视频
        if len(cid)==1:
            response = self.session.get(self.play_url,params=params).json()
            url = response['data']['durl'][0]['url']
            content = self.session.get(url).content
            with open(self.save_dir / f'{name}.mp4', 'wb') as f:
                f.write(content)
            logger.info(f'{name} download finished!')
            return 
        # 分p的视频, 目前命名方式是title_下标，没用分p的标题
        for i, icid in enumerate(cid):
            try:
                params['cid'] = icid
                response = self.session.get(self.play_url,params=params).json()
                url = response['data']['durl'][0]['url']
                content = self.session.get(url).content
                with open(self.save_dir / f'{name}_{i}.mp4', 'wb') as f:
                    f.write(content)
                logger.info(f'{name}_{i} download finished!')
            except:
                logger.warning(f'{name} download fail!!')
                continue

    # 根据keyword和page返回bvid、name、cid的list
    def get_all_search_info(self, keyword, page=1):
        bvid_list = []
        name_list = []
        pattern = '<em class=(.*)</em>' # 去除标记搜索的标签
        #self.session.headers.update(self.search_headers)
        for i in range(page):
            url = self.search_url.format(keyword, i+1)
            data = self.session.get(url).json()
            if data['code']==-400 or data['code']==-403 or data['code']==-404:
                logger.warning('data is None!!可能请求被拦截')
                return [],[],[]
            result_list = data['data']['result']
            for j in result_list:
                bvid_list.append(j['bvid'])
                name_list.append(re.sub(pattern, keyword, j['title'])) 
        cid_list = self.get_cid(bvid_list)
        return bvid_list, name_list, cid_list

    # 根据bvid获取cid(不知道为啥用avid获取会失败)
    # 返回：
    #   bvid为数，则返回一维数组（多p会有多个cid
    #   bvid为数组，则返回二维数组（多p会有多个cid
    def get_cid(self, bvid):
        cid_list = []
        if isinstance(bvid, str):
            data = self.session.get(self.pagelist_url.format(bvid)).json()['data']
            cid_list = [di['cid'] for di in data]
        else:
            for i in bvid:
                logger.debug(self.pagelist_url.format(i))
                data = self.session.get(self.pagelist_url.format(i)).json()
                if data['code']<=-400:
                    logger.warning('cid get err')
                    cid_list.append(0)
                    continue
                data = data['data']
                cid_list.append([di['cid'] for di in data])
        return cid_list


    # 爬取特定的一个视频的例子，是小拉泽的一个零食视频（因为没登陆所以码率较低
    def example(self):
        params = {
            'bvid'  : 'BV1nN411i7mg',
            'qn'    : 64,
            'cid'   : 1260891578,
            'platform'  : 'html5',
            'high_quality'  : 1,
        }
        response = self.session.get(self.play_url,params=params).json()
        durl = response['data']['durl']
        if not self.save_dir.exists():
            self.save_dir.mkdir()
        for i in durl:
            url = i['url']
            logger.debug(url)
            name = url.split('?')[0].split('/')[-1]
            content = self.session.get(url).content
            with open(self.save_dir / f'{name}', 'wb') as f:
                f.write(content)

b = Bili()
#b.example()
b.download_search('你好')
end_time = time.time()
logger.info(f'used time: {end_time-start_time}s.')
