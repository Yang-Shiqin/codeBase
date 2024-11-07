/* SDL播放音视频 */

extern "C"
{
#include <stdio.h>
#include <SDL2/SDL.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include "av_queue.h"

#define WINDOW_W 1024
#define WINDOW_H 480
#define YUV_W 960
#define YUV_H 400
#define MAX_AUDIO_FRAME_SIZE 192000


class AvProcessor{
private:
    AVFormatContext *fmt_ctx = nullptr;
    enum ERRNO{
        AVCODEC_SEND_PKT_FAILED = 1,
        SWS_GETCONTEXT_FAILED,
        A_FRAME_ALLOC_FAILED,
        FRAME_BUFFER_ALLOC_FAILED,
        V_FRAME_YUV_ALLOC_FAILED,
        V_FRAME_ALLOC_FAILED,
        A_PACKET_ALLOC_FAILED,
        V_PACKET_ALLOC_FAILED,
        A_CODEC_OPEN_FAILED,
        READ_A_PARA_FAILED,
        V_CODEC_OPEN_FAILED,
        READ_V_PARA_FAILED,
        A_CODEC_NOT_FOUND,
        V_CODEC_NOT_FOUND,
        FIND_BEST_A_STREAM_FAILED,
        FIND_BEST_V_STREAM_FAILED,
        FIND_STREAM_FAILED,
        OPEN_INPUT_FAILED,
    };
    int is_quit = 0;
    // audio
    const AVCodec *a_codec = nullptr;
    AVCodecContext *a_codec_ctx = nullptr;
    AVPacket * a_pkt = nullptr;
    AVFrame * a_frame = nullptr;
    struct SwrContext *swr_ctx = nullptr;
    int a_index;
    AvQueue<AVPacket*> a_pkt_queue;
    uint8_t *audio_chunk = nullptr;    // 用于存放解码后的PCM数据, 给声卡播放
    // video
    const AVCodec *v_codec = nullptr;
    AVCodecContext *v_codec_ctx = nullptr;
    AVPacket * v_pkt = nullptr;
    AVFrame * v_frame = nullptr;
    struct SwsContext *sws_ctx = nullptr;
    int v_index;
    AvQueue<AVPacket*> v_pkt_queue;
    AvQueue<AVFrame*> v_frame_queue;
public:
    int invalid = 0;  // 错误处理, 0: valid, >0: invalid
    AvProcessor(const char *src);
    ~AvProcessor();
    int demux();
    static int decode_video_thread(void* data){ // 静态成员函数作为创建线程的入口
        return ((AvProcessor*)data)->decode_video();
    }
    int decode_video();
    int decode_audio();
};

// [ ] TODO: src输入其实不太好
AvProcessor::AvProcessor(const char *src){
    int ret;
    // 1. 打开输入视频文件
    ret = avformat_open_input(&(this->fmt_ctx), src, nullptr, nullptr);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "open input failed\n");
        this->invalid = OPEN_INPUT_FAILED;
        return;
    }
    // 2. 获取视频流详细信息并填入fmt_ctx中
    if(avformat_find_stream_info(this->fmt_ctx, nullptr) < 0){
        av_log(nullptr, AV_LOG_ERROR, "Could not find stream information\n");
        this->invalid = FIND_STREAM_FAILED;
        return;
    }

    // 3. 从输入文件中获取流stream，v_index为视频流索引，a_index为音频流索引
    this->v_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (this->v_index < 0){
        av_log(nullptr, AV_LOG_ERROR, "finding best stream failed\n");
        this->invalid = FIND_BEST_V_STREAM_FAILED;
        return;
    }
    this->a_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (this->a_index < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "finding best stream failed\n");
        this->invalid = FIND_BEST_A_STREAM_FAILED;
        return;
    }

    // 4. 查找视频、音频流对应的编码器并分配上下文
    this->v_codec = avcodec_find_decoder(this->fmt_ctx->streams[this->v_index]->codecpar->codec_id);
    if (!this->v_codec){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
        this->invalid = V_CODEC_NOT_FOUND;
        return;
    }
    this->v_codec_ctx = avcodec_alloc_context3(this->v_codec);

    this->a_codec = avcodec_find_decoder(this->fmt_ctx->streams[this->a_index]->codecpar->codec_id);
    if (!this->a_codec){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
        this->invalid = A_CODEC_NOT_FOUND;
        return;
    }
    this->v_codec_ctx = avcodec_alloc_context3(this->a_codec);

    // 5. 读取视频、音频流参数到编码器上下文, 打开编码器
    ret = avcodec_parameters_to_context(this->v_codec_ctx, this->fmt_ctx->streams[this->v_index]->codecpar);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_V_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->v_codec_ctx, this->v_codec, nullptr);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_open2 failed\n");
        this->invalid = V_CODEC_OPEN_FAILED;
        return;
    }

    ret = avcodec_parameters_to_context(this->a_codec_ctx, this->fmt_ctx->streams[this->a_index]->codecpar);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_A_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->a_codec_ctx, this->a_codec, nullptr);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_open2 failed\n");
        this->invalid = A_CODEC_OPEN_FAILED;
        return;
    }

    // 6. 初始化包结构以存放读入的packet
    this->v_pkt = av_packet_alloc();
    if (!this->v_pkt){
        av_log(nullptr, AV_LOG_ERROR, "pkt alloc failed\n");
        this->invalid = V_PACKET_ALLOC_FAILED;
        return;
    }
    this->a_pkt = av_packet_alloc();
    if (!this->a_pkt){
        av_log(nullptr, AV_LOG_ERROR, "pkt alloc failed\n");
        this->invalid = A_PACKET_ALLOC_FAILED;
        return;
    }

    // 7. 初始化帧结构以存放读入的frame
    // 7.1 初始化存放解码pkt后的帧结构
    this->v_frame = av_frame_alloc();
    if (!this->v_frame){
        av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = V_FRAME_ALLOC_FAILED;
        return;
    }
    // 7.2 初始化存放裁剪后的帧结构, 设置初值和分配缓冲区
    // this->v_frame_yuv = av_frame_alloc(); // 用于存放裁剪后输出
    // if (!this->v_frame_yuv){
    //     av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
    //     this->invalid = V_FRAME_YUV_ALLOC_FAILED;
    //     return;
    // }
    // this->v_frame_yuv->format = AV_PIX_FMT_YUV420P;      // 设置目标像素格式
    // this->v_frame_yuv->width = this->v_codec_ctx->width;         // 设置目标宽度
    // this->v_frame_yuv->height = this->v_codec_ctx->height;       // 设置目标高度
    // ret = av_frame_get_buffer(this->v_frame_yuv, 32);    // 分配目标帧缓冲区
    // if (ret < 0){
    //     av_log(nullptr, AV_LOG_ERROR, "av_frame_get_buffer failed\n");
    //     this->invalid = FRAME_BUFFER_ALLOC_FAILED;
    //     return;
    // }
    this->a_frame = av_frame_alloc();
    if (!this->a_frame){
        av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = A_FRAME_ALLOC_FAILED;
        return;
    }

    this->audio_chunk = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE);

    // 8. 初始化缩放上下文和音频格式转换上下文
    this->sws_ctx = sws_getContext(this->v_codec_ctx->width, this->v_codec_ctx->height, this->v_codec_ctx->pix_fmt, 
        this->v_codec_ctx->width, this->v_codec_ctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!this->sws_ctx){
        av_log(nullptr, AV_LOG_ERROR, "sws_getContext failed\n");
        this->invalid = SWS_GETCONTEXT_FAILED;
        return;
    }

    int64_t channel_layout = av_get_default_channel_layout(this->a_codec_ctx->channels);
    this->swr_ctx = swr_alloc_set_opts(
        nullptr, 
        channel_layout, 
        AV_SAMPLE_FMT_S16, 
        this->a_codec_ctx->sample_rate,
        channel_layout,
        this->a_codec_ctx->sample_fmt, 
        this->a_codec_ctx->sample_rate, 0, nullptr);
    if (!this->swr_ctx || swr_init(this->swr_ctx) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "swr_getContext failed\n");
        this->invalid = SWR_GETCONTEXT_FAILED;
        return;
    }
}

