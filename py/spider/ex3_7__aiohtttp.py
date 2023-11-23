#!/usr/bin/env python
# coding=utf-8
import aiohttp
import asyncio
import time
import sys
from loguru import logger
from pathlib import Path

start_time = time.time()
logger.remove()
logger.add(sys.stderr, level='INFO')

async def get_text(url):
    async with aiohttp.ClientSession() as session:
        # proxy='http://ip:port'参数设置代理
        async with session.get(url) as resp:
            await resp.text()       # 字符串格式响应
            #await resp.read()       # 二进制格式响应
            #await resp.json()       # json格式响应

async def main(url, i):
    await asyncio.gather(*[get_text(url) for o in range(i)])

# 1. 指定url
url = 'https://www.python.org'
asyncio.run(main(url,5))
end_time = time.time()
logger.info(f'used time: {end_time-start_time}s.')
