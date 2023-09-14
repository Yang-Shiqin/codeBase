#!/usr/bin/env python
# coding=utf-8
from loguru import logger
import sys

logger.remove()         # 去除默认输出到stderr的
trace = logger.add(     # 增加一个sink放log
    # sink='a.log',       # 输出位置.可以是file、str、pathlib.Path、method、handler
    sink=sys.stderr,
    format="{time} {level} {message}",  # 输出格式
    # filter="my_module", # 
    level="INFO"        # 输出log等级
    # rotation="500 MB",  # 每500MB存一个文件(可以把名字改成'a_{time}.log')
    # rotation="00:00",   # 每天0点创建, '1 week'每周，'10 days'每十天
    # retention="10 days",# 日志最久保留10天，'1 week, 3 days'
    # compression="zip",  # 压缩，gz/bz2/xz/lzma/tar/tar.gz/tar.bz2/tar.xz
                  )

# level(从高到低)
logger.error("错误")
logger.warning("警告")
logger.warning('{}{}{}',"警告",1,'sdf')
logger.info("信息")
logger.debug("调试")
logger.remove(trace)    # 移开sink(不再接log水)

