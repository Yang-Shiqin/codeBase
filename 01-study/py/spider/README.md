
```bash
.
├── data                            # 输出数据(html、img等)
├── ex1_1__urllib.py                # urllib库http请求例子
├── ex1_2__requests.py              # requests库http请求例子
├── ex1_3__para.py                  # requests参数设置例子
├── ex1_4__ua.py                    # UA伪装
├── ex1_5__baidu_fanyi              # 百度翻译聚焦翻译结果(不含数据解析)
│   ├── baidu_fanyi_sug.py          # 通过ajax请求sug返回简短翻译结果
│   ├── baidu_fanyi_v2transapi.py   # 通过ajax请求v2transapi返回复杂翻译结果
│   ├── get_sign.js                 # 从网上扒下来的百度翻译sign加密js脚本
│   └── helper.py                   # 我的cookie
├── ex1_6__douban_film.py           # 爬取豆瓣排行榜中多个电影具体信息, 豆瓣的链接现在似乎变了
├── ex1_7__kfc.py                   # 爬取kfc具体位置多页全部的营业厅信息
├── ex2_1__get_image.py             # 从img链接保存img
├── ex2_2__get_web_imgs.py          # 从豆瓣主页爬取所有图片保存
├── ex2_3__bs4.py                   # bs4库html解析例子
├── ex2_4__bs4_sanguoyanyi.py       # 用bs4爬取三国演义的各章节和内容
├── ex2_5__xpath.py                 # xpath库html解析例子
├── ex2_6__xpath_58house.py         # 用xpath爬取58同城二手房的信息
├── ex2_7__xpath_4Kimgs.py          # 用xpath爬取彼岸图网的图片
├── ex2_8__xpath_city.py            # 用xpath爬取城市名称
├── ex2_9__xpath_free_bio.py        # 用xpath爬取站长网站的免费简历模板
├── ex3_1__request_cookie           # request用cookie登录
├── ex3_2__agent.py                 # request代理(但是不行)
├── ex3_3__asy_pool.py              # 异步请求，用线程池request b站主页视频详情页内容
├── ex3_4__asy_pool_bilibili.py     # 异步请求，用线程池request b站主页视频详情页中视频链接下载（不知道是啥的链接，而且经常403失败，而且下下来的视频不一定能看
├── ex3_5__get_bilibili_video.py    # 异步请求，线程池request b站搜索的视频，批量下载，写得比较完整
├── ex3_6__asy_co.py                # 协程爬b站（现在用的是同步操作所以慢，还没改好
├── ex3_7__aiohtttp.py              # aiohttp库示例
├── ex4_1__selenium.py              # 网页自动化测试库selenium小示例
├── ex4_2__selenium_login.py        # selenium登录有验证码的古诗文网
├── fail                            # 失败合辑
├── project                         # 一些能直接和用户交互的小project
│   ├── st_badpeople.py             # st的豆瓣页(不太行)
│   └── st_fanyi.py                 # st的百度翻译
└── tip01-show_your_request.py      # 小技巧1: 能返回你请求报文的网站
├── utils.py                        # 有用的函数，如mkdir
```
