// gcc 03__extract_audio.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.mp4 ../output/out.aac
// ffplay ../output/out.aac

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <stdio.h>

#define ADTS_HEADER_SIZE 7

// 生成adts头信息
void generate_adts_header(uint8_t *buffer, int pkt_size, int profile, int freq_index, int channel_config) {
  // ADTS Header
  buffer[0] = 0xff; // Syncword (first byte)
  buffer[1] = 0xf1; // ID, Layer, Protection absent (1)
  buffer[2] = (profile-1) << 6; // Profile
  buffer[2] |= (freq_index & 0x0f) << 2; // Sampling frequency index
  buffer[2] |= (channel_config & 0x04) >> 2; // Channel configuration (upper 1 bit)
  buffer[3] = (channel_config & 0x03) << 6; // Channel configuration (lower 2 bits)
  
  // Frame length (pkt_size + ADTS_HEADER_SIZE)
  buffer[3] |= ((pkt_size + ADTS_HEADER_SIZE) >> 11) & 0x03; // Frame length (first 5 bits)
  buffer[4] = ((pkt_size + ADTS_HEADER_SIZE) >> 3) & 0xff; // Frame length (next 8 bits)
  buffer[5] = ((pkt_size + ADTS_HEADER_SIZE) & 0x07) << 5; // Frame length (last 3 bits)
  buffer[5] |= 0x1f; // Buffer fullness (set to 0x1F as an example)
  buffer[6] = 0xfc; // Number of AAC frames (set to 0)
}

// 从mp4视频中提取aac音频
// | ADTS Header | AAC Data | ADTS Header | AAC Data | ...
int main(int argc, char* argv[])
{
  int ret;
  int l;
  int audio_index;
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

  // 2.2 打开输出音频文件
  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    goto close_input;
  }

  // 3. 从输入文件中获取流stream，audio_index为音频流索引
  audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  if (audio_index < 0){
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
    if (pkt->stream_index == audio_index){  // 只处理目标音频流
      /* 
       * mp4的音频和aac的音频封装格式不同, av_read_frame从mp4中读取的是原始数据, 
       * aac的封装格式是adts+原始数据, 所以需要添加adts头信息
       */
      // 4.3.1 写入adts头信息
      uint8_t adts_header[ADTS_HEADER_SIZE]={0};
      generate_adts_header(adts_header, pkt->size, 2, 3, 2);  // 2: AAC LC, 3: 48kHz, 2: stereo
      fwrite(adts_header, 1, ADTS_HEADER_SIZE, dst_fd);

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
  av_packet_free(&pkt);
close_output:
  fclose(dst_fd);
close_input:
  avformat_close_input(&fmt_ctx);
  return 0;
}

