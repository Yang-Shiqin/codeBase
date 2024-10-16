/* 日志系统 */

// gcc 01__log.c -lavutil
// ./a.out

#include <libavutil/log.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  av_log_set_level(AV_LOG_INFO);  // 设置日志级别
  av_log(NULL, AV_LOG_INFO, "Hello, World!"); // 输出info级别日志
  return 0;
}
