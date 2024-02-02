
/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Hammad Shahid
Student ID: 201442557
Email: ed19h4s@leeds.ac.uk
Date Work Commenced: 10/04/2023
*************************************************************************/

#include "symbols.h"


// define your own types and function prototypes for the symbol table(s) module below

// the SymbolTable struct represents a symbol table.


int static_index = 0;
int field_index = 0;
int arg_index = 0;
int var_index = 0;

int current_pass = 2;

// Initialize symbol table
void init_symbol_table() {
    symbol_table.count = 0;
    symbol_table.current_scope_level = 0;
    symbol_table.current_function_name[0] = '\0';
    symbol_table.current_class_name[0] = '\0';
}


// Add symbol to symbol table
void add_symbol(char* lexeme, TokenType type, KindType kind) {
    Symbol symbol;
    strcpy(symbol.lexeme, lexeme);
    symbol.type = type;
    symbol.kind = kind;

    symbol.index = -1;
    symbol.scope_level = symbol_table.current_scope_level;
    strcpy(symbol.function_name, symbol_table.current_function_name);
    strcpy(symbol.class_name, symbol_table.current_class_name);



    if (kind == STATIC) {
        symbol.index = static_index;
        static_index++;
    }
    else if (kind == FIELD) {
        symbol.index = field_index;
        field_index++;
    }
    else if (kind == ARG) {
        symbol.index = arg_index;
        arg_index++;
    }
    else if (kind == VAR) {
        symbol.index = var_index;
        var_index++;
    }

    symbol_table.symbols[symbol_table.count] = symbol;
    symbol_table.count++;

}

// Get the symbol with the given lexeme
Symbol* get_symbol(char* lexeme) {
    for (int i = symbol_table.count - 1; i >= 0; i--) {
        if (strcmp(symbol_table.symbols[i].lexeme, lexeme) == 0 &&
            strcmp(symbol_table.symbols[i].function_name, symbol_table.current_function_name) == 0 &&
            strcmp(symbol_table.symbols[i].class_name, symbol_table.current_class_name) == 0 &&
            symbol_table.symbols[i].scope_level <= symbol_table.current_scope_level) {
            return &symbol_table.symbols[i];
        }
    }
    return NULL;
}

Symbol* get_subroutine(char* lexeme){
    for (int i = symbol_table.count - 1; i >= 0; i--){

        if (strcmp(symbol_table.symbols[i].lexeme, lexeme) == 0)
        {
            return &symbol_table.symbols[i];
        }
    }

    return NULL;
}

Symbol* get_class(char* lexeme){
    for (int i = symbol_table.count - 1; i >= 0; i--){

        if (strcmp(symbol_table.symbols[i].lexeme, lexeme) == 0 && symbol_table.symbols[i].kind == STATIC)
        {
            return &symbol_table.symbols[i];
        }
    }

    return NULL;
}


void clear_symbol_table() {
    symbol_table.count = 0;
    static_index = 0;
    field_index = 0;
    arg_index = 0;
    var_index = 0;
}

void set_current_pass(){
    current_pass = 0;
}

int get_current_pass(){
    return current_pass;
}

void append_current_pass(){
    current_pass++;
}

void enter_scope() {
    symbol_table.current_scope_level++;
}

void exit_scope() {
    for (int i = symbol_table.count - 1; i >= 0; i--) {
        if (symbol_table.symbols[i].scope_level == symbol_table.current_scope_level &&
            strcmp(symbol_table.symbols[i].function_name, symbol_table.current_function_name) == 0 &&
            strcmp(symbol_table.symbols[i].class_name, symbol_table.current_class_name) == 0) {
            symbol_table.count--;
            if (symbol_table.symbols[i].kind == STATIC) {
                static_index--;
            }
            else if (symbol_table.symbols[i].kind == FIELD) {
                field_index--;
            }
            else if (symbol_table.symbols[i].kind == ARG) {
                arg_index--;
            }
            else if (symbol_table.symbols[i].kind == VAR) {
                var_index--;
            }
        }
    }
    symbol_table.current_scope_level--;
}

void set_current_function_name(char* name) {
    strcpy(symbol_table.current_function_name, name);
}

void set_current_class_name(char* name){
    strcpy(symbol_table.current_class_name, name);
}

char * get_current_class_name(){
    return symbol_table.current_class_name;
}








