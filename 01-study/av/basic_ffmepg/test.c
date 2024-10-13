// gcc 03__extract_audio.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.mp4 ../output/out.aac
// ffplay ../output/out.aac

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <stdio.h>
#include <string.h>


// 从mp4视频中提取aac音频
// | ADTS Header | AAC Data | ADTS Header | AAC Data | ...
int main(int argc, char* argv[])
{
  int ret;
  int l;
  int video_index;
  AVFormatContext *fmt_ctx = NULL;
  char * src = NULL;
  char * dst = NULL;
  AVPacket * pkt = NULL;
  av_log_set_level(AV_LOG_INFO);

  // 1. 设置从命令行读入参数(输入视频，输出音频)
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

  // 2. 打开文件
  // 2.1 打开输入视频文件
  ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "open input failed\n");
    return -1;
  }

  printf("%s\n", fmt_ctx->iformat->name);
  printf("%s\n", strstr(fmt_ctx->iformat->name, "264"));

  // 2.2 打开输出音频文件
  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    goto close_input;
  }

  // 3. 从输入文件中获取流stream，video_index为音频流索引
  video_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  if (video_index < 0){
    av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
    goto close_output;
  }

  // 4. 从流中读取包packet
  // 4.1 初始化packet结构体
  pkt = av_packet_alloc();
  if (!pkt){
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    goto close_output;
  }

  // 4.2 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(fmt_ctx, pkt) >= 0){
    // 4.3 处理packet
    if (pkt->stream_index == video_index){  // 只处理目标音频流
      /* 
       * mp4的音频和aac的音频封装格式不同, av_read_frame从mp4中读取的是原始数据, 
       * aac的封装格式是adts+原始数据, 所以需要添加adts头信息
       */

      // 4.3.2 写入数据
      l = fwrite(pkt->data, 1, pkt->size, dst_fd);  // 写入输出文件(只写了数据，没有写adts头信息，ffplay无法播放)
      if (l != pkt->size){
        av_log(NULL, AV_LOG_WARNING, "data size not eq\n");
      }
    }
    // 4.4 解引用packet
    av_packet_unref(pkt);
  }

  // 5. 释放资源
close_output:
  fclose(dst_fd);
close_input:
  avformat_close_input(&fmt_ctx);
  return 0;
}

