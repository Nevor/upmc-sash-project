/*
 * Author : Roven Gabriel <nevor@nevor.net>
 *  student at Université Pierre et Marie Curie, Paris, France    
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Utility routines.
 */

#ifndef EVAL_UPMC_H
#define EVAL_UPMC_H

enum kind_en { Nothing, Variable, Plus, Minus, Mult, Div, Integer };

union value {
    int num; // for Integer
    char* var;
};

struct ast_st {
    enum kind_en kind;

    struct ast_st* left;
    struct ast_st* right;

    union value value;
};

struct ast_st* new_node(void);

struct ast_st* make_integer(int value);

struct ast_st* make_variable(char* name);

struct ast_st* make_plus(struct ast_st* left, struct ast_st* right);

struct ast_st* make_minus(struct ast_st* left, struct ast_st* right);

struct ast_st* make_mult(struct ast_st* left, struct ast_st* right);

struct ast_st* make_div(struct ast_st* left, struct ast_st* right);


struct ast_st*
parseStr(const char * str);

int 
evalArith(struct ast_st* ast);

void 
getLeftRightValue(struct ast_st* ast, int* left_value, int* right_value);

struct ast_st* 
simplifyAst(struct ast_st* ast);

#endif

