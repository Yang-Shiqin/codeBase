#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

typedef enum{
    INIT,               // 初始态
    IN_INT_PART,        // 整数部分(小数的整数/整数)
    DOT_PART,           // 点(小数的点)
    IN_FRAC_PART        // 小数部分
}LexStatus;             // 词法分析状态


int g_line_pos;         // 解析到该行的位置
char* g_line;           // 正在解析的行

// 解析当前位置的Token并把Token切出来返回(1个)
void get_token(Token* token){   // 出参
    LexStatus status = INIT;
    char cur;
    int str_pos = 0;
    token->type = BAD_TOKEN;
    while((cur=g_line[g_line_pos]) != '\0'){
        if((status==IN_INT_PART || status==IN_FRAC_PART)
            && !(isdigit(cur) || cur=='.')){    // NUM
                token->type = NUM;
                sscanf(token->str, "%lf", &(token->value));
                return;
            }

        if(isspace(cur)){
            if(cur == '\n'){
                token->type = CR;
                return;
            }
            g_line_pos++;
            continue;
        }

        if(str_pos>=MAX_TOKEN_SIZE-1){
            fprintf(stderr, "token too long!!!!!!!\n");
            exit(EXIT_FAILURE);
        }
        token->str[str_pos] = g_line[g_line_pos++];
        str_pos++;
        token->str[str_pos] = '\0';

        switch (cur)
        {
        case '+':
            token->type = ADD;
            return;
        case '-':
            token->type = SUB;
            return;
        case '*':
            token->type = MUL;
            return;
        case '/':
            token->type = DIV;
            return;
        case '(':
            token->type = LP;
            return;
        case ')':
            token->type = RP;
            return;
        case '.':
            if(status==IN_INT_PART){
                status = DOT_PART;
            }else{
            fprintf(stderr, "syntax error.\n");
            exit(EXIT_FAILURE);
            }
            break;
        default:
            break;
        }

        if(isdigit(cur)){
            if(status==INIT){
                status = IN_INT_PART;
            }else if(status==DOT_PART){
                status = IN_FRAC_PART;
            }
        }else{
            fprintf(stderr, "bad character(%c)\n", cur);
            exit(EXIT_FAILURE);
        }
    }

}

// 读入新的一行
void set_line(char* line){
    g_line_pos = 0;
    g_line = line;
}