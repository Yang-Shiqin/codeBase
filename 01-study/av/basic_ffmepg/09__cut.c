/* 裁剪. 测试支持:
 * - avi->mpeg-ts
 * - avi->mp4
 * - avi->avi
 * 其他都会失败, 时间戳有问题(不知道咋搞); mp4->avi报错h264封装格式不同(可能不仅要解复用复用，还要解码编码)
 */

// gcc 09__cut.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.avi ../output/fly.mp4 2.5 4
// ffplay ../output/fly.mp4

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <stdio.h>

int cut_format(const char *src, const char *dst, double st, double et){
  int ret, i;
  AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;

  // 1. 打开输入视频文件
  ret = avformat_open_input(&ifmt_ctx, src, NULL, NULL);  // 根据src文件名判断封装格式
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "open input failed\n");
    return -1;
  }

  // 2. 获取视频流详细信息并填入ifmt_ctx中
  ret = avformat_find_stream_info(ifmt_ctx, NULL);
  if(ret < 0){
    av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
    goto close_input;
	}
  av_dump_format(ifmt_ctx, 0, src, 0);

  // 3. 初始化输出上下文结构体
  avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, dst); // 根据dst文件名判断封装格式
  if (!ofmt_ctx){
    av_log(NULL, AV_LOG_ERROR, "avformat_alloc_output_context2 failed\n");
    ret = -1;
    goto close_input;
  }

  // 4. 拷贝输入流编解码信息到输出上下文
  // 遍历流
  for (i=0; i<ifmt_ctx->nb_streams; i++){
    AVStream *in_stream = ifmt_ctx->streams[i];

    // 4.1 初始化输出流
    AVStream *out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream){
      av_log(NULL, AV_LOG_ERROR, "avformat_new_stream failed\n");
      ret = -1;
      goto close_output;
    }

    // 4.2 拷贝流的编解码参数信息
    ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
    if (ret < 0){
      av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_copy failed\n");
      goto close_output;
    }
    out_stream->codecpar->codec_tag = 0;  // 让 FFmpeg 自动确定合适的编解码器标记
  }
  av_dump_format(ofmt_ctx, 0, dst, 1);

  // 5. 若输出需要文件, 则打开文件
  if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)){
    ret = avio_open(&ofmt_ctx->pb, dst, AVIO_FLAG_WRITE);
    if (ret < 0){
      av_log(NULL, AV_LOG_ERROR, "avio_open failed\n");
      goto close_output;
    }
  }

  // 6. 将流头写入输出
  ret = avformat_write_header(ofmt_ctx, NULL);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "avformat_write_header failed\n");
    goto close_io;
  }

  // 7. 初始化包结构以存放读入的packet
  AVPacket * pkt = av_packet_alloc();
  if (!pkt){
    av_log(NULL, AV_LOG_ERROR, "pkt alloc failed\n");
    ret = -1;
    goto close_io;
  }

  // 记录每个流的开始pts、dts
  int64_t *pts_start_list = av_mallocz(ifmt_ctx->nb_streams * sizeof(int64_t));
  if (!pts_start_list){
    av_log(NULL, AV_LOG_ERROR, "av_mallocz failed\n");
    ret = -1;
    goto close_io;
  }
  int64_t *dts_start_list = av_mallocz(ifmt_ctx->nb_streams * sizeof(int64_t));
  if (!dts_start_list){
    av_log(NULL, AV_LOG_ERROR, "av_mallocz failed\n");
    ret = -1;
    goto close_pts;
  }

  // 8. 查找开始时间对应的帧
  ret = av_seek_frame(ifmt_ctx, -1, st*AV_TIME_BASE, AVSEEK_FLAG_ANY);
  if (ret < 0){
    av_log(NULL, AV_LOG_ERROR, "av_seek_frame failed\n");
    goto close_dts;
  }

  // 9. 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(ifmt_ctx, pkt) >= 0){
    if ((pkt->stream_index>=0) && (pkt->stream_index<ifmt_ctx->nb_streams)){
      // 9.1 记录每个流开始pts和dts
      if (pts_start_list[pkt->stream_index] == 0){
        pts_start_list[pkt->stream_index] = pkt->pts;
      }
      if (dts_start_list[pkt->stream_index] == 0){
        dts_start_list[pkt->stream_index] = pkt->dts;
      }
      pts_start_list[pkt->stream_index] = FFMIN(pts_start_list[pkt->stream_index], pkt->pts);
      dts_start_list[pkt->stream_index] = FFMIN(dts_start_list[pkt->stream_index], pkt->dts);
      // 9.2 对时间戳进行时基转换
      AVRational bq = ifmt_ctx->streams[pkt->stream_index]->time_base;
      AVRational cq = ofmt_ctx->streams[pkt->stream_index]->time_base;
      if (pkt->pts*av_q2d(bq) >= et) break;  // 读到结束时间则退出
      pkt->pts = av_rescale_q_rnd(pkt->pts-pts_start_list[pkt->stream_index], bq, cq, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
      pkt->dts = av_rescale_q_rnd(pkt->dts-dts_start_list[pkt->stream_index], bq, cq, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
      if (pkt->pts < 0) pkt->pts = 0;
      if (pkt->dts < 0) pkt->dts = 0;
      // if (pkt->pts < pkt->dts) {
      //   pkt->pts = pkt->dts;
      // }
      pkt->duration = av_rescale_q(pkt->duration, bq, cq);  // 改了整个流的时间比例才改这个，一般不用直接改这个
      pkt->pos = -1;  // 新生成的数据包, 无位置信息可用
      // 9.3 将数据包按顺序写入输出
      ret = av_interleaved_write_frame(ofmt_ctx, pkt);
      if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "av_interleaved_write_frame failed\n");
        goto close_pkt;
      }
    }
    // 9.4 解引用packet
    av_packet_unref(pkt);
  }

  // 10. 将流尾写入输出
  av_write_trailer(ofmt_ctx);

  // 11. 释放资源
close_pkt:
  av_packet_free(&pkt);
close_io:
  if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
    avio_closep(&ofmt_ctx->pb);
close_dts:
  av_freep(&dts_start_list);
close_pts:
  av_freep(&pts_start_list);
close_output:
  avformat_free_context(ofmt_ctx);
close_input:
  avformat_close_input(&ifmt_ctx);
  return ret;
}

int main(int argc, char* argv[])
{
  char *src = NULL, *dst = NULL;
  double st, et;

  av_log_set_level(AV_LOG_INFO);

  // 设置从命令行读入参数(输入视频，输出音频)
  if (argc<5){  // 错误处理
    av_log(NULL, AV_LOG_ERROR, "usage: %s <input> <output> <start_time(s)> <end_time(s)>\n", argv[0]);
    return -1;
  }

  src = argv[1];
  dst = argv[2];
  st = atof(argv[3]);
  et = atof(argv[4]);
  if (!src || !dst || st<0 || et<st){
    av_log(NULL, AV_LOG_ERROR, "cmd is invalid\n");
    return -1;
  }
  return cut_format(src, dst, st, et);
}

