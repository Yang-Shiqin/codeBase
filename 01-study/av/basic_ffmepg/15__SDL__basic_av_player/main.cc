/* SDL播放音视频 */

extern "C"
{
#include <SDL2/SDL.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define WINDOW_W 1024
#define WINDOW_H 480
#define YUV_W 960
#define YUV_H 400

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

    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. 创建窗口
    SDL_Window* window = SDL_CreateWindow("basic_AV_Player", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Event event;
    while(1){
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            /* code */
            break;
        default:
            break;
        }
    }

    return 0;
}