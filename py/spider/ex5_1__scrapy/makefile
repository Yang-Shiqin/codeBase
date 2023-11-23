PROJECT_NAME := scrapy_example
SPIDER_PATH := $(PROJECT_NAME)/$(PROJECT_NAME)/spiders
SPIDER_NAME := first
INIT_URL := www.xxx.com

create: $(PROJECT_NAME)
	cd $(SPIDER_PATH) && scrapy genspider $(SPIDER_NAME) $(INIT_URL)

run: $(PROJECT_NAME)
	scrapy crawl $(SPIDER_NAME)

$(PROJECT_NAME):
	scrapy startproject $(PROJECT_NAME)

#clean:
#	rm -rf $(PROJECT_NAME)