// [ ] TODO:
AvProcessor::~AvProcessor(){
    switch (this->invalid)
    {
    case 0:
    case AVCODEC_SEND_PKT_FAILED:
    case SWS_GETCONTEXT_FAILED:
    case A_FRAME_ALLOC_FAILED:
    case FRAME_BUFFER_ALLOC_FAILED:
    case V_FRAME_YUV_ALLOC_FAILED:
    case V_FRAME_ALLOC_FAILED:
    case A_PACKET_ALLOC_FAILED:
    case V_PACKET_ALLOC_FAILED:
    case A_CODEC_OPEN_FAILED:
    case READ_A_PARA_FAILED:
    case V_CODEC_OPEN_FAILED:
    case READ_V_PARA_FAILED:
    case A_CODEC_NOT_FOUND:
    case V_CODEC_NOT_FOUND:
    case FIND_BEST_A_STREAM_FAILED:
    case FIND_BEST_V_STREAM_FAILED:
    case FIND_STREAM_FAILED:
        avformat_close_input(&(this->fmt_ctx));
    case OPEN_INPUT_FAILED:;
    }
}

int AvProcessor::demux(){
    if (this->invalid){
        return this->invalid;
    }
    // 创建视频解码线程
    SDL_Thread *video_tid = SDL_CreateThread(decode_video_thread, "decode_video_thread", this);
    while(1){
        if (this->is_quit){
            // 唤醒解码线程以退出
            break;
        }
        AVPacket *pkt = av_packet_alloc();
        if (av_read_frame(this->fmt_ctx, pkt) < 0){
            if(this->fmt_ctx->pb->error == 0) {
                SDL_Delay(100); /* no error; wait for user input */
                continue;
            } else {    // 读完了
                break;
            }
        }else{
            if (pkt->stream_index == this->v_index){
                this->v_pkt_queue.push(pkt);
            }else if (pkt->stream_index == this->a_index){
                this->a_pkt_queue.push(pkt);
            }
        }
    }
}

