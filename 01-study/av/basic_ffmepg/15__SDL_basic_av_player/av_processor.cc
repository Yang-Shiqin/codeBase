/* SDL播放音视频 */

#include "av_processor.h"

#define MAX_AUDIO_FRAME_SIZE 192000
#define MAX_AUDIO_FRAME_READ_ONCE 5

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
    this->a_codec_ctx = avcodec_alloc_context3(this->a_codec);

    // 5. 读取视频、音频流参数到编码器上下文, 打开编码器
    ret = avcodec_parameters_to_context(this->v_codec_ctx, this->fmt_ctx->streams[this->v_index]->codecpar);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_V_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->v_codec_ctx, this->v_codec, nullptr);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "video avcodec_open2 failed\n");
        this->invalid = V_CODEC_OPEN_FAILED;
        return;
    }
    this->h = this->v_codec_ctx->height;
    this->w = this->v_codec_ctx->width;

    ret = avcodec_parameters_to_context(this->a_codec_ctx, this->fmt_ctx->streams[this->a_index]->codecpar);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
        this->invalid = READ_A_PARA_FAILED;
        return;
    }

    ret = avcodec_open2(this->a_codec_ctx, this->a_codec, nullptr);
    if (ret < 0){
        av_log(nullptr, AV_LOG_ERROR, "audio avcodec_open2 failed\n");
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
    this->v_frame = av_frame_alloc();
    if (!this->v_frame){
        av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = V_FRAME_ALLOC_FAILED;
        return;
    }
    this->a_frame = av_frame_alloc();
    if (!this->a_frame){
        av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
        this->invalid = A_FRAME_ALLOC_FAILED;
        return;
    }

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

// 析构函数, 错误处理和资源释放
AvProcessor::~AvProcessor(){
    switch (this->invalid)
    {
    case 0:
    case GET_BYTES_FAILED:
    case AV_MALLOC_FAILED:
    case AVCODEC_SEND_PKT_FAILED:
    case CREAT_DAUDIO_THREAD_FAILED:
    case CREAT_DVIDEO_THREAD_FAILED:
        this->is_quit = 1;
        swr_free(&this->swr_ctx);
    case SWR_GETCONTEXT_FAILED:
        sws_freeContext(this->sws_ctx);
    case SWS_GETCONTEXT_FAILED:
        av_frame_free(&this->a_frame);
    case A_FRAME_ALLOC_FAILED:
        av_frame_free(&this->v_frame);
    case V_FRAME_ALLOC_FAILED:
        av_packet_free(&this->a_pkt);
    case A_PACKET_ALLOC_FAILED:
        av_packet_free(&this->v_pkt);
    case V_PACKET_ALLOC_FAILED:
    case A_CODEC_OPEN_FAILED:
    case READ_A_PARA_FAILED:
    case V_CODEC_OPEN_FAILED:
    case READ_V_PARA_FAILED:
        avcodec_free_context(&(this->a_codec_ctx));
    case A_CODEC_NOT_FOUND:
        avcodec_free_context(&(this->v_codec_ctx));
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
    // 1. 创建视频解码线程和音频解码线程
    SDL_Thread *video_tid = SDL_CreateThread(decode_video_thread, "decode_video_thread", this);
    if (!video_tid) {
        av_log(NULL, AV_LOG_ERROR, "SDL_CreateThread decode_video_thread failed\n");
        return (this->invalid = CREAT_DVIDEO_THREAD_FAILED);
    }
    SDL_Thread *audio_tid = SDL_CreateThread(decode_audio_thread, "decode_audio_thread", this);
    if (!audio_tid) {
        av_log(NULL, AV_LOG_ERROR, "SDL_CreateThread decode_audio_thread failed\n");
        return (this->invalid = CREAT_DAUDIO_THREAD_FAILED);
    }
    // 2. 解复用
    while(1){
        if (this->is_quit){
            // 等待解码线程退出
            SDL_WaitThread(video_tid, nullptr);
            SDL_WaitThread(audio_tid, nullptr);
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
            if (pkt->stream_index == this->v_index){    // 视频流
                this->v_pkt_queue.push(pkt);
            }else if (pkt->stream_index == this->a_index){  // 音频流
                this->a_pkt_queue.push(pkt);
            }
        }
    }
    return 0;
}

int AvProcessor::decode_video(){
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    // 视频解码
    while(1){
        if (this->is_quit){
            return 0;
        }
        // 1. 读取packet
        pkt = this->v_pkt_queue.pop();
        // 2. 发送packet到解码器
        if (avcodec_send_packet(this->v_codec_ctx, pkt)){
            av_log(nullptr, AV_LOG_ERROR, "avcodec_send_packet failed\n");
            return (this->invalid = AVCODEC_SEND_PKT_FAILED);
        }
        // 3. 从解码器接收解码后的帧
        while(avcodec_receive_frame(this->v_codec_ctx, this->v_frame)>=0){
            // 3.1 分配帧
            frame = av_frame_alloc();
            if (!frame){
                av_log(nullptr, AV_LOG_ERROR, "frame alloc failed\n");
                return (this->invalid = V_FRAME_ALLOC_FAILED);
            }
            frame->format = AV_PIX_FMT_YUV420P;      // 设置目标像素格式
            frame->width = this->v_codec_ctx->width;         // 设置目标宽度
            frame->height = this->v_codec_ctx->height;       // 设置目标高度
            if (av_frame_get_buffer(frame, 32) < 0) {    // 分配目标帧缓冲区
                av_log(nullptr, AV_LOG_ERROR, "frame buffer alloc failed\n");
                av_frame_free(&frame);
                return (this->invalid = V_FRAME_ALLOC_FAILED);
            }
            // 3.2 裁剪黑边
            sws_scale(this->sws_ctx, (const uint8_t* const*)this->v_frame->data, this->v_frame->linesize, 0, 
                this->v_codec_ctx->height, frame->data, frame->linesize);
            // 3.3 压入帧队列
            v_frame_queue.push(frame);
        }
        // 4. 解引用packet
        av_packet_unref(pkt);
    }
    return 0;
}

int AvProcessor::decode_audio(){
    AVPacket *pkt = nullptr;
    int data_size = 0;
    uint8_t *buf = (uint8_t*)av_malloc(8192);
    if (!buf){
        av_log(nullptr, AV_LOG_ERROR, "av_malloc failed\n");
        return (this->invalid = AV_MALLOC_FAILED);
    }
    // 音频解码
    while(1){
        if (this->is_quit){
            return 0;
        }
        // 1. 读取packet
        pkt = this->a_pkt_queue.pop();
        // 2. 发送packet到解码器
        if (avcodec_send_packet(this->a_codec_ctx, pkt)){
            av_log(nullptr, AV_LOG_ERROR, "avcodec_send_packet failed\n");
            av_free(buf);
            return (this->invalid = AVCODEC_SEND_PKT_FAILED);
        }
        // 3. 从解码器接收解码后的帧
        while(avcodec_receive_frame(this->a_codec_ctx, this->a_frame) >= 0){
            // 3.1 计算数据大小
            data_size = av_samples_get_buffer_size(
                nullptr, this->a_codec_ctx->channels, this->a_frame->nb_samples, AV_SAMPLE_FMT_S16, 1);
            if (data_size < 0)
            {
                av_log(nullptr, AV_LOG_ERROR, "Failed to calculate data size\n");
                av_free(buf);
                return (this->invalid = GET_BYTES_FAILED);
            }
            // 3.2 格式转换
            swr_convert(this->swr_ctx, &buf, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)this->a_frame->data, this->a_frame->nb_samples);
            // 3.3 压入音频帧队列
            this->audio_chunk.push(buf, data_size);
        }
        // 4. 解引用packet
        av_packet_unref(pkt);
    }
    av_free(buf);
    return 0;
}

void AvProcessor::audio_chunk_pop(uint8_t *stream, int len){ this->audio_chunk.pop(stream, len); }
AVFrame* AvProcessor::video_frame_pop(){ return this->v_frame_queue.pop(); }