# selenium在wsl1的环境弄不好，所以我是在win下运行的
import os
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys

if __name__ == "__main__":
    # 1. 指定url
    url = 'https://so.gushiwen.cn/user/login.aspx'
    option = webdriver.EdgeOptions()
    option.add_experimental_option("detach", True)
    
    driver = webdriver.Edge(options=option)
    driver.get(url)
    viewstate = driver.find_element(By.XPATH, '//*[@id="__VIEWSTATE"]')#/@value')
    email = driver.find_element(By.NAME, 'email')
    pwd = driver.find_element(By.NAME, 'pwd')
    code = driver.find_element(By.NAME, 'code')
    email.send_keys('2442494081@qq.com')
    #email.send_keys(Keys.RETURN)
    pwd.send_keys('112233445566')
    #pwd.send_keys(Keys.RETURN)
    c = input('code:')
    code.send_keys(c)
    code.send_keys(Keys.RETURN)
    text = driver.page_source#.encode("iso-8859-1").decode('utf-8')
    with open('../data/tmp.html', 'w', encoding='utf-8') as f:
        f.write(text)

    #driver.quit()
