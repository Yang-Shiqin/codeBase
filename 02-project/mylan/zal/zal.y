%{
#include <stdio.h>
#include "zal_in.h"
extern int yylex();
extern char *yytext;

int
yyerror(char const *str)
{    
    char *near_token=NULL;
    if(yytext[0]=='\0'){
        near_token="EOF";
    }else{
        near_token=yytext;
    }
    zal_compile_error(PARSE_ERR, near_token);
    return 0;
}

#define YYDEBUG 1
%}
%union {
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
}
%token <expr>       INT_LITERAL
%token <expr>       DOUBLE_LITERAL
%token <expr>       STRING_LITERAL
%token <identifier> IDENTIFIER
%token FUNCTION IF ELIF ELSE WHILE FOR RETURN CONTINUE BREAK NULL_T 
        TRUE_T FALSE_T LP RP LC RC LB RB SEMICOLON COMMA LOGIC_AND 
        LOGIC_OR ASSIGN EQ NE GE LE GT LT ADD SUB MUL DIV MOD INC DEC DOT
        ADD_ASSIGN_T SUB_ASSIGN_T MUL_ASSIGN_T DIV_ASSIGN_T MOD_ASSIGN_T

%type <para_list>   para_list
%type <argv_list>   argv_list
%type <expr>        expr logical_or_expr logical_and_expr eq_ne_expr
        compare_expr add_sub_expr math_expr minus_expr in_de_expr 
        pri_expr condition array_literal l_expr
%type <expr_list>   expr_list
%type <state>       state if_state for_state while_state return_state 
        break_state continue_state
%type <state_list>  state_list
%type <block>       block
%type <elif>        elif
%type <assign_operator> assign_operator

%%

script
    : func_def_or_state
    | script func_def_or_state
    ;

func_def_or_state
    : func_def
    | state
    {
        ZAL_Interpreter* inter = zal_get_current_inter();
        inter->state_list = zal_add_statement_to_list(inter->state_list, $1);
    }
    ;

func_def
    : FUNCTION IDENTIFIER LP para_list RP block
    {
        zal_inter_create_function($2, $4, $6);
    }
	| FUNCTION IDENTIFIER LP RP block
    {
        zal_inter_create_function($2, NULL, $5);
    }
    ;

state
    : expr SEMICOLON
    {
        $$ = zal_create_expr_statement($1);
    }
    | if_state
    | for_state
    | while_state
    | return_state
    | break_state
    | continue_state
    ;

para_list
    : IDENTIFIER
    {
        $$ = zal_create_parameter_list($1);
    }
	| para_list COMMA IDENTIFIER
    {
        $$ = zal_add_parameter_to_list($1, $3);
    }
    ;

block
    : LC state_list RC
    {
        $$ = zal_create_block($2);
    }
	| LC RC
    {
        $$ = zal_create_block(NULL);
    }
    ;

expr_list
    : /*null*/
    {
        $$ = NULL;
    }
	| expr
    {
        $$ = zal_create_expr_list($1);
    }
	| expr_list COMMA expr
    {
        $$ = zal_add_expr_to_list($1, $3);
    }
    ;

if_state
    : IF LP condition RP block
    {
        $$ = zal_create_if_statement($3, $5, NULL, NULL);
    }
	| IF LP condition RP block elif
    {
        $$ = zal_create_if_statement($3, $5, $6, NULL);
    }
	| IF LP condition RP block ELSE block
    {
        $$ = zal_create_if_statement($3, $5, NULL, $7);
    }
    | IF LP condition RP block elif ELSE block
    {
        $$ = zal_create_if_statement($3, $5, $6, $8);
    }
    ;

for_state
    : FOR LP expr SEMICOLON condition SEMICOLON expr RP block
    {
        $$ = zal_create_for_statement($3, $5, $7, $9);
    }
    ;

while_state
    : WHILE LP condition RP block
    {
        $$ = zal_create_while_statement($3, $5);
    }
    ;

return_state
    : RETURN expr SEMICOLON
    {
        $$ = zal_create_return_statement($2);
    }
    ;

break_state
    : BREAK SEMICOLON
    {
        $$ = zal_create_break_statement();
    }
    ;

continue_state
    : CONTINUE SEMICOLON
    {
        $$ = zal_create_continue_statement();
    }
    ;

state_list
    : state
    {
        $$ = zal_create_statement_list($1);
    }
	| state_list state
    {
        $$ = zal_add_statement_to_list($1, $2);
    }
    ;

expr
	: logical_or_expr
    | l_expr assign_operator expr
    {
        $$ = zal_create_assign_expr($1, $2, $3);
    }
    ;

assign_operator
    : ASSIGN
    {
        $$ = NORMAL_ASSIGN;
    }
    | ADD_ASSIGN_T
    {
        $$ = ADD_ASSIGN;
    }
    | SUB_ASSIGN_T
    {
        $$ = SUB_ASSIGN;
    }
    | MUL_ASSIGN_T
    {
        $$ = MUL_ASSIGN;
    }
    | DIV_ASSIGN_T
    {
        $$ = DIV_ASSIGN;
    }
    | MOD_ASSIGN_T
    {
        $$ = MOD_ASSIGN;
    }
    ;

