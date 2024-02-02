#ifndef SYMBOLS_H
#define SYMBOLS_H


#include "parser.h"
#include "lexer.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 2000



// enum for the kind of the token. e.g. "var". kind of the variable.
typedef enum { VAR, ARG, STATIC, FIELD, SUBROUTINE} KindType;


typedef enum {CHAR, BOOLEAN, VOID} TokenTypes;

typedef struct {
    char lexeme[128]; // the lexeme of the token. e.g. "x". name of the variable.
    char type; // Is it a variable name, function name or class name.
    KindType kind; // the kind of the token. e.g. "var". kind of the variable.
    int index; // the index of the token. e.g. 0. index of the variable.
    int scope_level; // the scope level of the token. e.g. 0. scope level of the variable.
    char function_name[128]; // the name of the function that the symbol belongs to.
    char class_name[128]; // the name of the class that the symbol belongs to.

} Symbol;

// the SymbolTable struct represents a symbol table.
typedef struct {
    Symbol symbols[MAX_SIZE]; // the array of symbols.
    int count; // the size of the array.
    int current_scope_level; // the current scope level of the symbol table. 0 is global 1 is local.
    char current_function_name[128]; // the name of the current function of the symbol table.
    char current_class_name[128]; // the name of the current class of the symbol table.
} SymbolTable;

// Declare the global symbol table
static SymbolTable symbol_table;

void init_symbol_table();
void add_symbol(char* lexeme, TokenType type, KindType kind);
Symbol* get_symbol(char* lexeme);
Symbol* get_subroutine(char* lexeme);
void clear_symbol_table();
int get_current_pass();
void append_current_pass();
void enter_scope();
void exit_scope();
void set_current_function_name(char* name);
void set_current_class_name(char* name);
char * get_current_class_name();
Symbol* get_class(char* lexeme);

extern int current_pass;







#endif
