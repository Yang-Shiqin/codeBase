/* 视频编码: 随机生成yuv数据编码为h264 */

// gcc 13__encode_video.c -lavutil -lavcodec
// ./a.out ../output/out.h264 libx264
// ffplay ../output/out.h264

#include <libavutil/log.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <stdio.h>
#include <string.h>

/* 模拟相机采集的数据, 填入frame */
int gen_yuv_data(AVFrame * frame, int height, int width){
  static int iframe = 0;
  int x, y;
  // 清空stdout缓存
  fflush(stdout);
  // 确保frame是可写的(可能在别的地方被送到解码器等共享, 就不可写, 就要复制一份)
  if (av_frame_make_writable(frame)){
    av_log(NULL, AV_LOG_ERROR, "frame is not writable\n");
    return -1;
  }
  // 生成YUV420P数据
  /* Y */
  for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
          frame->data[0][y * frame->linesize[0] + x] = x + y + iframe * 3;
      }
  }
  /* Cb and Cr */
  for (y = 0; y < height/2; y++) {
      for (x = 0; x < width/2; x++) {
          frame->data[1][y * frame->linesize[1] + x] = 128 + y + iframe * 2;
          frame->data[2][y * frame->linesize[2] + x] = 64 + x + iframe * 5;
      }
  }
  iframe++;
  return 0;
}

int main(int argc, char* argv[])
{
  int ret, i;
  int framerate;
  char *dst = NULL, *codec_name = NULL;
  uint8_t endcode[] = { 0, 0, 1, 0xb7 };
  av_log_set_level(AV_LOG_INFO);

  // 设置从命令行读入参数(输入视频，输出音频)
  if (argc<3){  // 错误处理
    av_log(NULL, AV_LOG_ERROR, "usage: %s <output> <codec name>\n", argv[0]);
    return -1;
  }

  dst = argv[1];
  codec_name = argv[2];
  if (!dst || !codec_name){
    av_log(NULL, AV_LOG_ERROR, "output or codec_name is NULL\n");
    return -1;
  }

  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    return -1;
  }

  // 1. 打开编码器
  // 1.1 根据name寻找编码器
  const AVCodec* codec = avcodec_find_encoder_by_name(codec_name);

  // 1.2 初始化编码器上下文
  AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

  // 1.3 设置编码器参数(因为没有输入文件的参数可复制, 需要手动设置)
  framerate = 25;
  codec_ctx->bit_rate = 400000;                 // 比特率
  codec_ctx->width = 352;                       // 分辨率的宽(必须是2的倍数)
  codec_ctx->height = 288;                      // 分辨率的高(必须是2的倍数)
  codec_ctx->time_base = (AVRational){1, framerate};  // 时间基(帧率的倒数)
  codec_ctx->framerate = (AVRational){framerate, 1};  // 帧率(每秒25帧)
  codec_ctx->gop_size = 10;                     // 图像组的大小(关键帧间隔), 越大压缩率越高, 解压时间越长
  codec_ctx->max_b_frames = 1;                  // 两个参考帧之间最多可以插入的B帧的数量
  codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;      // 编码器的像素格式

  if (codec->id == AV_CODEC_ID_H264)
      av_opt_set(codec_ctx->priv_data, "preset", "slow", 0);  // 设置编码速度(慢, 质量更高)

  // 1.4 打开编码器
  ret = avcodec_open2(codec_ctx, codec, NULL);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "avcodec_open2 failed\n");
    goto close_output;
  }

  // 2. 初始化包结构以存放读入的packet
  AVPacket * pkt = av_packet_alloc();
  if (!pkt){
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    ret = -1;
    goto close_codec_ctx;
  }

  // 3. 初始化帧, 设置参数和分配缓冲区
  AVFrame * frame = av_frame_alloc(); // 用于存放裁剪后输出
  if (!frame){
    av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
    ret = -1;
    goto close_pkt;
  }
  frame->format = codec_ctx->pix_fmt;      // 设置目标像素格式
  frame->width = codec_ctx->width;         // 设置目标宽度
  frame->height = codec_ctx->height;       // 设置目标高度
  ret = av_frame_get_buffer(frame, 32);    // 分配目标帧缓冲区
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "av_frame_get_buffer failed\n");
    goto close_frame;
  }

  // 4. 采样、编码、输出
  for (i=0; i<framerate; i++){
    // 4.1 采样
    ret = gen_yuv_data(frame, codec_ctx->height, codec_ctx->width);
    if (ret < 0){
      av_log(NULL, AV_LOG_ERROR, "gen_yuv_data failed\n");
      goto close_frame;
    }
    frame->pts = i; // 设置帧的显示时间戳

    // 4.2 编码: 发送packet到解码器
    if (avcodec_send_frame(codec_ctx, frame)){
      av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
      ret = -1;
      goto close_frame;
    }
    // 4.3 编码: 从解码器接收解码后的帧
    while(!avcodec_receive_packet(codec_ctx, pkt)){
      // 4.4 输出保存YUV数据
      fwrite(pkt->data, 1, pkt->size, dst_fd);
      // 4.5 解引用packet
      av_packet_unref(pkt);
    }
  }

  // 5. 处理剩余pkt
  if (avcodec_send_frame(codec_ctx, NULL)){
    av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
    ret = -1;
    goto close_frame;
  }
  while(!avcodec_receive_packet(codec_ctx, pkt)){
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

