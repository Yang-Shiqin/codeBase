# selenium在wsl1的环境弄不好，所以我是在win下运行的
import os
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from time import sleep

if __name__ == "__main__":
    output_path = 'HUST_resource/'
    # 1. 指定url
    url = 'http://it.cmpreading.com/index.aspx#'
    option = webdriver.EdgeOptions()
    option.add_experimental_option("excludeSwitches", ["enable-automation"])
    option.add_experimental_option("detach", True)
    
    driver = webdriver.Edge(options=option)
    driver.get(url)
    
    # 获取所有课程的href的元素
    href_ele_list = driver.find_elements(By.XPATH, '/html/body/div[3]/div[2]/div/div/div[2]/ul/li/div[1]/a')
    # 所有课程的href字符串
    href_list = [i.get_attribute('href') for i in href_ele_list]
    href_list = [
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b09bde4b0d59c87b7c88b&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f1786f3e4b0a1003cae2c37&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f17ebb6e4b0df48afbcc7b3&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0ac9e4b0d59c87b7c912&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0afce4b01b26d1bbdeaa&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0b29e4b01b26d1bbdec3&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0b92e4b0d59c87b7c983&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0c09e4b0d59c87b7c9bd&catalogid=1',

        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0b29e4b01b26d1bbdec3&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0a20e4b0d59c87b7c8b9&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6c6c53e4b01b26d1bc2802&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f0e6651e4b04349896bd0c9&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f2a64c1e4b0d6e16a29a330&catalogid=1',
        
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f2b5572e4b073cc175693fc&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f0ff4f1e4b04349896c44dd&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f17c814e4b0a1003cae4503&catalogid=1',
        
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f1a3657e4b0df48afbd4453&catalogid=1',
        # 'http://it.cmpreading.com/detail.aspx?id=p_5f335d21e4b075dc42ad36b7&catalogid=1',

        'http://it.cmpreading.com/detail.aspx?id=p_5f14fb95e4b0d73896b390bb&catalogid=1',

        # 'http://it.cmpreading.com/detail.aspx?id=p_5f6b0b69e4b0d59c87b7c95b&catalogid=1',
    ]
    for href in href_list:
        print('sdf'+href)
        sleep(20)
        driver.get(href)
        name = driver.find_element(By.XPATH, '/html/body/div[1]/div[2]/div[2]/div/div/h3').text.split('|')[-1]
        print(name)
        class_buttom = driver.find_elements(By.XPATH, '/html/body/div[1]/div[4]/div/div/div[2]/div/div')
        for j in class_buttom:
            j.click()
            sleep(20)
            video = driver.find_element(By.XPATH, '/html/body/div[1]/div[2]/div[1]/div/video')
            video_src = video.get_attribute('src')
            with open(output_path+name+'.txt', 'a', encoding='utf-8') as f:
                f.write(video_src+'\n')
    # admin.click()
    # email = driver.find_element(By.NAME, 'login')
    # pw = driver.find_element(By.NAME, 'password')
    # email.send_keys('2442494081@qq.com')
    # lpw = os.environ.get('lkp')
    # true_lpw = ''
    # for i, l in enumerate(lpw):
    #     true_lpw += chr(ord(l)+1+i)
    # pw.send_keys(true_lpw)
    # pw.send_keys(Keys.RETURN)
    
    driver.quit()
