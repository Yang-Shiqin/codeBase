/* 最基础基于SDL+FFMpeg播放器, 能够播放音视频和退出 */

// make
// ./BasicAvPlayer ../../data/human.mp4

#include "av_SDL.h"

int main(int argc, char *argv[]){
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