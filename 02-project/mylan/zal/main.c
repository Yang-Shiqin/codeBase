#include <stdio.h>
#include <stdlib.h>
#include "ZAL.h"


int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "usage:./%s filename", argv[0]);
        exit(1);
    }
    FILE *fp=NULL;
    if ((fp=fopen(argv[1], "r"))==NULL)
    {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(0);
    }
    ZAL_Interpreter *inter=ZAL_create_interpreter();
    ZAL_compile(inter, fp);
    ZAL_interpret(inter);
    ZAL_destroy_interpreter(inter);

    return 0;
}