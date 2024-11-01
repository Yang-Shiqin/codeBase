/* 音频编码:  */

// gcc 14__encode_audio.c -lavutil -lavcodec
// ./a.out ../output/out.aac libfdk_aac
// ffplay ../output/out.aac

#include <libavutil/log.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <stdio.h>
#include <string.h>

/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt)
{
  const enum AVSampleFormat *p = codec->sample_fmts;

  while (*p != AV_SAMPLE_FMT_NONE)
  {
    if (*p == sample_fmt)
      return 1;
    p++;
  }
  return 0;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(const AVCodec *codec)
{
  const int *p;
  int best_samplerate = 0;

  if (!codec->supported_samplerates)
    return 44100;

  p = codec->supported_samplerates;
  while (*p)
  {
    if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
      best_samplerate = *p;
    p++;
  }
  return best_samplerate;
}

/* 模拟采集的数据, 填入frame */
int gen_pcm_rand_data(uint8_t * data, int nb_samples, int nb_channels)
{
  for (int i = 0; i < nb_samples * nb_channels; i++)
  {
    data[i] = rand() % 32768; // 生成随机的 16-bit PCM 数据
  }
}

int main(int argc, char *argv[])
{
  int ret, i;
  char *dst = NULL, *codec_name = NULL;
  uint8_t endcode[] = {0, 0, 1, 0xb7};
  av_log_set_level(AV_LOG_INFO);

  // 设置从命令行读入参数(输入视频，输出音频)
  if (argc < 3)
  { // 错误处理
    av_log(NULL, AV_LOG_ERROR, "usage: %s <output> <codec name>\n", argv[0]);
    return -1;
  }

  dst = argv[1];
  codec_name = argv[2];
  if (!dst || !codec_name)
  {
    av_log(NULL, AV_LOG_ERROR, "output or codec_name is NULL\n");
    return -1;
  }

  FILE *dst_fd = fopen(dst, "wb");
  if (!dst_fd)
  {
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    return -1;
  }

  // 1. 打开编码器
  // 1.1 根据name寻找编码器
  // const AVCodec *codec = avcodec_find_encoder_by_name(codec_name);
  const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
  if (!codec)
  {
    av_log(NULL, AV_LOG_ERROR, "codec %s not found\n", codec_name);
    return -1;
  }

  // 1.2 初始化编码器上下文
  AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

  // 1.3 设置编码器参数(因为没有输入文件的参数可复制, 需要手动设置)
  codec_ctx->bit_rate = 64000;               // 比特率
  codec_ctx->sample_fmt = AV_SAMPLE_FMT_S16; // 编码器的像素格式
  if (!check_sample_fmt(codec, codec_ctx->sample_fmt))
  {
    av_log(NULL, AV_LOG_ERROR, "Encoder does not support sample format %s",
           av_get_sample_fmt_name(codec_ctx->sample_fmt));
    goto close_output;
  }
  codec_ctx->sample_rate = select_sample_rate(codec);
  codec_ctx->channel_layout = av_get_channel_layout("stereo");
  codec_ctx->channels = av_get_channel_layout_nb_channels(codec_ctx->channel_layout);

  // 1.4 打开编码器
  ret = avcodec_open2(codec_ctx, codec, NULL);
  if (ret < 0)
  {
    av_log(NULL, AV_LOG_ERROR, "avcodec_open2 failed\n");
    goto close_output;
  }

  // 2. 初始化包结构以存放读入的packet
  AVPacket *pkt = av_packet_alloc();
  if (!pkt)
  {
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    ret = -1;
    goto close_codec_ctx;
  }

  // 3. 初始化帧, 设置参数和分配缓冲区
  AVFrame *frame = av_frame_alloc(); // 用于存放裁剪后输出
  if (!frame)
  {
    av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
    ret = -1;
    goto close_pkt;
  }
  frame->nb_samples = codec_ctx->frame_size;          // 设置目标帧的采样数
  frame->format = codec_ctx->sample_fmt;              // 设置目标帧的采样格式
  frame->channel_layout = codec_ctx->channel_layout;  // 设置目标帧的声道布局
  ret = av_frame_get_buffer(frame, 0);                // 分配目标帧缓冲区, 0表示默认对齐
  if (ret < 0)
  {
    av_log(NULL, AV_LOG_ERROR, "av_frame_get_buffer failed\n");
    goto close_frame;
  }

  // 4. 采样、编码、输出
  for (i = 0; i < 200; i++)
  {
    // 4.1 采样
    ret = gen_pcm_rand_data(frame->data[0], frame->nb_samples, frame->channels);
    if (ret < 0)
    {
      av_log(NULL, AV_LOG_ERROR, "gen_yuv_data failed\n");
      goto close_frame;
    }
    frame->pts = i; // 设置帧的显示时间戳

    // 4.2 编码: 发送packet到解码器
    if (avcodec_send_frame(codec_ctx, frame))
    {
      av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
      ret = -1;
      goto close_frame;
    }
    // 4.3 编码: 从解码器接收解码后的帧
    while (!avcodec_receive_packet(codec_ctx, pkt))
    {
      // 4.4 输出保存PCM数据
      fwrite(pkt->data, 1, pkt->size, dst_fd);
      // 4.5 解引用packet
      av_packet_unref(pkt);
    }
  }

  // 5. 处理剩余pkt
  if (avcodec_send_frame(codec_ctx, NULL))
  {
    av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
    ret = -1;
    goto close_frame;
  }
  while (!avcodec_receive_packet(codec_ctx, pkt))
  {
    fwrite(pkt->data, 1, pkt->size, dst_fd);
    av_packet_unref(pkt);
  }
  // 6. 写结束码
  fwrite(endcode, 1, sizeof(endcode), dst_fd);

  // 7. 释放资源
close_frame:
  av_frame_free(&frame);
close_pkt:
  av_packet_free(&pkt);
close_codec_ctx:
  avcodec_free_context(&codec_ctx);
close_output:
  fclose(dst_fd);
  return ret;
}
