#!/usr/bin/env python
# coding=utf-8

from loguru import logger
import sys
sys.path.append('..')
from dir1.code1 import func1

def func2():
    logger.info("func2: ysq is good")
    func1()
    logger.debug("func2: ysq is good!")

if __name__ == "__main__":
    logger.add('../a.log', filter=lambda record: record['level'].name =='INFO')
    logger.add('../b.log', filter=lambda record: record['level'].name =='DEBUG')
    func2()
