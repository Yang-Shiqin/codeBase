# selenium在wsl1的环境弄不好，所以我是在win下运行的
# 用selenium爬取豆瓣电影排行榜电影具体信息
import os
from utils import mkdir
from selenium import webdriver
from selenium.webdriver.common.by import By

if __name__ == "__main__":
    # 豆瓣排行榜
    # 1. 指定url
    url = 'https://movie.douban.com/typerank?type_name=%E5%96%9C%E5%89%A7&type=24&interval_id=100:90&action='
    
    driver = webdriver.Edge()
    driver.get(url)
    title = driver.title
    print(title)
    title_ele = driver.find_element(By.XPATH, '//*[@id="content"]/h1')
    print(title_ele.text)

    # driver.quit()
