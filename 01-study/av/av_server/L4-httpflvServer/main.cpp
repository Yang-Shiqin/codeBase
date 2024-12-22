#include "Server/BoostServer.h"
#include "Scheduler.h"
#include "Utils/Config.h"

// 框架和bxc_RTMP差不多, 代码稍微简单一些, 如果要深入理解可能需要学习boost库的API. 没用智能指针用的new/delete自己管理内存
// 核心：
// - 服务器收到连接调用新连接回调(创建连接, 设置断开连接回调)
// - 连接收到数据调用读处理回调(http flv server的核心逻辑)

/*
* 作者：北小菜
* 个人网站：http://www.any12345.com
* 邮箱：bilibili_bxc@126.com
* QQ：1402990689
* 微信：bilibili_bxc
* 作者-哔哩哔哩主页：https://space.bilibili.com/487906612
* 作者-头条西瓜主页：https://www.ixigua.com/home/4171970536803763
* github开源地址：https://github.com/any12345com/BXC_MediaServer
* gitee开源地址：https://gitee.com/Vanishi/BXC_MediaServer


ffmpeg 播放http-flv视频流
    ffplay -i http://localhost:8080/test.flv

实现浏览器播放HLS和HTTP-FLV视频流-源码下载  http://www.any12345.com/article.html?id=1

*/

int main(int argc, char* argv[])
{
    const char* file = NULL;
    file = "config.json";

    // 1. 读取配置文件
    // 读取配置文件用到了第三方库jsoncpp, 获取ip、port、threadNum
    Config config(file);
    if (!config.state) {
        printf("failed to read config file: %s\n", file);
        return -1;
    }
    
    // 2. 创建httpflv服务器
    // 服务器的实现用到了第三方库boost
    BoostServer server(&config);
    std::thread([&]() {
        server.start();
    }).detach();
    
    // 3. 创建调度器(其实啥都没干)
    Scheduler sch(&server,&config);
    sch.loop();

    return 0;
}
