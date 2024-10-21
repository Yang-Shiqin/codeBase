/* 解复用: 从avi/ts提取h264视频 */

// gcc 04__extract_video.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.avi ../output/out.h264 或 ./a.out ../data/fly.ts ../output/out.h264
// ffplay ../output/out.h264

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <stdio.h>
#include <string.h>

int add_start_code();
int add_SPS_PPS(uint8_t* extradata, int extradata_size, AVPacket* out_pkt);
int h264_mp42annexb(AVFormatContext *fmt_ctx, AVPacket* in, FILE* dst_fd);

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

  // 2.2 打开输出视频文件
  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    ret = -1;
    goto close_input;
  }

  // 3. 从输入文件中获取流stream，video_index为视频流索引
  video_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  if (video_index < 0){
    av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
    ret = video_index;
    goto close_output;
  }

  // 4. 从流中读取包packet
  // 4.1 初始化packet结构体
  pkt = av_packet_alloc();
  if (!pkt){
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    ret = -1;
    goto close_output;
  }

  // 4.2 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(fmt_ctx, pkt) >= 0){
    if (pkt->stream_index == video_index){  // 只处理目标视频流
      // 4.3 处理packet
      if (strstr(fmt_ctx->iformat->name, "mp4")){
        // [ ] TODO: 这个分支暂时没做
        goto tmp;
        // MP4 -> Annex-B(H.264的一种封装格式): 比较复杂, 需要处理SPS和PPS, 以及去掉长度前缀, 添加start code
        h264_mp42annexb(fmt_ctx, pkt, dst_fd);
      }else if (strstr(fmt_ctx->iformat->name, "avi") || strstr(fmt_ctx->iformat->name, "ts")){
        // avi、mpegts都可以直接写入数据变成h264播放
        l = fwrite(pkt->data, 1, pkt->size, dst_fd);
        if (l != pkt->size){
          av_log(NULL, AV_LOG_WARNING, "data size not eq\n");
        }
      }else{
tmp:
        av_log(NULL, AV_LOG_INFO, "%s not support yet\n", fmt_ctx->iformat->name);
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
  return ret;
}

/******************************************* 以下函数(MP4视频提取)尚未完全实现 *******************************************/

// 下面三个用来mp4->h264的函数太复杂了, 以后会写了再写
int add_start_code(){
  return 0;
}

/* 从mp4的extradata中获取SPS和PPS, 放入pkt中(写文件其实不用这步, 但封装成函数了还是要吧)
 * extradata:
 *   0~3字节:没用
 *   4: 最后2位表示编码长度(不知道啥, 不知道啥用)
 *   5: 最后5位表示SPS个数, 一般为1
 *   6~7: SPS长度
 *   8+SPS长度: 最后5位表示PPS个数, 一般为1
 *   9+SPS长度~: PPS长度
 */ 
int add_SPS_PPS(uint8_t* extradata, int extradata_size, AVPacket* out_pkt){
  return 0;
}

/* 从mp4视频(通常是AVCC封装格式)中提取h264视频(Annex-B封装格式), SPS和PPS在mp4中存在文件头(编解码器参数), 流中不包含
 * AVVC: | NALU size | NALU | NALU size | NALU |
 * Annex-B: | start code | NALU | start code | NALU |
 * 对每个IDR帧, 帧前加入SPS、PPS帧; 对所有帧去掉长度前缀, 添加start code */
int h264_mp42annexb(AVFormatContext *fmt_ctx, AVPacket* in, FILE* dst_fd){
  int i;
  int ret = 0;
  int pos = 0;  // 当前位置
  int nalu_size;
  int nalu_type;
  int buf_size = in->size;  // 当前包大小
  const uint8_t *buf = in->data;
  const uint8_t *buf_end = in->data + buf_size;

  while (buf < buf_end){
    ret = AVERROR(EINVAL);
    // 1. 读取AVCC头部长度
    if (buf+4 > buf_end){
      av_log(NULL, AV_LOG_ERROR, "nalu size error\n");
      return -1;
    }

    for (nalu_size=0, i=0; i<4; i++){
      nalu_size = (nalu_size << 8) | buf[i];  // 大端
    }
    if (nalu_size<0 || nalu_size>buf_end-buf){
      av_log(NULL, AV_LOG_ERROR, "nalu size error\n");
      return -1;
    }
    buf += 4;

    // 2. 读取NALU类型, IDR帧(5)则添加SPS和PPS; 所有帧都添加start code
    nalu_type = buf[0] & 0x1f;
    if (nalu_type == 5){
      // SPS和PPS从stream->codecpar->extradata中获取
      // add_SPS_PPS(fmt_ctx->streams[in->stream_index]->codecpar->extradata, 
      //             fmt_ctx->streams[in->stream_index]->codecpar->extradata_size, );
      // [ ] todo
    }else{
      add_start_code();
    }

    // 3. 写入数据
    // [ ] TODO
    // 4. 更新位置
    // [ ] TODO
    buf += nalu_size;
  }
  return ret;
}