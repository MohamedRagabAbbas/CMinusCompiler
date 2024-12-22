#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef enum { INT_TYPE, FLOAT_TYPE } VarType;

typedef struct {
    char *name;
    VarType type;
    union {
        int int_val;
        float float_val;
    } value;
    int decl_line;
} Symbol;

void init_symbol_table();
void free_symbol_table();
bool insert_symbol(const char *name, VarType type, int decl_line);
Symbol *lookup_symbol(const char *name);
void print_symbol_table();
bool set_symbol_value(const char *name, VarType type, void *value);
bool get_symbol_value(const char *name, VarType *type, void *value);

#endif
