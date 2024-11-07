// 很玄学，有时可以正常播，有时卡顿（所以卡顿不一定是我们的原因

/* SDL播放ffmpeg从avi提取的pcm数据(08+09, 08作为一个类) */

// g++ 10__extract_pcm_2_SDL.cc -lSDL2 -lavformat -lavcodec -lavutil -lswresample
// ./a.out ../data/fly.avi

extern "C"
{
#include <stdio.h>
#include <SDL2/SDL.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
// #include <stdio.h>
}

#define MAX_AUDIO_FRAME_SIZE 192000
// 声卡队列(必须是全局或静态的)
static uint8_t *audio_pos = nullptr;
static size_t buffer_len = 0;

class ExtractPCM
{
public:
    int invalid = 0; // 错误处理, 0: valid, >0: invalid
    ExtractPCM(const char *src);
    ~ExtractPCM();
    int extract_pcm(uint8_t *audio_chunk, size_t* buffer_len);  // 解码音频

private:
    AVFormatContext *fmt_ctx = nullptr;
    const AVCodec *codec = nullptr;
    AVCodecContext *codec_ctx = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    int audio_index;
    struct SwrContext *swr_ctx = nullptr;
    int step = 0; // 模拟生成器
    enum ERRNO
    {
        GET_BYTES_FAILED = 1,
        AVCODEC_SEND_PKT_FAILED,
        SWR_GETCONTEXT_FAILED,
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

ExtractPCM::ExtractPCM(const char *src)
{
    // 1. 打开输入视频文件
    int ret;
    ret = avformat_open_input(&(this->fmt_ctx), src, nullptr, nullptr);
    if (ret < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "open input failed\n");
        this->invalid = OPEN_INPUT_FAILED;
        return;
    }
    // 2. 获取视频流详细信息并填入fmt_ctx中
    ret = avformat_find_stream_info(this->fmt_ctx, nullptr);
    if (ret < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "Could not find stream information\n");
        this->invalid = FIND_STREAM_FAILED;
        return;
    }
    // 3. 从输入文件中获取流stream，audio_index为音频流索引
    this->audio_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (this->audio_index < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "finding best stream failed\n");
        this->invalid = FIND_BEST_STREAM_FAILED;
        return;
    }

    // 4. 查找流对应的解码器并分配上下文
    this->codec = avcodec_find_decoder(this->fmt_ctx->streams[this->audio_index]->codecpar->codec_id);
    if (!this->codec)
    {
        av_log(nullptr, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
        this->invalid = CODEC_NOT_FOUND;
        return;
    }
    this->codec_ctx = avcodec_alloc_context3(this->codec);

    // 5. 读取流参数到解码器上下文, 打开解码器
    ret = avcodec_parameters_to_context(this->codec_ctx, this->fmt_ctx->streams[this->audio_index]->codecpar);
    if (ret < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->codec_ctx, this->codec, nullptr);
    if (ret < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "avcodec_open2 failed\n");
        this->invalid = CODEC_OPEN_FAILED;
        return;
    }

    // 6. 初始化包结构以存放读入的packet
    this->pkt = av_packet_alloc();
    if (!this->pkt)
    {
        av_log(nullptr, AV_LOG_ERROR, "pkt alloc failed\n");
        this->invalid = PACKET_ALLOC_FAILED;
        return;
    }
    // 7. 初始化帧结构以存放读入的frame
    this->frame = av_frame_alloc();
    if (!this->frame)
    {
        av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = FRAME_ALLOC_FAILED;
        return;
    }
    // 8. 初始化重采样上下文
    int64_t channel_layout = av_get_default_channel_layout(this->codec_ctx->channels);
    this->swr_ctx = swr_alloc_set_opts(
        nullptr, 
        channel_layout, 
        AV_SAMPLE_FMT_S16, 
        this->codec_ctx->sample_rate,
        channel_layout,
        this->codec_ctx->sample_fmt, 
        this->codec_ctx->sample_rate, 0, nullptr);
    if (!this->swr_ctx || swr_init(this->swr_ctx) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "swr_getContext failed\n");
        this->invalid = SWR_GETCONTEXT_FAILED;
        return;
    }
}

ExtractPCM::~ExtractPCM()
{
    switch (this->invalid)
    {
    case 0:
    case GET_BYTES_FAILED:
    case AVCODEC_SEND_PKT_FAILED:
        av_frame_free(&(this->frame));
        swr_free(&(this->swr_ctx));
    case SWR_GETCONTEXT_FAILED:
    case FRAME_ALLOC_FAILED:
        av_packet_free(&pkt);
    case PACKET_ALLOC_FAILED:
    case CODEC_OPEN_FAILED:
    case READ_PARA_FAILED:
        avcodec_free_context(&(this->codec_ctx));
    case CODEC_NOT_FOUND:
    case FIND_BEST_STREAM_FAILED:
    case FIND_STREAM_FAILED:
        avformat_close_input(&(this->fmt_ctx));
    case OPEN_INPUT_FAILED:;
    }
}

