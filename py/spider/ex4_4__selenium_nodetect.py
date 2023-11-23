# selenium在wsl1的环境弄不好，所以我是在win下运行的
import os
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from time import sleep

if __name__ == "__main__":
    # 1. 指定url
    option = webdriver.EdgeOptions()
    option.add_experimental_option("excludeSwitches", ["enable-automation"])
    option.add_experimental_option("detach", True)
    
    driver = webdriver.Edge(options=option)
    r = driver.get('https://www.baidu.com')
    print(driver.page_source)
    driver.save_screenshot('data/tmp.png')
    #driver.quit()
