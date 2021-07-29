#ifndef __TOKEN_H__
#define __TOKEN_H__

#define MAX_TOKEN_SIZE (56)

typedef enum{
    BAD_TOKEN,  // 无效记号
    NUM,        // 数字
    LP,         // (
    RP,
    ADD,
    SUB,
    MUL,
    DIV,
    CR          // 行尾
}TokenType;

typedef struct token{
    TokenType type;
    char str[MAX_TOKEN_SIZE];
    double value;
}Token;

void get_token(Token* token);
void set_line(char* line);

#endif