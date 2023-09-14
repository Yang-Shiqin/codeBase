#!/usr/bin/env python
# coding=utf-8
import asyncio

async def request(url):
    print(f'{url} success')

asyncio.run(request('sdf'))
asyncio.run(request('sdf'))
