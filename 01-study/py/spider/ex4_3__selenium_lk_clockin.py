# selenium在wsl1的环境弄不好，所以我是在win下运行的
import os
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from time import sleep

if __name__ == "__main__":
    # 1. 指定url
    url = 'https://leetcode.cn/'
    cookie_name = 'LEETCODE_SESSION'
    option = webdriver.EdgeOptions()
    option.add_experimental_option("excludeSwitches", ["enable-automation"])
    option.add_experimental_option("detach", True)
    print('sd')
    
    driver = webdriver.Edge(options=option)
    driver.get(url)
    # 创建一个字典来表示要添加的Cookie
    cookie = {
        'name': cookie_name,
        'value': os.environ.get(cookie_name),
        'domain': '.leetcode.cn',
        'path': '/',
    }
    
    # 添加Cookie到WebDriver实例
    driver.add_cookie(cookie)
    
    # 刷新页面，使添加的Cookie生效
    driver.refresh()
    sleep(20)   # 直接找元素可能还没加载出来，所以要等一会
    try:
        # 账号登录（但是不可能成功，因为我没有lk账号
        login = driver.find_element(By.CSS_SELECTOR, '#navbar_sign_in_button')
        if login:   # 未登录
            login.click()
            sleep(20)
            admin = driver.find_element(By.XPATH, '//*[@id="lc-content"]/div/div/div/div/div[6]/div')
            admin.click()
            email = driver.find_element(By.NAME, 'login')
            pw = driver.find_element(By.NAME, 'password')
            email.send_keys('2442494081@qq.com')
            lpw = os.environ.get('lkp')
            true_lpw = ''
            for i, l in enumerate(lpw):
                true_lpw += chr(ord(l)+1+i)
            pw.send_keys(true_lpw)
            pw.send_keys(Keys.RETURN)
    except:
        pass
    driver.quit()
