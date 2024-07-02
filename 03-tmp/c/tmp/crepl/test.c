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
#define A "aa"
#define B A "bb"

int main()
{
    mkdir("as/", 0755);
    printf("Hello world %s\n", A B);
    return 0;
}

