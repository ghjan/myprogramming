# -*- coding: utf-8 -*-

# Scrapy settings for taobao_goods project
#
# For simplicity, this file contains only settings considered important or
# commonly used. You can find more settings consulting the documentation:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#     http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#     http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html

BOT_NAME = 'taobao_goods'

SPIDER_MODULES = ['taobao_goods.spiders']
NEWSPIDER_MODULE = 'taobao_goods.spiders'

# LOG_FILE = "scrapy.log"

# 设置深度 默认为0（无限制）
DEPTH_LIMIT = 2

# 注意注意：这里要加浏览器信息，不然会有500的错误
USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; rv:40.0) Gecko/20100101 Firefox/40.0'

# DOWNLOADER_MIDDLEWARES = {
#         'scrapy.contrib.downloadermiddleware.httpproxy.HttpProxyMiddleware':301,
#     }

# 下载图片的pipelines scrapy都做好了
ITEM_PIPELINES = {'scrapy.pipelines.images.ImagesPipeline': 1}
# items 中图片的url
IMAGES_URLS_FIELD = 'IMG_URL'
# .代表当前路径
IMAGES_STORE = '.'
# 设置图片大小
# IMAGES_THUMBS = {
#     'small': (50, 50),
#     'big': (270, 270),
# }

# Configure item pipelines
# See http://scrapy.readthedocs.org/en/latest/topics/item-pipeline.html
ITEM_PIPELINES = {
   'taobao_goods.pipelines.TaobaoGoodsPipeline': 300,
}
