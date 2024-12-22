#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_TABLE_SIZE 100

static Symbol *symbol_table[SYMBOL_TABLE_SIZE];
static int symbol_count = 0;

void init_symbol_table() {
    symbol_count = 0;
}

void free_symbol_table() {
    for (int i = 0; i < symbol_count; i++) {
        free(symbol_table[i]->name);
        free(symbol_table[i]);
    }
}

bool insert_symbol(const char *name, VarType type, int decl_line) {
    if (lookup_symbol(name) != NULL) {
        return false; // Symbol already exists
    }
    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->decl_line = decl_line;
    symbol_table[symbol_count++] = sym;
    return true;
}

Symbol *lookup_symbol(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i]->name, name) == 0) {
            return symbol_table[i];
        }
    }
    return NULL;
}

void print_symbol_table() {
    printf("Symbol Table:\n");
    printf("Name\tType\tValue\tDecl Line\n");
    for (int i = 0; i < symbol_count; i++) {
        Symbol *sym = symbol_table[i];
        printf("%s\t%s\t", sym->name, sym->type == INT_TYPE ? "int" : "float");
        if (sym->type == INT_TYPE) {
            printf("%d\t", sym->value.int_val);
        } else {
            printf("%f\t", sym->value.float_val);
        }
        printf("%d\n", sym->decl_line);
    }
}

bool set_symbol_value(const char *name, VarType type, void *value) {
    Symbol *sym = lookup_symbol(name);
    if (sym == NULL || sym->type != type) {
        return false;
    }
    if (type == INT_TYPE) {
        sym->value.int_val = *(int *)value;
    } else {
        sym->value.float_val = *(float *)value;
    }
    return true;
}

bool get_symbol_value(const char *name, VarType *type, void *value) {
    Symbol *sym = lookup_symbol(name);
    if (sym == NULL) {
        return false;
    }
    *type = sym->type;
    if (sym->type == INT_TYPE) {
        *(int *)value = sym->value.int_val;
    } else {
        *(float *)value = sym->value.float_val;
    }
    return true;
}