int ExtractPCM::extract_pcm(uint8_t *audio_chunk, size_t* buffer_len)
{
    static uint8_t *audio_chunk_ptr = nullptr;
    static int data_size = 0;
    audio_chunk_ptr = audio_chunk;
    if (this->step == 1)
    {
        goto step1;
    }
    // 8. 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
    while (av_read_frame(this->fmt_ctx, this->pkt) >= 0)
    {
        if (this->pkt->stream_index == this->audio_index)
        { // 只处理目标音频流
            // 8.1 发送packet到解码器
            if (avcodec_send_packet(this->codec_ctx, this->pkt))
            {
                av_log(nullptr, AV_LOG_ERROR, "avcodec_send_packet failed\n");
                return (this->invalid = AVCODEC_SEND_PKT_FAILED);
            }
            // 8.2 从解码器接收解码后的帧
            while (avcodec_receive_frame(this->codec_ctx, this->frame) >= 0)
            {
                data_size = av_samples_get_buffer_size(
                    nullptr, this->codec_ctx->channels, this->frame->nb_samples, AV_SAMPLE_FMT_S16, 1);
                if (data_size < 0)
                {
                    av_log(nullptr, AV_LOG_ERROR, "Failed to calculate data size\n");
                    return (this->invalid = GET_BYTES_FAILED);
                }
                if (*buffer_len+data_size > MAX_AUDIO_FRAME_SIZE)
                {
                    return 0;
                }
step1:
                // 8.4 保存PCM数据
                av_log(nullptr, AV_LOG_INFO, "audio_chunk_ptr: %p\n", audio_chunk_ptr);
                int out_samples = swr_convert(this->swr_ctx, &(audio_chunk_ptr), MAX_AUDIO_FRAME_SIZE, (const uint8_t **)this->frame->data, this->frame->nb_samples);
                av_log(nullptr, AV_LOG_INFO, "audio_chunk_ptr: %p %p\n", audio_chunk_ptr, &audio_chunk_ptr);
                // 获取covert后的数据大小
                *buffer_len += data_size;
                audio_chunk_ptr += data_size;
                av_log(nullptr, AV_LOG_INFO, "buffer_len: %ld\n", *buffer_len);

            }
        }
        // 8.5 解引用packet
        av_packet_unref(this->pkt);
    }
    return 0;
}

// 声卡回调函数
void audiospec_callback(void *udata, Uint8 *stream, int len)
{
    if (buffer_len == 0)
    {
        SDL_memset(stream, 0, len);
        return;
    }

    SDL_memset(stream, 0, len);
    av_log(nullptr, AV_LOG_INFO, "len: %d, buffer_len: %ld\n", len, buffer_len);

    len = (len < (int)buffer_len) ? len : (int)buffer_len;
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);    // 混音: dst=mix(dst, src)

    audio_pos += len;
    buffer_len -= len;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {  // 错误处理
        av_log(nullptr, AV_LOG_ERROR, "usage: %s <input>\n", argv[0]);
        return 1;
    }
    char *src = argv[1];
    if (!src) {
        av_log(nullptr, AV_LOG_ERROR, "input is nullptr\n");
        return 1;
    }

    // char *dst = "../output/fly.pcm";
    // FILE *fp = fopen(dst, "w");
    // 创建音频提取器
    ExtractPCM ex_pcm(src);

    // pcm数据buf
    uint8_t *audio_chunk = (uint8_t *)malloc(MAX_AUDIO_FRAME_SIZE+1);
    if (!audio_chunk) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate audio chunk buffer!\n");
        return 1;
    }
    audio_pos = audio_chunk;

    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        free(audio_chunk);
        return 1;
    }

    // 2. 打开音频设备
    SDL_AudioSpec spec;
    // 2.1 设置参数(回调函数是因为声卡是拉数据而不是我们推给他)
    spec.freq = 48000;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 4096; // 增大缓冲区大小
    spec.callback = audiospec_callback;
    spec.userdata = nullptr;
    // 2.2 打开音频设备
    if (SDL_OpenAudio(&spec, nullptr))
    {
        av_log(nullptr, AV_LOG_ERROR, "Failed to open audio device, %s\n", SDL_GetError());
        free(audio_chunk);
        SDL_Quit();
        return 1;
    }
    // 2.3 播放音频
    SDL_PauseAudio(0); // 播放音频(非0是暂停, 0是播放)

    if (ex_pcm.invalid){
        av_log(nullptr, AV_LOG_ERROR, "PCM file could not be opened!\n");
        free(audio_chunk);
        SDL_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // 4. 播放音频
    int ret, read_len;
    do
    {
        ret = ex_pcm.extract_pcm(audio_chunk, &buffer_len);
        if (ret)
        {
            av_log(nullptr, AV_LOG_ERROR, "extract_pcm failed\n");
            break;
        }
        read_len = buffer_len;
        av_log(nullptr, AV_LOG_INFO, "buffer_len: %ld\n", buffer_len);
        audio_pos = audio_chunk;
        // fwrite(audio_chunk, 1, buffer_len, fp);
        // 等待声卡播放完毕
        while (buffer_len > 0)
        {
            SDL_Delay(100);
        }
    } while (read_len != 0);

    // 7. 释放资源
    free(audio_chunk);
    SDL_CloseAudio();
    SDL_Quit();
    // fclose(fp);
    return 0;
}