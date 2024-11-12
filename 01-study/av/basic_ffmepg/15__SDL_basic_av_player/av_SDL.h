#pragma once
#include "av_processor.h"

class Player{
private:
    AvProcessor* processor;
    SDL_Event event;
    int invalid = 0;
    enum ERRNO{
        VIDEO_FRAME_BROKE = 1,
        CREAT_DEMUX_THREAD_FAILED,
        OPEN_AUDIO_FAILED,
        CREAT_TEXTURE_FAILED,
        CREAT_RENDERER_FAILED,
        CREAT_WINDOW_FAILED,
        SDL_INIT_FAILED,
    };
    // video
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    // audio
    SDL_AudioSpec spec;
    int video_display();
public:
    Player(AvProcessor* processor);
    ~Player();
    int play();
};