condition
    : expr
    {
        $$ = $1;
    }
	| expr_list COMMA expr
    {
        $$ = $3;
    }
    ;

elif
    : ELIF LP condition RP block
    {
        $$ = zal_create_elif($3, $5);
    }
	| elif ELIF LP condition RP block
    {
        $$ = zal_add_elif_to_list($1, $4, $6);
    }
    ;

l_expr
    : IDENTIFIER
    {
        $$ = zal_create_identifier_expr($1);
    }
	| l_expr LB expr RB
    {
        $$ = zal_create_index_expr($1, $3);
    }
    ;

logical_or_expr
    : logical_and_expr
	| logical_or_expr LOGIC_OR logical_and_expr
    {
        $$ = zal_create_binary_expr(LOGICAL_OR_EXPRESSION, $1, $3);
    }
    ;

logical_and_expr
    : eq_ne_expr
	| logical_and_expr LOGIC_AND eq_ne_expr
    {
        $$ = zal_create_binary_expr(LOGICAL_AND_EXPRESSION, $1, $3);
    }
    ;

eq_ne_expr
    : compare_expr
	| eq_ne_expr EQ compare_expr
    {
        $$ = zal_create_binary_expr(EQ_EXPRESSION, $1, $3);
    }
	| eq_ne_expr NE compare_expr
    {
        $$ = zal_create_binary_expr(NE_EXPRESSION, $1, $3);
    }
    ;

compare_expr
	: add_sub_expr
	| compare_expr GT add_sub_expr
    {
        $$ = zal_create_binary_expr(GT_EXPRESSION, $1, $3);
    }
	| compare_expr GE add_sub_expr
    {
        $$ = zal_create_binary_expr(GE_EXPRESSION, $1, $3);
    }
	| compare_expr LT add_sub_expr
    {
        $$ = zal_create_binary_expr(LT_EXPRESSION, $1, $3);
    }
	| compare_expr LE add_sub_expr
    {
        $$ = zal_create_binary_expr(LE_EXPRESSION, $1, $3);
    }
    ;

add_sub_expr
    : math_expr
	| add_sub_expr ADD math_expr
    {
        $$ = zal_create_binary_expr(ADD_EXPRESSION, $1, $3);
    }
	| add_sub_expr SUB math_expr
    {
        $$ = zal_create_binary_expr(SUB_EXPRESSION, $1, $3);
    }
    ;

math_expr
	: minus_expr
	| math_expr MUL minus_expr
    {
        $$ = zal_create_binary_expr(MUL_EXPRESSION, $1, $3);
    }
	| math_expr DIV minus_expr
    {
        $$ = zal_create_binary_expr(DIV_EXPRESSION, $1, $3);
    }
	| math_expr MOD minus_expr
    {
        $$ = zal_create_binary_expr(MOD_EXPRESSION, $1, $3);
    }
    ;

minus_expr
    : in_de_expr
	| SUB pri_expr
    {
        $$ = zal_create_minus_expr($2);
    }
    ;

in_de_expr
	: pri_expr
	| pri_expr INC
    {
        $$ = zal_create_inc_dec_expr($1, INCREMENT_EXPRESSION);
    }
	| pri_expr DEC
    {
        $$ = zal_create_inc_dec_expr($1, DECREMENT_EXPRESSION);
    }
    ;

pri_expr
	: INT_LITERAL
	| DOUBLE_LITERAL
	| STRING_LITERAL
	| array_literal
	| IDENTIFIER LP argv_list RP
    {
        $$ = zal_create_func_call_expr($1, $3);
    }
    | IDENTIFIER LP RP
    {
        $$ = zal_create_func_call_expr($1, NULL);
    }
    | l_expr DOT IDENTIFIER LP argv_list RP
    {
        $$ = zal_create_method_call_expr($1, $3, $5);
    }
    | l_expr DOT IDENTIFIER LP RP
    {
        $$ = zal_create_method_call_expr($1, $3, NULL);
    }
	| LP expr RP
    {
        $$ = $2;
    }
	| IDENTIFIER
    {
        $$ = zal_create_identifier_expr($1);
    }
	| NULL_T
    {
        $$ = zal_create_null_expr();
    }
	| TRUE_T
    {
        $$ = zal_create_bool_expr(ZAL_TRUE);
    }
	| FALSE_T
    {
        $$ = zal_create_bool_expr(ZAL_FALSE);
    }
    ;

array_literal
    : LC expr_list RC
    {
        $$ = zal_create_array_expr($2);
    }
	| LC expr_list COMMA RC
    {
        $$ = zal_create_array_expr($2);
    }
    ;

argv_list
    : expr
    {
        $$ = zal_create_argument_list($1);
    }
	| argv_list COMMA expr
    {
        $$ = zal_add_argument_to_list($1, $3);
    }
    ;