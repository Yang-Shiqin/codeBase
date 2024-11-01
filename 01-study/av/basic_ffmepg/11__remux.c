/* 格式转换/解复用+复用: mp4->flv等. 测试支持:
 * - mp4->mpeg-ts
 * - mp4->mp4
 * - avi->mpeg-ts
 * - avi->mp4
 * - avi->avi
 * - mpeg-ts->mpeg-ts
 * - mpeg-ts->mp4
 * - mpeg-ts->avi
 * mp4->avi 会失败, 报错h264封装格式不同(可能不仅要解复用复用，还要解码编码)
 */

// gcc 11__remux.c -lavformat -lavutil -lavcodec
// ./a.out ../data/fly.avi ../output/fly.mp4
// ffplay ../output/fly.mp4

#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  int ret, i, new_index;
  AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
  char *src = NULL, *dst = NULL;

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

  // 4. 复制输入流信息到输出上下文
  // 只复制视频、音频、字幕, 因此要对index重新映射
  int *stream_mapping = av_mallocz(ifmt_ctx->nb_streams * sizeof(*stream_mapping));
  if (!stream_mapping){
    av_log(NULL, AV_LOG_ERROR, "av_mallocz failed\n");
    ret = -1;
    goto close_output;
  }
  // 遍历流
  for (i=0, new_index=0; i<ifmt_ctx->nb_streams; i++){
    AVStream *in_stream = ifmt_ctx->streams[i];
    if ((in_stream->codecpar->codec_type != AVMEDIA_TYPE_AUDIO) &&
        (in_stream->codecpar->codec_type != AVMEDIA_TYPE_VIDEO) &&
        (in_stream->codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)){
      stream_mapping[i] = -1; // 不复制非(视频、音频、字幕)
      continue;
    }
    stream_mapping[i] = new_index++;  // 映射到输出流的index

    // 4.1 初始化输出流
    AVStream *out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream){
      av_log(NULL, AV_LOG_ERROR, "avformat_new_stream failed\n");
      ret = -1;
      goto close_map;
    }

    // 4.2 拷贝流的编解码参数信息
    ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
    if (ret < 0){
      av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_copy failed\n");
      goto close_map;
    }
    out_stream->codecpar->codec_tag = 0;  // 让 FFmpeg 自动确定合适的编解码器标记
  }
  av_dump_format(ofmt_ctx, 0, dst, 1);

  // 5. 若输出需要文件, 则打开文件
  if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)){
    ret = avio_open(&ofmt_ctx->pb, dst, AVIO_FLAG_WRITE);
    if (ret < 0){
      av_log(NULL, AV_LOG_ERROR, "avio_open failed\n");
      goto close_map;
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

  // 8. 读取packet(读完整的帧而不是半帧的包, 所以命名还是av_read_frame而不是av_read_packet)
  while(av_read_frame(ifmt_ctx, pkt) >= 0){
    if ((pkt->stream_index>=0) && (pkt->stream_index<ifmt_ctx->nb_streams)
        && (stream_mapping[pkt->stream_index] != -1)){  // 只处理目标流
      // 8.1 对index转换
      pkt->stream_index = stream_mapping[pkt->stream_index];
      // 8.2 对时间戳进行时基转换
      AVRational bq = ifmt_ctx->streams[pkt->stream_index]->time_base;
      AVRational cq = ofmt_ctx->streams[stream_mapping[pkt->stream_index]]->time_base;
      pkt->pts = av_rescale_q_rnd(pkt->pts, bq, cq, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
      pkt->dts = av_rescale_q_rnd(pkt->dts, bq, cq, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
      pkt->duration = av_rescale_q(pkt->duration, bq, cq);
      pkt->pos = -1;  // 新生成的数据包, 无位置信息可用
      // 8.3 将数据包按顺序写入输出
      ret = av_interleaved_write_frame(ofmt_ctx, pkt);
      if (ret < 0){
        av_log(NULL, AV_LOG_ERROR, "av_interleaved_write_frame failed\n");
        goto close_pkt;
      }
    }
    // 8.4 解引用packet
    av_packet_unref(pkt);
  }

  // 9. 将流尾写入输出
  av_write_trailer(ofmt_ctx);

  // 10. 释放资源
close_pkt:
  av_packet_free(&pkt);
close_io:
  if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
    avio_closep(&ofmt_ctx->pb);
close_map:
  av_freep(&stream_mapping);
close_output:
  avformat_free_context(ofmt_ctx);
close_input:
  avformat_close_input(&ifmt_ctx);
  return ret;
}

