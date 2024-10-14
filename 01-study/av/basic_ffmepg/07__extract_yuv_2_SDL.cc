/* SDL显示yuv数据 */

extern "C" {
    #include <SDL2/SDL.h>
    #include <stdio.h>
    #include <libavutil/avutil.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

#define WINDOW_W 1024
#define WINDOW_H 480
#define YUV_W 960
#define YUV_H 400

class ExtractYUV{
public:
    int invalid = 0;  // 0: valid, >0: invalid
    ExtractYUV(const char *src, int h, int w);
    ~ExtractYUV();
    int extract_yuv(uint8_t *y_plane, uint8_t *u_plane, uint8_t *v_plane);
private:
    AVFormatContext *fmt_ctx = NULL;
    AVCodec *codec = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVPacket * pkt = NULL;
    AVFrame * frame = NULL;
    AVFrame * frame_yuv = NULL;
    struct SwsContext *sws_ctx = NULL;
    int video_index;
    int h, w;
    int step = 0;   // 模拟生成器
    enum ERRNO{
        AVCODEC_SEND_PKT_FAILED = 1,
        SWS_GETCONTEXT_FAILED,
        FRAME_BUFFER_ALLOC_FAILED,
        FRAME_YUV_ALLOC_FAILED,
        FRAME_ALLOC_FAILED,
        PACKET_ALLOC_FAILED,
        CODEC_OPEN_FAILED,
        READ_PARA_FAILED,
        CODEC_NOT_FOUND,
        FIND_BEST_STREAM_FAILED,
        FIND_STREAM_FAILED,
        OPEN_INPUT_FAILED,
    };
};

ExtractYUV::ExtractYUV(const char *src, int h, int w):h(h), w(w){
    int ret;
    int l;
    // 1. 打开输入视频文件
    ret = avformat_open_input(&(this->fmt_ctx), src, NULL, NULL);
    if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "open input failed\n");
        this->invalid = OPEN_INPUT_FAILED;
        return;
    }
    // 2. 获取视频流详细信息并填入fmt_ctx中
    if(avformat_find_stream_info(this->fmt_ctx, NULL) < 0){
        av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
        this->invalid = FIND_STREAM_FAILED;
        return;
    }

    // 3. 从输入文件中获取流stream，video_index为视频流索引
    this->video_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (this->video_index < 0){
        av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
        this->invalid = FIND_BEST_STREAM_FAILED;
        return;
    }

    // 4. 查找流对应的编码器并分配上下文
    this->codec = avcodec_find_decoder(this->fmt_ctx->streams[this->video_index]->codecpar->codec_id);
    if (!this->codec){
        av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
        this->invalid = CODEC_NOT_FOUND;
        return;
    }
    this->codec_ctx = avcodec_alloc_context3(this->codec);

    // 5. 读取流参数到编码器上下文, 打开编码器
    ret = avcodec_parameters_to_context(this->codec_ctx, this->fmt_ctx->streams[this->video_index]->codecpar);
    if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->codec_ctx, this->codec, NULL);
    if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "avcodec_open2 failed\n");
        this->invalid = CODEC_OPEN_FAILED;
        return;
    }

    // 6. 初始化包结构以存放读入的packet
    this->pkt = av_packet_alloc();
    if (!this->pkt){
        av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
        this->invalid = PACKET_ALLOC_FAILED;
        return;
    }

    // 7. 初始化帧结构以存放读入的frame
    // 7.1 初始化存放解码pkt后的帧结构
    this->frame = av_frame_alloc();
    if (!this->frame){
        av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = FRAME_ALLOC_FAILED;
        return;
    }
    // 7.2 初始化存放裁剪后的帧结构, 设置初值和分配缓冲区
    this->frame_yuv = av_frame_alloc(); // 用于存放裁剪后输出
    if (!this->frame_yuv){
        av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = FRAME_YUV_ALLOC_FAILED;
        return;
    }
    this->frame_yuv->format = AV_PIX_FMT_YUV420P;      // 设置目标像素格式
    this->frame_yuv->width = this->codec_ctx->width;         // 设置目标宽度
    this->frame_yuv->height = this->codec_ctx->height;       // 设置目标高度
    ret = av_frame_get_buffer(this->frame_yuv, 32);    // 分配目标帧缓冲区
    if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "av_frame_get_buffer failed\n");
        this->invalid = FRAME_BUFFER_ALLOC_FAILED;
        return;
    }

    // 8. 初始化缩放上下文
    this->sws_ctx = sws_getContext(this->codec_ctx->width, this->codec_ctx->height, this->codec_ctx->pix_fmt, 
        this->codec_ctx->width, this->codec_ctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    if (!sws_ctx){
        av_log(NULL, AV_LOG_ERROR, "sws_getContext failed\n");
        this->invalid = SWS_GETCONTEXT_FAILED;
        return;
    }
}

