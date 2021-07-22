#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

#define LINE_BUF_SIZE (256)

int tmp_exist;
static Token tmp_token;

void my_get_token(Token* token);
void unget_token(Token* token);
double parse_line();
double parse_expression();
double parse_term();
double parse_pri_expression();

void my_get_token(Token* token){
    if(tmp_exist){
        tmp_exist = 0;
        *token = tmp_token;
    }else{
        get_token(token);
    }
}

void unget_token(Token* token){
    tmp_exist = 1;
    tmp_token.type = token->type;
    strcpy(tmp_token.str, token->str);
    tmp_token.value = token->value;
}

double parse_line(){
    tmp_exist = 0;
    return parse_expression();
}

double parse_expression(){
    Token token;
    double v1, v2;
    v1 = parse_term();
    while(1){
        my_get_token(&token);
        if(token.type!=ADD && token.type!=SUB){
            unget_token(&token);
            return v1;
        }
        v2 = parse_term();
        if(token.type==ADD){
            v1 += v2;
        }else{
            v1 -= v2;
        }
    }
}

double parse_term(){
    Token token;
    double v1, v2;
    v1 = parse_pri_expression();
    while(1){
        my_get_token(&token);
        if(token.type!=MUL && token.type!=DIV){
            unget_token(&token);
            return v1;
        }
        v2 = parse_pri_expression();
        if(token.type==MUL){
            v1 *= v2;
        }else{
            v1 /= v2;
        }
    }
}
    
double parse_pri_expression(){
    Token token;
    my_get_token(&token);
    if(token.type==NUM)
        return token.value;
    fprintf(stderr, "syntax error.\n");
    exit(EXIT_FAILURE);
}

int main(){
    char line[LINE_BUF_SIZE];
    double value;

    printf("calc: ");
    while (fgets(line, LINE_BUF_SIZE, stdin) != NULL) {
        set_line(line);
        value = parse_line();
        printf(">>%f\n", value);
        printf("calc: ");
    }

    return 0;
}