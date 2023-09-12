from lxml import etree
import os
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By

if __name__ == "__main__":
    # 1. 指定url
    url = 'https://so.gushiwen.cn/user/login.aspx'
    # UA伪装
    headers = {
        'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
                ' AppleWebKit/537.36 (KHTML, like Gecko)'
                ' Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.54'
    }
    
    driver = webdriver.Edge()
    driver.get(url)
    viewstate = driver.find_element(By.XPATH, '//*[@id="__VIEWSTATE"]')#/@value')

    data = {
        '__VIEWSTATE': viewstate,
        '__VIEWSTATEGENERATOR': 'C93BE1AE',
        'from': 'http://so.gushiwen.cn/user/collect.aspx',
        'email': '2442494081@qq.com',
        'pwd': '112233445566',
        'code': input("code: "),
        'denglu': '登录',
    }
    response = requests.post(url=url, headers=headers, data=data)
    text = response.text.encode("iso-8859-1").decode('utf-8')
    print(text)
    with open('../data/tmp.html', 'w', encoding='utf-8') as f:
        f.write(text)
    driver.quit()
