// gcc 02__info_meta.c -lavformat -lavutil
// ./a.out

#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main()
{
  int ret;
  AVFormatContext *fmt_ctx = NULL;
  char *input = "../data/fly.mp4";  // 输入文件

  ret = avformat_open_input(&fmt_ctx, input, NULL, NULL); // 打开输入文件
  if (ret < 0){
    return -1;
  }
  av_dump_format(fmt_ctx, 0, input, 0); // 打印文件信息
  avformat_close_input(&fmt_ctx); // 关闭输入文件
  return 0;
}

