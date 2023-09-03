#!/usr/bin/env python
# coding=utf-8
import urllib.request

url = 'https://www.python.org'
url = 'https://s.taobao.com/search?q=%E5%85%85%E7%94%B5%E5%AE%9D'
response = urllib.request.urlopen(url)
print(response.read())
print(response.status)
print(response.getheaders())
