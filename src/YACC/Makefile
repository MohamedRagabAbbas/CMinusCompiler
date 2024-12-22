# Compiler and Tools
CC = gcc
LEX = flex
YACC = yacc

# Compiler Flags
CFLAGS = -Wall -g

# YACC Flags
YACCFLAGS = -d

# Target Executable
TARGET = parser

# Source Files
YACC_SRC = parser.y
FLEX_SRC = lexer.l
HELPER_SRC = parser.c  # Ensure parser.c does NOT define yyparse, yyerror, or main

# Generated Files
YACC_OUT_C = y.tab.c
YACC_OUT_H = y.tab.h
FLEX_OUT_C = lex.yy.c

# Libraries
LIBS = -lfl  # Use -lfl instead of -ll

# Default Target
all: $(TARGET)

# Build the Parser Executable
$(TARGET): $(YACC_OUT_C) $(FLEX_OUT_C) $(HELPER_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(YACC_OUT_C) $(FLEX_OUT_C) $(HELPER_SRC) $(LIBS)

# Generate YACC Outputs
$(YACC_OUT_C) $(YACC_OUT_H): $(YACC_SRC)
	$(YACC) $(YACCFLAGS) $(YACC_SRC)

# Generate FLEX Output
$(FLEX_OUT_C): $(FLEX_SRC) $(YACC_OUT_H)
	$(LEX) $(FLEX_SRC)

# Clean Build Artifacts
clean:
	rm -f $(TARGET) $(YACC_OUT_C) $(YACC_OUT_H) $(FLEX_OUT_C) y.output

# Phony Targets
.PHONY: all clean