ExtractYUV::~ExtractYUV(){
    switch (this->invalid){
    case 0:
    case AVCODEC_SEND_PKT_FAILED:
        sws_freeContext(sws_ctx);
    case SWS_GETCONTEXT_FAILED:
    case FRAME_BUFFER_ALLOC_FAILED:
        av_frame_free(&frame_yuv);
    case FRAME_YUV_ALLOC_FAILED:
        av_frame_free(&frame);
    case FRAME_ALLOC_FAILED:
        av_packet_free(&pkt);
    case PACKET_ALLOC_FAILED:
    case CODEC_OPEN_FAILED:
    case READ_PARA_FAILED:
        avcodec_free_context(&codec_ctx);
    case CODEC_NOT_FOUND:
    case FIND_BEST_STREAM_FAILED:
    case FIND_STREAM_FAILED:
        avformat_close_input(&fmt_ctx);
    case OPEN_INPUT_FAILED:;
    }
}

int ExtractYUV::extract_yuv(uint8_t *y_plane, uint8_t *u_plane, uint8_t *v_plane){
    if (step==1){
        goto step1;
    }
    while(av_read_frame(this->fmt_ctx, this->pkt) >= 0){
        if (this->pkt->stream_index == this->video_index){  // 只处理目标视频流
            // 9.1 处理packet
            if (strstr(this->fmt_ctx->iformat->name, "avi") || strstr(this->fmt_ctx->iformat->name, "ts")){
                // avi、mpegts都可以直接写入数据变成h264播放
                // 9.2 发送packet到解码器
                if (avcodec_send_packet(this->codec_ctx, this->pkt)){
                    av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
                    return (this->invalid = AVCODEC_SEND_PKT_FAILED);
                }
                // 9.3 从解码器接收解码后的帧
step1:
                while(!avcodec_receive_frame(this->codec_ctx, this->frame)){
                    // 9.4 裁剪黑边
                    sws_scale(this->sws_ctx, (const uint8_t* const*)this->frame->data, this->frame->linesize, 0, 
                        this->codec_ctx->height, this->frame_yuv->data, this->frame_yuv->linesize);
                    // 9.5 保存YUV数据
                    memcpy(y_plane, this->frame_yuv->data[0], this->h*this->w);    // Y
                    memcpy(u_plane, this->frame_yuv->data[1], this->h*this->w/4);  // U
                    memcpy(v_plane, this->frame_yuv->data[2], this->h*this->w/4);  // V
                    return 0;
                }
            }else{
                av_log(NULL, AV_LOG_INFO, "%s not support yet\n", fmt_ctx->iformat->name);
            }
        }
        // 9.6 解引用packet
        av_packet_unref(pkt);
    }
    return 0;
}

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
    ExtractYUV ex_yuv("../data/fly.avi", YUV_H, YUV_W);

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
        ex_yuv.extract_yuv(y_plane, u_plane, v_plane);

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
    free(y_plane);
    free(u_plane);
    free(v_plane);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}