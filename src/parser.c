// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"
#include "symbol_table.h"

extern int yylex();
extern char *yytext;
extern int line_num;
extern int char_pos;
extern FILE *yyin;

int lookahead;

void program();
void declaration_list();
void var_declaration();
void type_specifier(VarType *type);
void statement_list();
void statement();
void assignment_stmt();
void var(char *name, VarType *type);
void expression(VarType *type, void *value);
void relop();
void additive_expression(VarType *type, void *value);
void addop();
void term(VarType *type, void *value);
void mulop();
void factor(VarType *type, void *value);
void compound_stmt();
void selection_stmt();
void iteration_stmt();

void match(int expectedToken);
void syntax_error(const char *expected);
void getTokenName(int token, char *name);
void semantic_error(const char *message);

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *infile = fopen(argv[1], "r");
        if (!infile) {
            perror("Error opening file");
            return 1;
        }
        yyin = infile;
    }
    init_symbol_table();
    lookahead = yylex();
    program();
    if (lookahead == 0) { 
        printf("Parsing completed successfully\n");
        print_symbol_table();
    } else {
        syntax_error("EOF");
    }
    free_symbol_table();
    return 0;
}

void program() {
    if (lookahead == PROGRAM) {
        match(PROGRAM);
        if (lookahead == ID) {
            match(ID);
            match('{');
            declaration_list();
            statement_list();
            match('}');
        } else {
            syntax_error("ID");
        }
    } else {
        syntax_error("PROGRAM");
    }
}

void declaration_list() {
    while (lookahead == INT || lookahead == FLOAT) {
        var_declaration();
    }
}

void var_declaration() {
    VarType type;
    type_specifier(&type);
    if (lookahead == ID) {
        char *name = strdup(yytext);
        match(ID);
        if (!insert_symbol(name, type, line_num)) {
            semantic_error("Variable redeclaration");
        }
        if (lookahead == ';') {
            match(';');
        } else if (lookahead == '[') {
            match('[');
            if (lookahead == NUM) {
                match(NUM);
                match(']');
                match(';');
                // Arrays can be handled here
            } else {
                syntax_error("NUM");
            }
        } else {
            syntax_error("';' or '['");
        }
        free(name);
    } else {
        syntax_error("ID");
    }
}

void type_specifier(VarType *type) {
    if (lookahead == INT) {
        match(INT);
        *type = INT_TYPE;
    } else if (lookahead == FLOAT) {
        match(FLOAT);
        *type = FLOAT_TYPE;
    } else {
        syntax_error("INT or FLOAT");
    }
}

void statement_list() {
    while (lookahead == ID || lookahead == '{' || lookahead == IF || lookahead == WHILE) {
        statement();
    }
}

void statement() {
    if (lookahead == ID) {
        assignment_stmt();
        match(';'); // Match ';' after assignment
    } else if (lookahead == '{') {
        compound_stmt();
    } else if (lookahead == IF) {
        selection_stmt();
    } else if (lookahead == WHILE) {
        iteration_stmt();
    } else {
        syntax_error("statement");
    }
}

void assignment_stmt() {
    char name[100];
    VarType var_type;
    var(name, &var_type);
    if (lookahead == ASSIGNOP) {
        match(ASSIGNOP);
        VarType expr_type;
        union {
            int int_val;
            float float_val;
        } value;
        expression(&expr_type, &value);
        if (var_type != expr_type) {
            semantic_error("Type mismatch in assignment");
        } else {
            set_symbol_value(name, expr_type, &value);
        }
    } else {
        syntax_error("'='");
    }
}

void var(char *name, VarType *type) {
    if (lookahead == ID) {
        strcpy(name, yytext);
        Symbol *sym = lookup_symbol(yytext);
        if (sym == NULL) {
            semantic_error("Undeclared variable");
        } else {
            *type = sym->type;
        }
        match(ID);
        if (lookahead == '[') {
            match('[');
            VarType expr_type;
            int index;
            expression(&expr_type, &index);
            match(']');
            // Array access can be handled here
        }
    } else {
        syntax_error("ID");
    }
}

void expression(VarType *type, void *value) {
    VarType left_type;
    union {
        int int_val;
        float float_val;
    } left_value;
    additive_expression(&left_type, &left_value);
    if (lookahead == RELOP) {
        relop();
        VarType right_type;
        union {
            int int_val;
            float float_val;
        } right_value;
        additive_expression(&right_type, &right_value);
        // Handle relational operation result (e.g., set value to 0 or 1)
        *type = INT_TYPE;
        *((int *)value) = 0; // Placeholder
    } else {
        *type = left_type;
        if (*type == INT_TYPE) {
            *((int *)value) = left_value.int_val;
        } else {
            *((float *)value) = left_value.float_val;
        }
    }
}

void relop() {
    if (lookahead == RELOP) {
        match(RELOP);
    } else {
        syntax_error("relational operator");
    }
}

void additive_expression(VarType *type, void *value) {
    VarType left_type;
    union {
        int int_val;
        float float_val;
    } left_value;
    term(&left_type, &left_value);
    while (lookahead == ADDOP) {
        int op = yytext[0];
        match(ADDOP);
        VarType right_type;
        union {
            int int_val;
            float float_val;
        } right_value;
        term(&right_type, &right_value);
        if (left_type != right_type) {
            semantic_error("Type mismatch in additive expression");
        }
        if (left_type == INT_TYPE) {
            if (op == '+') {
                left_value.int_val += right_value.int_val;
            } else {
                left_value.int_val -= right_value.int_val;
            }
        } else {
            if (op == '+') {
                left_value.float_val += right_value.float_val;
            } else {
                left_value.float_val -= right_value.float_val;
            }
        }
    }
    *type = left_type;
    if (*type == INT_TYPE) {
        *((int *)value) = left_value.int_val;
    } else {
        *((float *)value) = left_value.float_val;
    }
}

