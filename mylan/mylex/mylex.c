#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

typedef enum{
    INIT,
    IN_INT_PART,
    DOT_PART,
    IN_FRAC_PART
}LexStatus;


int g_line_pos;
char* g_line;


void get_token(Token* token){
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


void set_line(char* line){
    g_line_pos = 0;
    g_line = line;
}