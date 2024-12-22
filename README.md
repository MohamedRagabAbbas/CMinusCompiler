
# C- Compiler

## Project Overview
This project is a compiler for a C-like language, built from scratch using two approaches: a traditional parser and a YACC-based parser. It includes components for lexical analysis, parsing, semantic analysis, error handling, and symbol table management. The compiler aims to provide educational insights into compiler construction and the differences between manual and automated parsing techniques.

## Features
- Lexical Analysis using Flex
- Syntax Analysis with two methods:
  - Manual Parsing
  - YACC Parsing
- Semantic Analysis
- Error Handling
- Symbol Table Management

## Getting Started
To run this compiler, clone the repository and compile the source files using a C compiler that supports YACC (like GNU Bison).

```bash
git clone https://github.com/your-username/CMinusCompiler.git
cd CMinusCompiler
make
./compiler your_program.cm
```

## Contributing
Contributions to the project are welcome! Please fork the repository, make your changes, and submit a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
