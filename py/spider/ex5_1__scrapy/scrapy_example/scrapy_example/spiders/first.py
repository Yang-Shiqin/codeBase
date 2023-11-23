import scrapy


class FirstSpider(scrapy.Spider):
    # 爬虫文件的名称
    name = "first"
    # 允许的域名: 用来限定start_urls列表中哪些url可以进行请求发送（一般会注释掉不用
    allowed_domains = ["www.xxx.com"]
    # 起始的url列表，该列表中存放的url会被scrapy自动进行请求的发送
    start_urls = ["https://www.xxx.com"]
    
    # 用于数据解析，response是请求成功后的响应对象
    def parse(self, response):
        pass
