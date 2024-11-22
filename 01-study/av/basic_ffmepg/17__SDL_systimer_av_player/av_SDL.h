#pragma once
#include "av_processor.h"

class Player{
private:
    AvProcessor* processor; // 音视频处理类
    SDL_Event event;
    int invalid = 0;
    enum ERRNO{ // 错误码
        VIDEO_FRAME_BROKE = 1,
        CREAT_DEMUX_THREAD_FAILED,
        OPEN_AUDIO_FAILED,
        CREAT_TEXTURE_FAILED,
        CREAT_RENDERER_FAILED,
        CREAT_WINDOW_FAILED,
        SDL_INIT_FAILED,
    };
    // 播放器时钟(单位为秒), 相比系统时钟多了快退快进和暂停的时间差, 暂停是-暂停+继续时刻, 快进是-, 快退是+
    // 即含义为正常播放时, 在系统时间系中的开始时间戳
    int64_t player_clock;
    // video
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    AVFrame* frame = nullptr;
    // audio
    SDL_AudioSpec spec;
    int timer_video_display();              // 定时显示视频
    int video_display(AVFrame* frame);      // 显示视频
    double get_player_clock();              // 获取播放器时钟
    void pause_player_clock(int is_pause);  // 暂停播放器时钟
public:
    Player(AvProcessor* processor);
    ~Player();
    int play(); // 同步播放音视频
};