int AvProcessor::decode_video(){
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    // if (!frame){
    //     av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
    //     return (this->invalid = DECODE_V_FRAME_ALLOC_FAILED);
    // }
    while(1){
        // 9.1 读取packet
        pkt = this->v_pkt_queue.pop();
        // 9.2 发送packet到解码器
        if (avcodec_send_packet(this->v_codec_ctx, pkt)){
            av_log(nullptr, AV_LOG_ERROR, "avcodec_send_packet failed\n");
            return (this->invalid = AVCODEC_SEND_PKT_FAILED);
        }
        // 9.3 从解码器接收解码后的帧
        while(!avcodec_receive_frame(this->v_codec_ctx, this->v_frame)){
            frame = av_frame_alloc();
            frame->format = AV_PIX_FMT_YUV420P;      // 设置目标像素格式
            frame->width = this->v_codec_ctx->width;         // 设置目标宽度
            frame->height = this->v_codec_ctx->height;       // 设置目标高度
            av_frame_get_buffer(frame, 32);    // 分配目标帧缓冲区
            // 9.4 裁剪黑边
            sws_scale(this->sws_ctx, (const uint8_t* const*)this->v_frame->data, this->v_frame->linesize, 0, 
                this->v_codec_ctx->height, frame->data, frame->linesize);
            v_frame_queue.push(frame);
        }
        // 9.6 解引用packet
        av_packet_unref(pkt);
    }
    return 0;
}

int AvProcessor::decode_audio(){
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    // if (!frame){
    //     av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
    //     return (this->invalid = DECODE_V_FRAME_ALLOC_FAILED);
    // }
    while(1){
        // 9.1 读取packet
        pkt = this->a_pkt_queue.pop();
        // 9.2 发送packet到解码器
        if (avcodec_send_packet(this->a_codec_ctx, pkt)){
            av_log(nullptr, AV_LOG_ERROR, "avcodec_send_packet failed\n");
            return (this->invalid = AVCODEC_SEND_PKT_FAILED);
        }
        // 9.3 从解码器接收解码后的帧
        while(!avcodec_receive_frame(this->a_codec_ctx, this->a_frame)){
            // 9.4 格式转换
            swr_convert(this->swr_ctx, &(this->audio_chunk), MAX_AUDIO_FRAME_SIZE, this->a_frame->data, this->a_frame->nb_samples);
        }
        // 9.6 解引用packet
        av_packet_unref(pkt);
    }
    return 0;
}