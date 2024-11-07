/* SDL播放pcm数据 */

// gcc 09__SDL_pcm.c -lSDL2
// ./a.out

#include <SDL2/SDL.h>
#include <stdio.h>

#define BLOCK_SIZE 4096000
static uint8_t *audio_pos = NULL;
static size_t buffer_len = 0;

void read_audio_data(void *udata, Uint8 *stream, int len){
    if(buffer_len == 0){
        return;
    }

    SDL_memset(stream, 0, len);

    len = (len < buffer_len) ? len : buffer_len;
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

    audio_pos += len;
    buffer_len -= len;
}

int main(int argc, char* argv[]){
    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. 打开音频设备
    SDL_AudioSpec spec;
    // 2.1 设置参数(回调函数是因为声卡是拉数据而不是我们推给他)
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 2048;
    spec.callback = read_audio_data;
    spec.userdata = NULL;
    // 2.2 打开音频设备
    if(SDL_OpenAudio(&spec, NULL)){
        fprintf(stderr, "Failed to open audio device, %s\n", SDL_GetError());
        goto close_sdl;
    }
    // 2.3 播放音频
    SDL_PauseAudio(0);  // 播放音频(非0是暂停, 0是播放)

    // pcm数据buf
    uint8_t *audio_chunk = (uint8_t *)malloc(BLOCK_SIZE+1);
    if (!audio_chunk) {
        printf("Could not allocate audio chunk buffer!\n");
        goto close_audio;
    }

    // 3. 打开文件
    FILE *fp = fopen("../data/fly.pcm", "r");
    if (!fp) {
        printf("PCM file could not be opened!\n");
        goto free_buf;
    }

    // 4. 播放音频
    int read_len;
    do {
        read_len = fread(audio_chunk, 1, BLOCK_SIZE, fp);
        buffer_len = read_len;
        audio_pos = audio_chunk;
        // 等待声卡播放完毕
        while(buffer_len > 0){
            SDL_Delay(100);
        }
    } while (read_len != 0);

    // 7. 释放资源
    fclose(fp);
free_buf:
    free(audio_chunk);
close_audio:
    SDL_CloseAudio();
close_sdl:
    SDL_Quit();
    return 0;
}