void term(VarType *type, void *value) {
    VarType left_type;
    union {
        int int_val;
        float float_val;
    } left_value;
    factor(&left_type, &left_value);
    while (lookahead == MULOP) {
        int op = yytext[0];
        match(MULOP);
        VarType right_type;
        union {
            int int_val;
            float float_val;
        } right_value;
        factor(&right_type, &right_value);
        if (left_type != right_type) {
            semantic_error("Type mismatch in term");
        }
        if (left_type == INT_TYPE) {
            if (op == '*') {
                left_value.int_val *= right_value.int_val;
            } else {
                left_value.int_val /= right_value.int_val;
            }
        } else {
            if (op == '*') {
                left_value.float_val *= right_value.float_val;
            } else {
                left_value.float_val /= right_value.float_val;
            }
        }
    }
    *type = left_type;
    if (*type == INT_TYPE) {
        *((int *)value) = left_value.int_val;
    } else {
        *((float *)value) = left_value.float_val;
    }
}

void factor(VarType *type, void *value) {
    if (lookahead == '(') {
        match('(');
        expression(type, value);
        match(')');
    } else if (lookahead == ID) {
        char name[100];
        var(name, type);
        if (!get_symbol_value(name, type, value)) {
            semantic_error("Variable has no value");
        }
    } else if (lookahead == NUM) {
        if (strchr(yytext, '.')) {
            *type = FLOAT_TYPE;
            *((float *)value) = atof(yytext);
        } else {
            *type = INT_TYPE;
            *((int *)value) = atoi(yytext);
        }
        match(NUM);
    } else {
        syntax_error("factor");
    }
}

void compound_stmt() {
    match('{');
    statement_list();
    match('}');
}

void selection_stmt() {
    match(IF);
    match('(');
    VarType expr_type;
    int condition;
    expression(&expr_type, &condition);
    if (expr_type != INT_TYPE) {
        semantic_error("Condition must be of type int");
    }
    match(')');
    statement();
    if (lookahead == ELSE) {
        match(ELSE);
        statement();
    }
}

void iteration_stmt() {
    match(WHILE);
    match('(');

    // Variables to hold the condition type and value
    VarType expr_type;
    int condition;

    // Save the current position in the input stream
    int condition_start_lookahead = lookahead;
    char condition_start_yytext[100];
    strcpy(condition_start_yytext, yytext);

    // Parse and evaluate the condition
    expression(&expr_type, &condition);
    if (expr_type != INT_TYPE) {
        semantic_error("Condition must be of type int");
    }
    match(')');

    // Save the position of the loop body
    int body_start_lookahead = lookahead;
    char body_start_yytext[100];
    strcpy(body_start_yytext, yytext);

    // Parse the loop body once to ensure it's syntactically correct
    statement();

    // Now, execute the loop
    while (condition) {
        // Execute the loop body
        // We need to reset the input stream to the start of the loop body

        // Save the current lexer state
        int saved_lookahead = lookahead;
        char saved_yytext[100];
        strcpy(saved_yytext, yytext);

        // Restore the lexer state to the start of the loop body
        lookahead = body_start_lookahead;
        strcpy(yytext, body_start_yytext);

        // Execute the loop body
        statement();

        // Restore the lexer state to after the loop body
        lookahead = saved_lookahead;
        strcpy(yytext, saved_yytext);

        // Re-evaluate the condition
        // Restore the lexer state to the start of the condition
        lookahead = condition_start_lookahead;
        strcpy(yytext, condition_start_yytext);

        expression(&expr_type, &condition);
        if (expr_type != INT_TYPE) {
            semantic_error("Condition must be of type int");
        }
    }

}


void match(int expectedToken) {
    if (lookahead == expectedToken) {
        lookahead = yylex();
    } else {
        char expected[50];
        getTokenName(expectedToken, expected);
        syntax_error(expected);
    }
}

void syntax_error(const char *expected) {
    fprintf(stderr, "Syntax error at line %d, pos %d: expected %s but found '%s'\n",
            line_num, char_pos, expected, yytext);
    exit(1);
}

void getTokenName(int token, char *name) {
    switch (token) {
        case PROGRAM: strcpy(name, "PROGRAM"); break;
        case ID: strcpy(name, "ID"); break;
        case NUM: strcpy(name, "NUM"); break;
        case INT: strcpy(name, "INT"); break;
        case FLOAT: strcpy(name, "FLOAT"); break;
        case IF: strcpy(name, "IF"); break;
        case ELSE: strcpy(name, "ELSE"); break;
        case WHILE: strcpy(name, "WHILE"); break;
        case RELOP: strcpy(name, "relational operator"); break;
        case ADDOP: strcpy(name, "'+' or '-'"); break;
        case MULOP: strcpy(name, "'*' or '/'"); break;
        case ASSIGNOP: strcpy(name, "'='"); break;
        default:
            if (isprint(token)) {
                sprintf(name, "'%c'", token);
            } else {
                sprintf(name, "token %d", token);
            }
            break;
    }
}

void semantic_error(const char *message) {
    fprintf(stderr, "Semantic error at line %d: %s\n", line_num, message);
    exit(1);
}
