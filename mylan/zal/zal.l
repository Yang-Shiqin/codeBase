%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "zal_in.h"
#include "y.tab.h"

int yywrap(void){
    return 1;
}

static void increase_line_number(void){
    zal_get_current_inter()->line_number++;
}
%}
%start COMMENT IN_STRING_SIG IN_STRING_DBL

%%

<INITIAL>"function"     return FUNCTION;
<INITIAL>"if"           return IF;
<INITIAL>"elif"         return ELIF;
<INITIAL>"else"         return ELSE;
<INITIAL>"while"        return WHILE;
<INITIAL>"for"          return FOR;
<INITIAL>"return"       return RETURN;
<INITIAL>"continue"     return CONTINUE;
<INITIAL>"break"        return BREAK;
<INITIAL>"null"         return NULL_T;
<INITIAL>"true"         return TRUE_T;
<INITIAL>"false"        return FALSE_T;
<INITIAL>"("            return LP;
<INITIAL>")"            return RP;
<INITIAL>"{"            return LC;
<INITIAL>"}"            return RC;
<INITIAL>"["            return LB;
<INITIAL>"]"            return RB;
<INITIAL>";"            return SEMICOLON;
<INITIAL>","            return COMMA;
<INITIAL>"&&"           return LOGIC_AND;
<INITIAL>"||"           return LOGIC_OR;
<INITIAL>"="            return ASSIGN;
<INITIAL>"=="           return EQ;
<INITIAL>"!="           return NE;
<INITIAL>">="           return GE;
<INITIAL>"<="           return LE;
<INITIAL>">"            return GT;
<INITIAL>"<"            return LT;
<INITIAL>"+"            return ADD;
<INITIAL>"-"            return SUB;
<INITIAL>"*"            return MUL;
<INITIAL>"/"            return DIV;
<INITIAL>"%"            return MOD;
<INITIAL>"++"           return INC;
<INITIAL>"--"           return DEC;
<INITIAL>"."            return DOT;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    yylval.identifier = zal_create_identifier(yytext);
    return IDENTIFIER;
}
<INITIAL>([0-9]+)|"0" {
    Expression *expr = zal_alloc_expr(INT_EXPRESSION);
    sscanf(yytext, "%d", &(expr->u.int_expr));
    yylval.expr = expr;
    return INT_LITERAL;
}
<INITIAL>([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*) {
    Expression *expr = zal_alloc_expr(DOUBLE_EXPRESSION);
    sscanf(yytext, "%lf", &(expr->u.double_expr));
    yylval.expr = expr;
    return DOUBLE_LITERAL;
}
<INITIAL>\" {
    zal_open_str_literal();
    BEGIN IN_STRING_DBL;
}
<INITIAL>\' {
    zal_open_str_literal();
    BEGIN IN_STRING_SIG;
}
<INITIAL>[ \t] ;
<INITIAL>\n {
    increase_line_number();
}
<INITIAL># BEGIN COMMENT;
<INITIAL>. {
    char buf[LINE_BUF_SIZE];
    if(isprint(yytext[0])) {
        buf[0] = yytext[0];
        buf[1] = '\0';
    }else{
        sprintf(buf, "0x%02x", (unsigned char)yytext[0]);
    }

    // zal_compile_error(CHARACTER_INVALID_ERR/*TODO*/);
}

<COMMENT>\n {
    increase_line_number();
    BEGIN INITIAL;
}
<COMMENT>. ;

<IN_STRING_DBL>\" {
    Expression *expr = zal_alloc_expr(STRING_EXPRESSION);
    expr->u.string_expr = zal_close_str_literal();
    yylval.expr = expr;
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<IN_STRING_DBL>\n {
    zal_cat_str_literal('\n');
    increase_line_number();
}
<IN_STRING_DBL>\\\" zal_cat_str_literal('"');
<IN_STRING_DBL>\\n  zal_cat_str_literal('\n');
<IN_STRING_DBL>\\t  zal_cat_str_literal('\t');
<IN_STRING_DBL>\\\\ zal_cat_str_literal('\\');
<IN_STRING_DBL>\\\' zal_cat_str_literal('\'');
<IN_STRING_DBL>\'   zal_cat_str_literal('\'');
<IN_STRING_DBL>.    zal_cat_str_literal(yytext[0]);

<IN_STRING_SIG>\' {
    Expression *expr = zal_alloc_expr(STRING_EXPRESSION);
    expr->u.string_expr = zal_close_str_literal();
    yylval.expr = expr;
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<IN_STRING_SIG>\n {
    zal_cat_str_literal('\n');
    increase_line_number();
}
<IN_STRING_SIG>\\\' zal_cat_str_literal('\'');
<IN_STRING_SIG>\\n  zal_cat_str_literal('\n');
<IN_STRING_SIG>\\t  zal_cat_str_literal('\t');
<IN_STRING_SIG>\\\\ zal_cat_str_literal('\\');
<IN_STRING_SIG>\\\" zal_cat_str_literal('"');
<IN_STRING_SIG>\"   zal_cat_str_literal('"');
<IN_STRING_SIG>.    zal_cat_str_literal(yytext[0]);

%%