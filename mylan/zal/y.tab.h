/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_LITERAL = 258,
    DOUBLE_LITERAL = 259,
    STRING_LITERAL = 260,
    IDENTIFIER = 261,
    FUNCTION = 262,
    IF = 263,
    ELIF = 264,
    ELSE = 265,
    WHILE = 266,
    FOR = 267,
    RETURN = 268,
    CONTINUE = 269,
    BREAK = 270,
    NULL_T = 271,
    TRUE_T = 272,
    FALSE_T = 273,
    LP = 274,
    RP = 275,
    LC = 276,
    RC = 277,
    LB = 278,
    RB = 279,
    SEMICOLON = 280,
    COMMA = 281,
    LOGIC_AND = 282,
    LOGIC_OR = 283,
    ASSIGN = 284,
    EQ = 285,
    NE = 286,
    GE = 287,
    LE = 288,
    GT = 289,
    LT = 290,
    ADD = 291,
    SUB = 292,
    MUL = 293,
    DIV = 294,
    MOD = 295,
    INC = 296,
    DEC = 297,
    DOT = 298,
    ADD_ASSIGN_T = 299,
    SUB_ASSIGN_T = 300,
    MUL_ASSIGN_T = 301,
    DIV_ASSIGN_T = 302,
    MOD_ASSIGN_T = 303
  };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define IDENTIFIER 261
#define FUNCTION 262
#define IF 263
#define ELIF 264
#define ELSE 265
#define WHILE 266
#define FOR 267
#define RETURN 268
#define CONTINUE 269
#define BREAK 270
#define NULL_T 271
#define TRUE_T 272
#define FALSE_T 273
#define LP 274
#define RP 275
#define LC 276
#define RC 277
#define LB 278
#define RB 279
#define SEMICOLON 280
#define COMMA 281
#define LOGIC_AND 282
#define LOGIC_OR 283
#define ASSIGN 284
#define EQ 285
#define NE 286
#define GE 287
#define LE 288
#define GT 289
#define LT 290
#define ADD 291
#define SUB 292
#define MUL 293
#define DIV 294
#define MOD 295
#define INC 296
#define DEC 297
#define DOT 298
#define ADD_ASSIGN_T 299
#define SUB_ASSIGN_T 300
#define MUL_ASSIGN_T 301
#define DIV_ASSIGN_T 302
#define MOD_ASSIGN_T 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 22 "zal.y"

    char            *identifier;
    ParameterList   *para_list;
    ArgumentList    *argv_list;
    Expression      *expr;
    ExpressionList  *expr_list;
    Statement       *state;
    StatementList   *state_list;
    Block           *block;
    Elif            *elif;
    AssignOperator  assign_operator;

#line 166 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
