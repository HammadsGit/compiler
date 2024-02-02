# Custom Compiler Project

## Overview
This project is a custom compiler designed to parse, analyze, and compile a specific programming language into machine code or an intermediate representation. It comprises several key components: a lexer (`lexer.c`), a parser (`parser.c`), symbol management (`symbols.c` and `symbols.h`), and the main compiler logic (`compiler.c` and `compiler.h`).

## Features
- **Lexical Analysis**: Tokenizes input source code into meaningful symbols.
- **Syntax Analysis**: Parses tokens to construct an abstract syntax tree (AST) representing the program's structure.
- **Symbol Management**: Manages a symbol table to track variables, types, and functions.
- **Compilation**: Transforms AST into target code or an intermediate representation.

## Getting Started
1. **Prerequisites**: Ensure you have GCC (or any C compiler) and Make installed on your system.
2. **Compilation**: Run `make` in the project directory to compile the compiler.
3. **Execution**: Use `./compiler [options] <source file>` to compile your source code.

## Usage
- Input your source code in a file.
- Run the compiler specifying the source file as an argument.
- The compiler will output the compiled code or any syntax errors encountered during compilation.

## Development
This project was developed with extensibility in mind, allowing for further enhancements such as adding new syntax rules, optimizing the generated code, or supporting additional target platforms.

## Contributions
Contributions are welcome! Whether you're fixing bugs, adding new features, or improving documentation, please feel free to make a pull request.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- Special thanks to everyone who contributed to the development and testing of this compiler.
- Inspired by the principles of compiler design and the desire to understand the intricacies of language processing.

For more detailed information about each component and how to extend the compiler, please refer to the source code documentation.
