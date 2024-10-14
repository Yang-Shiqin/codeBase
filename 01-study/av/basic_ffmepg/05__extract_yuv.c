/* 解复用+解码: 从avi/ts提取yuv视频解码数据 */

// gcc 05__extract_yuv.c -lavformat -lavutil -lavcodec -lswscale
// ./a.out ../data/fly.avi ../output/out.yuv 或 ./a.out ../data/fly.ts ../output/out.yuv
// ffplay ../output/out.h264

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int ret;
  int l;
  int video_index;
  AVFormatContext *fmt_ctx = NULL;
  char * src = NULL;
  char * dst = NULL;
  av_log_set_level(AV_LOG_INFO);

  // 设置从命令行读入参数(输入视频，输出音频)
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
  if(avformat_find_stream_info(fmt_ctx, NULL) < 0){
    av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
    goto close_input;
	}

  FILE* dst_fd = fopen(dst, "wb");
  if (!dst_fd){
    av_log(NULL, AV_LOG_ERROR, "open output failed\n");
    goto close_input;
  }

  // 3. 从输入文件中获取流stream，video_index为视频流索引
  video_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  if (video_index < 0){
    av_log(NULL, AV_LOG_ERROR, "finding best stream failed\n");
    goto close_output;
  }

  // 4. 查找流对应的编码器并分配上下文
  AVCodec *codec = avcodec_find_decoder(fmt_ctx->streams[video_index]->codecpar->codec_id);
  if (!codec){
    av_log(NULL, AV_LOG_ERROR, "avcodec_find_decoder failed\n");
    goto close_output;
  }
  AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

  // 5. 读取流参数到编码器上下文, 打开编码器
  ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[video_index]->codecpar);
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
    goto close_codec_ctx;
  }

  // 7. 初始化帧结构以存放读入的frame
  // 7.1 初始化存放解码pkt后的帧结构
  AVFrame * frame = av_frame_alloc();
  if (!frame){
    av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
    goto close_pkt;
  }
  // 7.2 初始化存放裁剪后的帧结构, 设置初值和分配缓冲区
  AVFrame * frame_yuv = av_frame_alloc(); // 用于存放裁剪后输出
  if (!frame_yuv){
    av_log(NULL, AV_LOG_ERROR, "frame alloc failed\n");
    goto close_pkt;
  }
  frame_yuv->format = AV_PIX_FMT_YUV420P;      // 设置目标像素格式
  frame_yuv->width = codec_ctx->width;         // 设置目标宽度
  frame_yuv->height = codec_ctx->height;       // 设置目标高度
  ret = av_frame_get_buffer(frame_yuv, 32);    // 分配目标帧缓冲区
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "av_frame_get_buffer failed\n");
    goto close_frame;
  }

  // 8. 初始化缩放上下文
  struct SwsContext *sws_ctx = sws_getContext(codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt, 
		codec_ctx->width, codec_ctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
  if (!sws_ctx){
    av_log(NULL, AV_LOG_ERROR, "sws_getContext failed\n");
    goto close_frame;
  }

  // 9. 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(fmt_ctx, pkt) >= 0){
    if (pkt->stream_index == video_index){  // 只处理目标视频流
      // 9.1 处理packet
      if (strstr(fmt_ctx->iformat->name, "avi") || strstr(fmt_ctx->iformat->name, "ts")){
        // avi、mpegts都可以直接写入数据变成h264播放
        // 9.2 发送packet到解码器
        if (avcodec_send_packet(codec_ctx, pkt)){
          av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet failed\n");
          goto close_frame;
        }
        // 9.3 从解码器接收解码后的帧
        while(!avcodec_receive_frame(codec_ctx, frame)){
          // 9.4 裁剪黑边
          sws_scale(sws_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, codec_ctx->height, frame_yuv->data, frame_yuv->linesize);
          // 9.5 保存YUV数据
          fwrite(frame_yuv->data[0], 1, codec_ctx->width*codec_ctx->height, dst_fd);    // Y
          fwrite(frame_yuv->data[1], 1, codec_ctx->width*codec_ctx->height/4, dst_fd);  // U
          fwrite(frame_yuv->data[2], 1, codec_ctx->width*codec_ctx->height/4, dst_fd);  // V
        }
      }else{
        av_log(NULL, AV_LOG_INFO, "%s not support yet\n", fmt_ctx->iformat->name);
      }
    }
    // 9.6 解引用packet
    av_packet_unref(pkt);
  }

  // 10. 释放资源
  sws_freeContext(sws_ctx);
  av_frame_free(&frame_yuv);
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
  return 0;
}

