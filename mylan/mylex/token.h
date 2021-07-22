#ifndef __TOKEN_H__
#define __TOKEN_H__

#define MAX_TOKEN_SIZE (56)

typedef enum{
    BAD_TOKEN,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    CR
}TokenType;

typedef struct token{
    TokenType type;
    char str[MAX_TOKEN_SIZE];
    double value;
}Token;

void get_token(Token* token);
void set_line(char* line);

#endif