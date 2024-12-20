/* 最基础基于SDL+FFMpeg播放器(能够播放音视频和退出)基础上, 实现较精细的同步, 能够空格暂停继续 */

// make
// ./BasicAvPlayer ../../data/human.mp4

// [ ] TODO: 视频pts无效时没有处理, 而且继续用来计算

#include "av_SDL.h"

int main(int argc, char *argv[]){
    av_log_set_level(AV_LOG_INFO);  // 设置日志级别
    // 0. 命令行参数解析
    if (argc < 2) {  // 错误处理
        av_log(NULL, AV_LOG_ERROR, "usage: %s <input>\n", argv[0]);
        return 1;
    }
    char *src = argv[1];
    if (!src) {
        av_log(NULL, AV_LOG_ERROR, "input is NULL\n");
        return 1;
    }

    // 1. 创建AvProcessor对象
    AvProcessor processor(src);

    // 2. 初始化SDL播放器
    Player player(&processor);

    // 3. 播放
    player.play();

    return 0;
}