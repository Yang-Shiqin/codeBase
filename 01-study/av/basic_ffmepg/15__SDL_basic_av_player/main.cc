/* SDL播放音视频 */

/*
- [ ] 错误处理
- [ ] SDL封装
- [ ] 注释
- [ ] 笔记
*/

#include "av_processor.h"
#include <iostream>

void read_audio_data(void *udata, Uint8 *stream, int len){
    AvProcessor* processor = (AvProcessor*)udata;
    processor->audio_chunk_pop(stream, len);
}

static Uint32 video_timer(Uint32 interval, void *opaque) {
  SDL_Event event;    // 初始化事件
  event.type = SDL_USEREVENT;  // 事件类型
  event.user.data1 = opaque;
  SDL_PushEvent(&event);
  return interval;
}

void display(AvProcessor *processor, SDL_Texture* texture, SDL_Renderer* renderer){
    AVFrame* frame = nullptr;
    frame = processor->video_frame_pop();
    if (!frame) {
        av_log(NULL, AV_LOG_ERROR, "frame is NULL\n");
        return;
    }

    // 6.2 更新纹理
    SDL_UpdateYUVTexture(texture, NULL, frame->data[0], frame->linesize[0], 
        frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);

    // 6.3 清空渲染器
    SDL_RenderClear(renderer);

    // 6.4 拷贝纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // 6.5 显示
    SDL_RenderPresent(renderer);
    // SDL_Delay(30);
    av_freep(&frame);
}

int main(int argc, char *argv[]){
    int h, w;

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

    // 创建AvProcessor对象
    AvProcessor processor(src);
    h = processor.get_h();
    w = processor.get_w();
    av_log(NULL, AV_LOG_INFO, "h: %d, w: %d\n", h, w);

    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. 初始化视频相关
    // 2.1 创建窗口
    SDL_Window* window = SDL_CreateWindow("basic_AV_Player", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (!window) {
        av_log(NULL, AV_LOG_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2.2 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        av_log(NULL, AV_LOG_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2.3 创建纹理
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, 
        SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!texture) {
        av_log(NULL, AV_LOG_ERROR, "Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 3. 初始化音频相关
    SDL_AudioSpec spec;
    // 3.1 设置参数(回调函数是因为声卡是拉数据而不是我们推给他)
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 2048;
    spec.callback = read_audio_data;
    spec.userdata = &processor;
    // 3.2 打开音频设备
    if(SDL_OpenAudio(&spec, NULL)){
        av_log(NULL, AV_LOG_ERROR, "Failed to open audio device, %s\n", SDL_GetError());
        return 1;
    }
    // 3.3 播放音频
    SDL_PauseAudio(0);  // 播放音频(非0是暂停, 0是播放)

    SDL_Thread* demux_tid = SDL_CreateThread(AvProcessor::demux_thread, "demux_thread", &processor);
    if (!demux_tid) {
        av_log(NULL, AV_LOG_ERROR, "SDL_CreateThread failed\n");
        return 1;
    }

    SDL_AddTimer(40, video_timer, &processor);
    SDL_Event event;
    int running = 1;
    while(running){
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            processor.is_quit = 1;
            running = 0;
            /* code */
            break;
        case SDL_KEYDOWN:
            /* code */
            break;
        case SDL_USEREVENT:
            // 6.1 读取YUV数据
            display(&processor, texture, renderer);
            break;
        default:
            break;
        }
    }
    // 7. 释放资源
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}