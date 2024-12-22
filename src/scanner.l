%{
#include "tokens.h"

int line_num = 1;
int char_pos = 1;
%}

%option noyywrap

ID      [a-zA-Z][a-zA-Z0-9]*([._@][a-zA-Z0-9])*
NUM     [0-9]+(\.[0-9]+)?([Ee][+-]?[0-9]+)?

%%

"Program"       { return PROGRAM; }
"int"           { return INT; }
"float"         { return FLOAT; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"<="|"=="|"!="|">="|"<"|">" { return RELOP; }
"+"|"-"         { return ADDOP; }
"*"|"/"         { return MULOP; }
"="             { return ASSIGNOP; }
"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
";"             { return ';'; }
","             { return ','; }
"["             { return '['; }
"]"             { return ']'; }
{ID}            { return ID; }
{NUM}           { return NUM; }
[ \t]+          { char_pos += yyleng; }
\n              { line_num++; char_pos = 1; }
.               { printf("Error: Invalid character '%s' at line %d, pos %d\n", yytext, line_num, char_pos);
                  char_pos += yyleng;
                }

%%
