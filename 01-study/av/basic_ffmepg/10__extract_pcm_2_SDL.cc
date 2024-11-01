// 播放卡顿，暂时还不知道为啥

/* SDL播放ffmpeg从avi提取的pcm数据(08+09, 08作为一个类) */

// g++ 10__extract_pcm_2_SDL.cc -lSDL2 -lavformat -lavcodec -lavutil
// ./a.out ../data/fly.avi

extern "C"
{
#include <stdio.h>
#include <SDL2/SDL.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define BLOCK_SIZE 4096000
// 声卡队列(必须是全局或静态的)
static uint8_t *audio_pos = NULL;
static size_t buffer_len = 0;

class ExtractPCM
{
public:
    int invalid = 0; // 错误处理, 0: valid, >0: invalid
    ExtractPCM(const char *src);
    ~ExtractPCM();
    int extract_pcm(uint8_t *audio_chunk, size_t* buffer_len);  // 解码音频

private:
    AVFormatContext *fmt_ctx = NULL;
    const AVCodec *codec = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVPacket *pkt = NULL;
    AVFrame *frame = NULL;
    int audio_index;
    int step = 0; // 模拟生成器
    enum ERRNO
    {
        GET_BYTES_FAILED = 1,
        AVCODEC_SEND_PKT_FAILED,
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
    ret = avformat_open_input(&(this->fmt_ctx), src, NULL, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "open input failed\n");
        this->invalid = OPEN_INPUT_FAILED;
        return;
    }
    // 2. 获取视频流详细信息并填入fmt_ctx中
    ret = avformat_find_stream_info(this->fmt_ctx, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
        this->invalid = FIND_STREAM_FAILED;
        return;
    }
    // 3. 从输入文件中获取流stream，audio_index为音频流索引
    this->audio_index = av_find_best_stream(this->fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (this->audio_index < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
        this->invalid = FIND_BEST_STREAM_FAILED;
        return;
    }

    // 4. 查找流对应的解码器并分配上下文
    this->codec = avcodec_find_decoder(this->fmt_ctx->streams[this->audio_index]->codecpar->codec_id);
    if (!this->codec)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
        this->invalid = CODEC_NOT_FOUND;
        return;
    }
    this->codec_ctx = avcodec_alloc_context3(this->codec);

    // 5. 读取流参数到解码器上下文, 打开解码器
    ret = avcodec_parameters_to_context(this->codec_ctx, this->fmt_ctx->streams[this->audio_index]->codecpar);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->codec_ctx, this->codec, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_open2 failed\n");
        this->invalid = CODEC_OPEN_FAILED;
        return;
    }

    // 6. 初始化包结构以存放读入的packet
    this->pkt = av_packet_alloc();
    if (!this->pkt)
    {
        av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
        this->invalid = PACKET_ALLOC_FAILED;
        return;
    }
    // 7. 初始化帧结构以存放读入的frame
    this->frame = av_frame_alloc();
    if (!this->frame)
    {
        av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = FRAME_ALLOC_FAILED;
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
                av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
                return (this->invalid = AVCODEC_SEND_PKT_FAILED);
            }
            // 8.2 从解码器接收解码后的帧
step1:
            uint8_t *audio_chunk_ptr = audio_chunk;
            while (avcodec_receive_frame(this->codec_ctx, this->frame) >= 0)
            {
                // 8.3 计算每帧的字节数
                int data_size = av_get_bytes_per_sample(this->codec_ctx->sample_fmt);
                if (data_size < 0)
                {
                    av_log(NULL, AV_LOG_ERROR, "Failed to calculate data size\n");
                    return (this->invalid = GET_BYTES_FAILED);
                }
                // 8.4 保存PCM数据
                if (this->codec_ctx->sample_fmt == AV_SAMPLE_FMT_FLTP)
                {
                    // 将浮点平面格式(fltp)转换为整数格式(s16le)(因为ffplay不支持fltp)
                    for (int i = 0; i < this->frame->nb_samples; i++)
                    {
                        for (int ch = 0; ch < this->codec_ctx->channels; ch++)
                        {
                            float *src = (float *)this->frame->data[ch];
                            int16_t sample = (int16_t)(src[i] * 32767.0f);
                            memcpy(audio_chunk_ptr, &sample, sizeof(int16_t));
                            audio_chunk_ptr += sizeof(int16_t);
                            *buffer_len += sizeof(int16_t);
                        }
                    }
                }
                else
                {
                    // 处理其他格式
                    for (int i = 0; i < this->frame->nb_samples; i++)
                    {
                        for (int ch = 0; ch < this->codec_ctx->channels; ch++)
                        {
                            memcpy(audio_chunk_ptr, this->frame->data[ch] + data_size * i, data_size);
                            audio_chunk_ptr += data_size;
                            *buffer_len += data_size;
                        }
                    }
                }
                return 0;
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
        return;
    }

    SDL_memset(stream, 0, len);

    len = (len < buffer_len) ? len : buffer_len;
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

    audio_pos += len;
    buffer_len -= len;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {  // 错误处理
        av_log(NULL, AV_LOG_ERROR, "usage: %s <input>\n", argv[0]);
        return 1;
    }
    char *src = argv[1];
    if (!src) {
        av_log(NULL, AV_LOG_ERROR, "input is NULL\n");
        return 1;
    }

    // 创建音频提取器
    ExtractPCM ex_pcm(src);

    // pcm数据buf
    uint8_t *audio_chunk = (uint8_t *)malloc(BLOCK_SIZE);
    if (!audio_chunk) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate audio chunk buffer!\n");
        return 1;
    }

    // 1. 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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
    spec.userdata = NULL;
    // 2.2 打开音频设备
    if (SDL_OpenAudio(&spec, NULL))
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to open audio device, %s\n", SDL_GetError());
        free(audio_chunk);
        SDL_Quit();
        return 1;
    }
    // 2.3 播放音频
    SDL_PauseAudio(0); // 播放音频(非0是暂停, 0是播放)

    if (ex_pcm.invalid){
        av_log(NULL, AV_LOG_ERROR, "PCM file could not be opened!\n");
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
            av_log(NULL, AV_LOG_ERROR, "extract_pcm failed\n");
            break;
        }
        read_len = buffer_len;
        audio_pos = audio_chunk;
        // 等待声卡播放完毕
        while (buffer_len > 0)
        {
            SDL_Delay(10);
        }
    } while (read_len != 0);

    // 7. 释放资源
    free(audio_chunk);
    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}