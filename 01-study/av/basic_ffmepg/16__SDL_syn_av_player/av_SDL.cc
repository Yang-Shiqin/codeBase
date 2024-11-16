#include "av_SDL.h"

// 音频数据回调函数
void read_audio_data(void *udata, Uint8 *stream, int len){
    AvProcessor* processor = (AvProcessor*)udata;
    processor->audio_chunk_pop(stream, len);
}

// 视频定时器
static Uint32 video_timer(Uint32 interval, void *opaque) {
  SDL_Event event;    // 初始化事件
  event.type = SDL_USEREVENT;  // 事件类型
  event.user.data1 = opaque;
  SDL_PushEvent(&event);
  return 0; // 1次触发后不会再次触发
}

Player::Player(AvProcessor* processor):processor(processor){
    int h, w;
    h = this->processor->get_h();
    w = this->processor->get_w();
    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        this->invalid = SDL_INIT_FAILED;
        return;
    }

    // 2. 初始化视频相关
    // 2.1 创建窗口
    this->window = SDL_CreateWindow("basic_AV_Player", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (!this->window) {
        av_log(NULL, AV_LOG_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        this->invalid = CREAT_WINDOW_FAILED;
        return;
    }

    // 2.2 创建渲染器
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    if (!this->renderer) {
        av_log(NULL, AV_LOG_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        this->invalid = CREAT_RENDERER_FAILED;
        return;
    }

    // 2.3 创建纹理
    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_IYUV, 
        SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!this->texture) {
        av_log(NULL, AV_LOG_ERROR, "Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        this->invalid = CREAT_TEXTURE_FAILED;
        return;
    }

    // 3. 初始化音频相关
    // 3.1 设置参数(回调函数是因为声卡是拉数据而不是我们推给他)
    this->spec.freq = 48000;    // [ ] TODO: 
    this->spec.format = AUDIO_S16SYS;
    this->spec.channels = 2;
    this->spec.silence = 0;
    this->spec.samples = 2048;
    this->spec.callback = read_audio_data;
    this->spec.userdata = this->processor;
    // 3.2 打开音频设备
    if(SDL_OpenAudio(&this->spec, NULL)){
        av_log(NULL, AV_LOG_ERROR, "Failed to open audio device, %s\n", SDL_GetError());
        this->invalid = OPEN_AUDIO_FAILED;
        return;
    }
}

Player::~Player(){
    switch (this->invalid){
    case 0: 
    case VIDEO_FRAME_BROKE:
    case CREAT_DEMUX_THREAD_FAILED:
    case OPEN_AUDIO_FAILED:
        SDL_DestroyTexture(texture);
    case CREAT_TEXTURE_FAILED:
        SDL_DestroyRenderer(renderer);
    case CREAT_RENDERER_FAILED:
        SDL_DestroyWindow(window);
    case CREAT_WINDOW_FAILED:
        SDL_Quit();
    case SDL_INIT_FAILED:;
    }
}

int Player::play(){
    // 1. 创建解复用线程
    SDL_Thread* demux_tid = SDL_CreateThread(AvProcessor::demux_thread, "demux_thread", this->processor);
    if (!demux_tid) {
        av_log(NULL, AV_LOG_ERROR, "SDL_CreateThread demux_thread failed\n");
        return (this->invalid = CREAT_DEMUX_THREAD_FAILED);
    }
    // 2. 播放音频
    SDL_PauseAudio(0);  // 播放音频(非0是暂停, 0是播放)
    // 3. 创建视频播放定时器
    SDL_AddTimer(40, video_timer, this->processor);
    // 4. 事件循环
    int running = 1;
    while(running){
        SDL_WaitEvent(&this->event);
        switch (this->event.type)
        {
        case SDL_QUIT:  // 退出事件
            this->processor->stop();
            SDL_WaitThread(demux_tid, nullptr);
            running = 0;
            break;
        case SDL_KEYDOWN:   // 键盘事件
            /* code */
            break;
        case SDL_USEREVENT: // 视频定时播放事件
            this->timer_video_display();
            break;
        default:
            break;
        }
    }
    return 0;
}

int Player::timer_video_display(){
    // 1. 从队列中取出视频帧
    if (this->frame==nullptr)
        this->frame = this->processor->video_frame_pop();
    if (!this->frame) {
        av_log(NULL, AV_LOG_ERROR, "frame is NULL\n");
        return (this->invalid = VIDEO_FRAME_BROKE);
    }
    double delay = this->processor->get_video_clock(this->frame) - this->processor->get_audio_clock();
    
    if (delay <= 0){
        this->video_display(this->frame);
        this->frame = nullptr;
        SDL_AddTimer(1, video_timer, this->processor);
    }else{
        SDL_AddTimer((int)(delay*1000+0.5), video_timer, this->processor);
    }
    return 0;
}

// 播放一帧视频
int Player::video_display(AVFrame* frame){
    // 2. 更新纹理
    SDL_UpdateYUVTexture(this->texture, NULL, frame->data[0], frame->linesize[0], 
        frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
    // 3. 清空渲染器
    SDL_RenderClear(this->renderer);
    // 4. 拷贝纹理到渲染器
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    // 5. 显示
    SDL_RenderPresent(this->renderer);
    // 6. 释放帧
    av_freep(&frame);
    return 0;
}