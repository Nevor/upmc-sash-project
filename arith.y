/*
 * Author : Roven Gabriel <nevor@nevor.net>
 *  student at Université Pierre et Marie Curie, Paris, France    
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Arithmetic parser.
 */

%define api.pure
%parse-param {struct ast_st** ast}
%parse-param { yyscan_t scanner }
%lex-param   { yyscan_t scanner }


%code requires {
 
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
 
}

%union {
    int integer;
    char* str;
    struct ast_st* ast; 
}

%defines "arith_parser.h"

%token <integer> INTEGER 
%token <str> VARIABLE  
%token PLUS MINUS MULT DIV 
%left PLUS MINUS
%left MULT DIV

%start arith_expr

%type <ast> expr

%{ 
    #include "eval-upmc.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    int yyerror(struct ast_st** ast, yyscan_t scanner, char *s);

    int yyerror(struct ast_st** ast, yyscan_t scanner, char *s) { 
        (void)ast;
        (void)scanner;

        fprintf(stderr, "%s\n", s); 

        return 0;
    }

    struct ast_st* new_node()
    {
        return (struct ast_st*)malloc(sizeof(struct ast_st));
    }

    struct ast_st* make_integer(int value)
    {
        struct ast_st* result = new_node();
        result->kind = Integer;
        result->value.num = value;

        return result;
    }

    struct ast_st* make_variable(char* name)
    {
        struct ast_st* result = new_node();
        result->kind = Variable;
        result->value.var = name;

        return result;
    }

    struct ast_st* make_plus(struct ast_st* left, struct ast_st* right)
    {
        struct ast_st* result = new_node();
        result->kind = Plus;
        result->left = left;
        result->right = right;

        return result;
    }

    struct ast_st* make_minus(struct ast_st* left, struct ast_st* right)
    {
        struct ast_st* result = new_node();
        result->kind = Minus;
        result->left = left;
        result->right = right;

        return result;
    }
    
    struct ast_st* make_mult(struct ast_st* left, struct ast_st* right)
    {
        struct ast_st* result = new_node();
        result->kind = Mult;
        result->left = left;
        result->right = right;

        return result;
    }

    struct ast_st* make_div(struct ast_st* left, struct ast_st* right)
    {
        struct ast_st* result = new_node();
        result->kind = Div;
        result->left = left;
        result->right = right;

        return result;
    }
%} 

%% 

arith_expr:
   expr { *ast = $1; }

expr: 
   INTEGER  { $$ = make_integer($1); } 
 | VARIABLE { $$ = make_variable(strdup($1)); } 
 | expr PLUS expr { $$ = make_plus($1, $3); } 
 | expr MINUS expr { $$ = make_minus($1, $3); } 
 | expr MULT expr { $$ = make_mult($1, $3); } 
 | expr DIV expr { $$ = make_div($1, $3); } 
 | '(' expr ')' { $$ = $2; }  
 ; 
 
%% 
 
