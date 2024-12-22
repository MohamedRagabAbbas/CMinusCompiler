# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99

# Object files and final executable
OBJ = parser.o lexer.o
EXEC = parser_program

# Default target
all: $(EXEC)

# Rule to link object files into the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to compile parser.c into parser.o
parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

# Rule to compile lexer.c into lexer.o
lexer.o: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)
