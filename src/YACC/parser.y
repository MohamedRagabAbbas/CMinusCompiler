%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"  /* Bison-generated header */

extern int yylex();
extern int line_num;
extern int char_pos;
extern char *yytext;
extern FILE *yyin;  /* Declare yyin */

void yyerror(const char *s);
%}

/* Token Definitions */
%token PROGRAM ID NUM INT FLOAT IF ELSE WHILE RELOP ADDOP MULOP ASSIGNOP
%token '{' '}' '(' ')' ';' ',' '[' ']' 

/* Operator Precedence and Associativity */
%left RELOP
%left ADDOP
%left MULOP

/* Start Symbol */
%start program

%%

program:
    PROGRAM ID '{' declaration_list statement_list '}'
    {
        printf("Parsing completed successfully\n");
    }
    ;

declaration_list:
    /* empty */
    | declaration_list var_declaration
    ;

var_declaration:
    type_specifier ID ';'
    | type_specifier ID '[' NUM ']' ';'
    ;

type_specifier:
    INT
    | FLOAT
    ;

statement_list:
    /* empty */
    | statement_list statement
    ;

statement:
    assignment_stmt
    | compound_stmt
    | selection_stmt
    | iteration_stmt
    ;

assignment_stmt:
    var ASSIGNOP expression ';'
    ;

var:
    ID
    | ID '[' expression ']'
    ;

expression:
    additive_expression
    | additive_expression RELOP additive_expression
    ;

additive_expression:
    term
    | additive_expression ADDOP term
    ;

term:
    factor
    | term MULOP factor
    ;

factor:
    '(' expression ')'
    | var
    | NUM
    ;

compound_stmt:
    '{' declaration_list statement_list '}'
    ;

selection_stmt:
    IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    ;

iteration_stmt:
    WHILE '(' expression ')' statement
    ;

%%

/* Error Handling Function */
void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d, pos %d: %s near '%s'\n",
            line_num, char_pos, s, yytext);
    exit(1);
}

/* Main Function */
int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *infile = fopen(argv[1], "r");
        if (!infile) {
            perror("Error opening file");
            return 1;
        }
        yyin = infile;  /* Assign input file to yyin */
    }
    if (yyparse() == 0) {
        printf("Parsing completed successfully\n");
    }
    return 0;
}
