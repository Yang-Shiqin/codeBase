/* SDL显示yuv数据 */

#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_W 1024
#define WINDOW_H 480
#define YUV_W 960
#define YUV_H 400

int main(int argc, char* argv[]){
    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. 创建窗口
    SDL_Window* window = SDL_CreateWindow("simplest_YUV_Player", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 3. 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 4. 创建纹理
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, 
        SDL_TEXTUREACCESS_STREAMING, YUV_W, YUV_H);
    if (!texture) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 5. 打开文件
    FILE *fp = fopen("../data/fly.yuv", "rb");
    if (!fp) {
        printf("YUV file could not be opened!\n");
        return 1;
    }

    // YUV数据buf
    uint8_t *y_plane = (uint8_t *)malloc(YUV_W*YUV_H);
    uint8_t *u_plane = (uint8_t *)malloc(YUV_W*YUV_H/4);
    uint8_t *v_plane = (uint8_t *)malloc(YUV_W*YUV_H/4);

    // 6. 渲染循环
    SDL_Event event;
    int running = 1;
    while(running){
        // 事件处理
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = 0;
            }
        }

        // 6.1 读取YUV数据
        fread(y_plane, 1, YUV_W*YUV_H, fp);
        fread(u_plane, 1, YUV_W*YUV_H/4, fp);
        fread(v_plane, 1, YUV_W*YUV_H/4, fp);

        // 6.2 更新纹理
        SDL_UpdateYUVTexture(texture, NULL, y_plane, YUV_W, u_plane, YUV_W/2, v_plane, YUV_W/2);

        // 6.3 清空渲染器
        SDL_RenderClear(renderer);

        // 6.4 拷贝纹理到渲染器
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // 6.5 显示
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    // 7. 释放资源
    fclose(fp);
    free(y_plane);
    free(u_plane);
    free(v_plane);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}