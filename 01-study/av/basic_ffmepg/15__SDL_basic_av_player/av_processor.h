#pragma once
#include "av_queue.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

class AvProcessor{
private:
    AVFormatContext *fmt_ctx = nullptr;
    enum ERRNO{
        GET_BYTES_FAILED = 1,
        AVCODEC_SEND_PKT_FAILED,
        SWR_GETCONTEXT_FAILED,
        SWS_GETCONTEXT_FAILED,
        A_FRAME_ALLOC_FAILED,
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
    // audio
    const AVCodec *a_codec = nullptr;
    AVCodecContext *a_codec_ctx = nullptr;
    AVPacket * a_pkt = nullptr;
    AVFrame * a_frame = nullptr;
    struct SwrContext *swr_ctx = nullptr;
    int a_index;
    AvQueue<AVPacket*> a_pkt_queue;
    AvBufferQueue<uint8_t> audio_chunk;    // 用于存放解码后的PCM数据, 给声卡播放
    // uint8_t *audio_chunk = nullptr;    // 用于存放解码后的PCM数据, 给声卡播放
    // video
    int h, w;
    const AVCodec *v_codec = nullptr;
    AVCodecContext *v_codec_ctx = nullptr;
    AVPacket * v_pkt = nullptr;
    AVFrame * v_frame = nullptr;
    struct SwsContext *sws_ctx = nullptr;
    int v_index;
    AvQueue<AVPacket*> v_pkt_queue;
    AvQueue<AVFrame*> v_frame_queue;
public:
    int is_quit = 0;
    int invalid = 0;  // 错误处理, 0: valid, >0: invalid
    AvProcessor(const char *src);
    ~AvProcessor();
    static int demux_thread(void* data){ // 静态成员函数作为创建线程的入口
        return ((AvProcessor*)data)->demux();
    }
    static int decode_video_thread(void* data){ // 静态成员函数作为创建线程的入口
        return ((AvProcessor*)data)->decode_video();
    }
    static int decode_audio_thread(void* data){ // 静态成员函数作为创建线程的入口
        return ((AvProcessor*)data)->decode_audio();
    }
    int demux();            // 解复用
    int decode_video();     // 视频解码
    int decode_audio();     // 音频解码
    void audio_chunk_pop(uint8_t *stream, int len);
    AVFrame* video_frame_pop();
    int get_h(){ return this->h; }
    int get_w(){ return this->w; }
};