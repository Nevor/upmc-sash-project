/*
 * Author : Roven Gabriel <nevor@nevor.net>
 *  student at Université Pierre et Marie Curie, Paris, France    
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Arithmetic evaluation routines.
 */

#include "sash.h"
#include "eval-upmc.h"
#include "arith_parser.h"
#include "arith_lexer.h"
#include <stdlib.h>
#include <stdio.h>


/* Parses a string and returns an AST */
struct ast_st*
parseStr(const char * str)
{
    struct ast_st* ast;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
        return NULL;
    }
 
    state = yy_scan_string(str, scanner);
 
    if (yyparse(&ast, scanner)) {
        return NULL;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    return ast;
}

/* Evaluates an AST */
int 
evalArith(struct ast_st* ast)
{
    char** buffer;

    switch(ast->kind) {
        case Nothing:
            return 0;
        case Integer:
            return ast->value.num;
        case Variable:
            expandVar(ast->value.var, (const char***)&buffer);
            return atoi(*buffer);
        case Plus:
            return evalArith(ast->left) + evalArith(ast->right);
        case Minus:
            return evalArith(ast->left) - evalArith(ast->right);
        case Mult:
            return evalArith(ast->left) * evalArith(ast->right);
        case Div:
            return evalArith(ast->left) / evalArith(ast->right);
    }

    return 0;
}

/* Extract the value of the left and right operand if they are integers */
void 
getLeftRightValue(struct ast_st* ast, int* left_value, int* right_value)
{
    struct ast_st* left = ast->left;
    struct ast_st* right = ast->right;
    
    if(left->kind == Integer) 
        *left_value = left->value.num;

    if(right->kind == Integer)
        *right_value = right->value.num;

}

/* Simplify an AST according to the question 6 b) */
struct ast_st* 
simplifyAst(struct ast_st* ast)
{
    if(ast == NULL) return NULL;

    int left_value, right_value;

    switch(ast->kind) {
        case Nothing:
            return ast;
        case Integer:
            return ast;
        case Variable:
            //expandVar(ast->value.var, (const char***)&buffer);
            //return atoi(*buffer);
            return ast;
        case Plus:
            ast->left = simplifyAst(ast->left);
            ast->right = simplifyAst(ast->right);

            getLeftRightValue(ast, &left_value, &right_value);

            if(ast->left->kind == Integer && left_value == 0) 
                return ast->right;
            else if(ast->right->kind == Integer && right_value == 0)
                return ast->left;
            else
                return ast;
        case Minus:
            ast->left = simplifyAst(ast->left);
            ast->right = simplifyAst(ast->right);

            getLeftRightValue(ast, &left_value, &right_value);

            if(ast->left->kind == Integer && 
               ast->right->kind == Integer &&
                left_value == right_value) 
            {
                return make_integer(0);
            }
  
            return ast;
        case Mult:
            ast->left = simplifyAst(ast->left);
            ast->right = simplifyAst(ast->right);

            getLeftRightValue(ast, &left_value, &right_value);

            if(ast->left->kind == Integer && left_value == 1) 
                return ast->right;
            else if(ast->right->kind == Integer && right_value == 1)
                return ast->left;
            else
                return ast;
        case Div:
            ast->left = simplifyAst(ast->left);
            ast->right = simplifyAst(ast->right);

            getLeftRightValue(ast, &left_value, &right_value);

            if(ast->left->kind == Integer && 
               ast->right->kind == Integer &&
                left_value == right_value) 
            {
                return make_integer(1);
            }
  
            return ast;
    }

    return ast;

}
