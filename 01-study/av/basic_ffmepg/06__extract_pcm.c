// 能播，但噪声很大，感觉可能是格式解析的不太对

/* 解复用+解码: 从avi/ts提取pcm音频解码数据 */

// gcc 06__extract_pcm.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.avi ../output/out.pcm
// ffplay -ar 48000 -ac 2 -f s32le ../output/out.pcm

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int ret;
  int audio_index;
  AVFormatContext *fmt_ctx = NULL;
  char * src = NULL;
  char * dst = NULL;
  av_log_set_level(AV_LOG_INFO);

  // 设置从命令行读入参数(输入format，输出pcm)
  if (argc<3){  // 错误处理
    av_log(NULL, AV_LOG_ERROR, "usage: %s <input> <output>\n", argv[0]);
    return -1;
  }

  src = argv[1];
  dst = argv[2];
  if (!src || !dst){
    av_log(NULL, AV_LOG_ERROR, "input or output is NULL\n");
    return -1;
  }

  // 1. 打开输入视频文件
  ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "open input failed\n");
    return -1;
  }
  // 2. 获取视频流详细信息并填入fmt_ctx中
  ret = avformat_find_stream_info(fmt_ctx, NULL);
  if(ret < 0){
    av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
    goto close_input;
	}

  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    ret = -1;
    goto close_input;
  }

  // 3. 从输入文件中获取流stream，audio_index为音频流索引
  audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  if (audio_index < 0){
    av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
    ret = audio_index;
    goto close_output;
  }

  // 4. 查找流对应的编码器并分配上下文
  const AVCodec *codec = avcodec_find_decoder(fmt_ctx->streams[audio_index]->codecpar->codec_id);
  if (!codec){
    av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
    ret = -1;
    goto close_output;
  }
  AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

  // 5. 读取流参数到编码器上下文, 打开编码器
  ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[audio_index]->codecpar);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_to_context failed\n");
    goto close_codec_ctx;
  }

  ret = avcodec_open2(codec_ctx, codec, NULL);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "avcodec_open2 failed\n");
    goto close_codec_ctx;
  }

  // 6. 初始化包结构以存放读入的packet
  AVPacket * pkt = av_packet_alloc();
  if (!pkt){
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    ret = -1;
    goto close_codec_ctx;
  }

  // 7. 初始化帧结构以存放读入的frame
  // 7.1 初始化存放解码pkt后的帧结构
  AVFrame * frame = av_frame_alloc();
  if (!frame){
    av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
    ret = -1;
    goto close_pkt;
  }
  
  // 9. 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(fmt_ctx, pkt) >= 0){
    if (pkt->stream_index == audio_index){  // 只处理目标音频流
      // 9.1 处理packet
      // 9.2 发送packet到解码器
      if (avcodec_send_packet(codec_ctx, pkt)){
        av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
        ret = -1;
        goto close_frame;
      }
      // 9.3 从解码器接收解码后的帧
      while(avcodec_receive_frame(codec_ctx, frame)>=0){
        // 9.5 保存YUV数据
        fwrite(frame->data[0], 1, frame->linesize[0], dst_fd);
      }
    }
    // 9.6 解引用packet
    av_packet_unref(pkt);
  }

  // 10. 释放资源
close_frame:
  av_frame_free(&frame);
close_pkt:
  av_packet_free(&pkt);
close_codec_ctx:
  avcodec_free_context(&codec_ctx);
close_output:
  fclose(dst_fd);
close_input:
  avformat_close_input(&fmt_ctx);
  return ret;
}

