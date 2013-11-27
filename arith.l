/*
 * Author : Roven Gabriel <nevor@nevor.net>
 *  student at Université Pierre et Marie Curie, Paris, France    
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Arithmetic lexer.
 */

%{ 
    #include <stdlib.h> 
    #include "arith_parser.h"

%}

%option header-file="arith_lexer.h"

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%%
\$[a-zA-Z]+ { yylval->str = yytext; return VARIABLE; }
 
[0-9]+  { yylval->integer = atoi(yytext); return INTEGER; } 
 
\+      { return PLUS; } 

-       { return MINUS; } 

\*      { return MULT; } 


\/      { return DIV; } 

.       { }
 
%%


