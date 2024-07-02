#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>
#include <regex.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define FNAME_LEN_MAX (32)

int get_func_name(const char *line, char *dst){
  const char *pattern = "int\\s+([a-zA-Z_]\\w*)\\s*\\(";
  regex_t regex;
  regmatch_t matches[2];
  // 编译正则表达式
  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    printf("Failed to compile regular expression\n");
    return -1;
  }
  // 执行正则匹配
  if (regexec(&regex, line, 2, matches, 0) == 0) {
    // 提取匹配到的函数名
    int len = matches[1].rm_eo-matches[1].rm_so;
    if (len>=30){
      regfree(&regex);
      return -1;
    }
    strncpy(dst, line+matches[1].rm_so, len);
    dst[len] = 0;
    regfree(&regex);
    return 0;
  } else {
    printf("No match found\n");
    regfree(&regex);
    return -1;
  }
}

// [ ] todo: 管道, gcc错误则删掉文件, 错误信息需要管道来传递
// 创建函数文件, 若创建匿名函数则生成链接所有.c的动态链接库
int create_func(const char* name, const char *line, char *files_name[], int* tail){
  // fork?
  int pipefd[2];
  if (-1==pipe(pipefd)){
    perror("pipe");
    return -1;
  }
  // todo
  pid_t pid=fork();
  if (-1==pid){
    perror("fork");
    return -1;
  }else if (pid==0){  // child, 写
    close(pipefd[0]);
    dup2(pipefd[1], STDERR_FILENO);
    close(STDOUT_FILENO);
    FILE *file = NULL;
    char file_name[64] = {"/tmp/crepl/"};
    char *args[128] = {"gcc", "-fPIC", "-shared", "-o", "all.so"};
    int argc;
    for (argc=0; argc < *tail; argc++) {
      args[argc+5] = files_name[argc];
    }
    argc += 5;
    args[argc] = NULL;
    if (NULL==name){  // 匿名文件, 用mkstemp生成临时文件(但感觉不是很有必要)
      // strcat(file_name, "anonXXXXXX");
      // int fd = mkstemp(file_name);
      // if (fd == -1) {
      //   perror("mkstemp");
      //   return -1;
      // }
      // write(fd, "int ____________________(){ return ", 36);
      // write(fd, line, strlen(line));
      // write(fd, ";}", 3);
      // // 完成操作后关闭文件描述符
      // close(fd);
      strcat(file_name, "-.c");
      file = fopen(file_name, "w");
      if (file == NULL) {
        printf("Error opening file\n");
        return -1;
      }
      fprintf(file, "int ____________________(){ return %s;}", line);
      fclose(file);
      args[argc++] = file_name;
      args[argc] = NULL;
      // 动态链接: 为了调用
      execvp("gcc", args);
      // 咋删临时文件
    }else{
      // 写文件
      file = fopen(name, "w");
      if (file == NULL) {
        printf("Error opening file\n");
        return -1;
      }
      fprintf(file, "%s", line);
      fclose(file);
      // 编译成动态链接库: 为了判断这个文件有没有问题
      execvp("gcc", args);
      return -1;    // 失败则会运行到这
    }
    perror("execlp");
    return -1;
  }else{  // parent, 读
    close(pipefd[1]);
    char buf[256] = {0};
    if ((read(pipefd[0], buf, sizeof(buf)-1)) > 0){ // 输出stderr就算报错
      printf("%s\n", buf);
      while((read(pipefd[0], buf, sizeof(buf)-1)) > 0) printf("%s\n", buf);
      return -1;
    }
    wait(NULL);
    return 0;
  }
  return 0;
}

int call_func(int *res){
  char *error;
  int (*func)()=NULL;
  void *handle = dlopen("/tmp/crepl/all.so", RTLD_LAZY);
  if ((error=dlerror())!=NULL){
    fprintf(stderr, "%s\n", error);
    return -1;
  }
  func = (int(*))dlsym(handle, "____________________");
  if ((error=dlerror())!=NULL){
    fprintf(stderr, "%s\n", error);
    return -1;
  }
  *res = func();
  dlclose(handle);
  return 0;
}

int parse(const char * line, char *files_name[], int* tail){
  assert(line);
  char name[64]={"/tmp/crepl/"};  // 太长不要
  while(*line==' ') line++;
  if (*line=='\n') return 0;
  if (strstr(line, "int ")==line){  // 函数
    // 1. 检查并获取函数命名
    if(0==get_func_name(line, name+strlen(name))){ // 没有检查有没有重复(题目里说没重复定义, 遇到可以算undefine behavior),不过其实没问题
      strcat(name, ".c");
      files_name[(*tail)++] = strdup(name);
      // 2. 创建函数
      if(-1==create_func(name, line, files_name, tail)){
        (*tail)--; // 失败则去掉当前函数
      }
    }
    // printf("func: Got %zu chars.\n", strlen(line)); // ??
  }else{  // 表达式
    // 1. 创建匿名函数
    if(-1!=create_func(NULL, line, files_name, tail)){
      int res;
      // 调用匿名函数
      call_func(&res);
      printf("%d\n", res);
    }
    printf("expr: Got %zu chars.\n", strlen(line)); // ??
  }
  return 0;
}

int main(int argc, char *argv[]) {
  static char line[4096];
  char* files_name[128] = {0};    // 最多100个函数
  int tail=0;
  int result = mkdir("/tmp/crepl", 0755);
  if (result != 0) {
    if (errno != EEXIST) {
      printf("Error creating directory: %d\n", errno);
      return 1;
    }
  }
  while (1) {
    printf("crepl> ");
    fflush(stdout);
    if (!fgets(line, sizeof(line), stdin)) {
      break;
    }
    parse(line, files_name, &tail);
  }
}
