#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

int parse(const char * line){
  assert(line);
  while(*line==' ') line++;
  if (*line=='\n') return 0;
  printf("Got %zu chars.\n", strlen(line)); // ??
  return 0;
}

int main(int argc, char *argv[]) {
  static char line[4096];
  while (1) {
    printf("crepl> ");
    fflush(stdout);
    if (!fgets(line, sizeof(line), stdin)) {
      break;
    }
    parse(line);
  }
}
