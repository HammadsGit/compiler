#include <stdlib.h>
#include <string.h>
#include <stdio.h>



#include "lexer.h"
#include "parser.h"
#include "symbols.h"



// #include "lexer.c"

// you can declare prototypes of parser functions below
ParserInfo classDeclar (ParserInfo pi);
ParserInfo memberDeclar (ParserInfo pi);
ParserInfo classVarDeclar (ParserInfo pi);
ParserInfo type (ParserInfo pi);
ParserInfo subroutineDeclar (ParserInfo pi);
ParserInfo paramList (ParserInfo pi);
ParserInfo subroutineBody (ParserInfo pi);
ParserInfo statement (ParserInfo pi);
ParserInfo varDeclarStatement (ParserInfo pi);
ParserInfo letStatement (ParserInfo pi);
ParserInfo ifStatement (ParserInfo pi);
ParserInfo whileStatement (ParserInfo pi);
ParserInfo doStatement (ParserInfo pi);
ParserInfo subroutineCall (ParserInfo pi);
ParserInfo expressionList (ParserInfo pi);
ParserInfo returnStatement (ParserInfo pi);
ParserInfo expression (ParserInfo pi);
ParserInfo relationalExpression (ParserInfo pi);
ParserInfo ArithmeticExpression (ParserInfo pi);
ParserInfo term(ParserInfo pi);
ParserInfo factor (ParserInfo pi);
ParserInfo operand (ParserInfo pi);



int ParanthesisCount = 0;

char temp_type;
char temp_lexeme[128];

int pass = 0;



int InitParser(char *file_name)
{

//    printf("%s", file_name);


    InitLexer(file_name);




    return 1;
}
ParserInfo type(ParserInfo pi)
{

    pi.tk = PeekNextToken(); // peak at the next token.

    if(pi.tk.tp == RESWORD){ // if it is a reserved word.
        if(strcmp(pi.tk.lx, "int") == 0){
            pi.tk = GetNextToken(); // get the next token.
        }
        else if(strcmp(pi.tk.lx, "char") == 0){
            pi.tk = GetNextToken(); // get the next token.
        }
        else if(strcmp(pi.tk.lx, "boolean") == 0){
            pi.tk = GetNextToken(); // get the next token.
        }
        else{
            pi.er = illegalType;
            return pi;
        }
    }
    else if(pi.tk.tp == ID){ // if it is an ID.

        pi.tk = GetNextToken(); // get the next token.

        pass = get_current_pass();

        if(pass % 2 == 0){
            Symbol* symbol_x = get_symbol(pi.tk.lx);
            Symbol* symbol_y = get_class(pi.tk.lx);



            if(symbol_x == NULL && symbol_y == NULL){
                pi.er = undecIdentifier;
                return pi;
            }
        }

    }

    else{
        pi.er = illegalType;
        return pi;
    }


    return pi;
}

ParserInfo paramlist(ParserInfo pi)
{
    pi = type(pi); // get the type.

    temp_type = pi.tk.tp;


    if(pi.er == none) { // if there is no error and a valid type is there. then get the next token.

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

        if (pi.tk.tp == ID) { // if it is an ID.

            pi.tk = GetNextToken(); // get the next token.

            strcpy(temp_lexeme, pi.tk.lx);

            //adding to the symbole table.
            pass = get_current_pass();

            if(pass % 2 != 0){
                add_symbol(temp_lexeme, temp_type, VAR);
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a comma.

            while (pi.tk.tp == SYMBOL && pi.tk.lx[0] == ',') { // if it is a comma.
                pi.tk = GetNextToken(); // get the next token.

                pi = type(pi); // get the type.

                temp_type = pi.tk.tp;


                if (pi.er == none) { // if there is no error and a valid type is there. then get the next token.

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

                    if (pi.tk.tp == ID) { // if it is an ID.

                        pi.tk = GetNextToken(); // get the next token.

                        strcpy(temp_lexeme, pi.tk.lx);

                        //adding to the symbole table.
                        if(pass % 2 != 0){
                            add_symbol(temp_lexeme, temp_type, VAR);
                        }

                        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a comma.

                    } else {
                        pi.er = idExpected;
                        return pi;
                    }
                }
                else {
                    return pi;
                }


            }
        }
    }
    else{
        pi.er = none; // it's empty so set the error to none.
        return pi;
    }







    return pi;
}

ParserInfo varDeclarStatement(ParserInfo pi)
{

    pi.tk = GetNextToken(); // get the next token, which is var.

    pi = type(pi); // get the type.



    temp_type = pi.tk.tp;

    if(pi.er == none) { // if there is no error and a valid type is there. then get the next token.

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

        if (pi.tk.tp == ID) { // if it is an ID.

            pi.tk = GetNextToken(); // get the next token.

            strcpy(temp_lexeme, pi.tk.lx);

            if(pass % 2 != 0){
                add_symbol(temp_lexeme, temp_type, VAR);
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a comma.

            while (pi.tk.tp == SYMBOL && pi.tk.lx[0] == ',') { // if it is a comma.
                pi.tk = GetNextToken(); // get the next token.

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

                if (pi.tk.tp == ID) { // if it is an ID.


                    pi.tk = GetNextToken(); // get the next token.


                    strcpy(temp_lexeme, pi.tk.lx);


                    if(pass % 2 != 0){
                        add_symbol(temp_lexeme, temp_type, VAR);
                    }

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a comma.

                } else {
                    pi.er = idExpected;
                    return pi;
                }
            }

            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){ // if it is a semicolon.
                pi.tk = GetNextToken(); // get the next token.
            }
            else{
                pi.er = semicolonExpected;
                return pi;
            }
        }
        else{
            pi.er = idExpected;
            return pi;
        }
    }
    else{
        return pi;
    }



    return pi;
}


ParserInfo term(ParserInfo pi)
{

    pi = factor(pi); // get the factor.

    if(pi.er  == none){

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        while(pi.tk.tp == SYMBOL && (pi.tk.lx[0] == '*' || pi.tk.lx[0] == '/')){ // if it is a symbol, * or /.
            pi.tk = GetNextToken(); // get the next token.

            pi = factor(pi); // get the factor.

            if(pi.er != none){ // if there is an error, return it.
                return pi;
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        }

        }


    return pi;
}

ParserInfo ArithmeticExpression(ParserInfo pi)
{

    pi = term(pi); // get the term.

    if(pi.er == none){
        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        while(pi.tk.tp == SYMBOL && (pi.tk.lx[0] == '+' || pi.tk.lx[0] == '-')){ // if it is a symbol, + or -.
            pi.tk = GetNextToken(); // get the next token.

            pi = term(pi); // get the term.

            if(pi.er != none){ // if there is an error, return it.
                return pi;
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        }

        }


    return pi;
}


ParserInfo relationalExpression (ParserInfo pi){

        pi = ArithmeticExpression(pi); // get the arithmetic expression.

        if(pi.er == none){ // if there is no error, then get the next token.

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

            while(pi.tk.tp == SYMBOL && (pi.tk.lx[0] == '<'  || pi.tk.lx[0] == '>' || pi.tk.lx[0] == '=')){ // if it is a symbol, <, >, or =.
                pi.tk = GetNextToken(); // get the next token.

                pi = ArithmeticExpression(pi); // get the arithmetic expression.

                if(pi.er != none){ // if there is an error, return it.
                    return pi;
                }

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.
            }
        }
        else{
            return pi;
        }

        return pi;
}


ParserInfo expression(ParserInfo pi)
{

    pi = relationalExpression(pi); // get the relational expression.

    if(pi.er == none){ // if there is no error and a valid type is there. then get the next token.

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        while(pi.tk.tp == SYMBOL && (pi.tk.lx[0] == '&'  || pi.tk.lx[0] == '|')){ // if it is a symbol, & or |.
            pi.tk = GetNextToken(); // get the next token.

            pi = relationalExpression(pi); // get the relational expression.

            if(pi.er != none){ // if there is an error, return it.
                return pi;
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.
        }
    }
    else{
        return pi;
    }



    return pi;
}

ParserInfo letStatement(ParserInfo pi){
    pi.tk = GetNextToken(); // get the next token, which is let.

    pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

    if(pi.tk.tp == ID){
        pi.tk = GetNextToken(); // get the next token. which is the identifier.

        pass = get_current_pass();


        if(pass % 2 == 0){ ///////////////////////// if it's an even pass, then check if the identifier is in the symbol table.
            Symbol* symbol_x = get_symbol(pi.tk.lx);

            Symbol* symbol_y = get_subroutine(pi.tk.lx);


            if(symbol_x == NULL && symbol_y == NULL){
                pi.er = undecIdentifier;
                return pi;
            }
        }


        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol, [ or =.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '['){
            pi.tk = GetNextToken(); // get the next token. which is the symbol.

            pi = expression(pi); // get the expression.

            if(pi.er == none){ // if there is no error getting the expression.
                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ']'){
                    pi.tk = GetNextToken(); // get the next token. which is the symbol.

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                    if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '='){
                        pi.tk = GetNextToken(); // get the next token. which is the symbol.

                        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                        pi = expression(pi); // get the expression.

//                        Symbol* symbol_x = get_symbol(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.
//
//                        if(symbol_x->type != pi.tk.tp || symbol_x == NULL || symbol_x->kind != VAR){
//                            pi.er = undecIdentifier;
//                            return pi;
//                        }

                        if(pi.er == none){
                            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
                                pi.tk = GetNextToken(); // get the next token. which is the symbol.
                            }
                            else{
                                pi.er = semicolonExpected;
                                return pi;
                            }
                        }
                        else{
                            return pi;
                        }
                    }
                    else{
                        pi.er = equalExpected;
                        return pi;
                    }
                }
                else{
                    pi.er = closeBracketExpected; // ] expected.
                    return pi;
                }
            }
            else{
                return pi; // return the error from expression.
             }
        }
        else if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '='){
            pi.tk = GetNextToken(); // get the next token. which is the symbol.

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.


            pi = expression(pi);


            if(pi.er == none){ // if there is no error getting the expression.
//                pi.tk = GetNextToken(); // get the next token. which is the symbol.

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
                    pi.tk = GetNextToken(); // get the next token. which is the symbol.
                }
                else{
                    pi.er = semicolonExpected;
                    return pi;
                }
            }
            else{ // if expression returns an error then return it.
                return pi;
            }
        }
        else{
            pi.er = equalExpected;
            return pi;
        }




    }
    else{
        pi.er = idExpected;
        return pi;
    }



    return pi;
}




ParserInfo ifStatement(ParserInfo pi){
    pi.tk = GetNextToken(); // get the next token, which is if.

    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

    if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){
        pi.tk = GetNextToken(); // get the next token, which is the symbol.

        pi = expression(pi); // get the expression.

        if(pi.er == none){ // if there is no error getting the expression.
            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')') {
                pi.tk = GetNextToken(); // get the next token, which is the symbol. ) expected.

                pi = subroutineBody(pi); // get the subroutine body.



                if(pi.er == none){ // Subroutine body has no error.

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a resword.

                    while(pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "else") == 0){ // if there is an else statement.
                        pi.tk = GetNextToken(); // get the next token, which is else.

                        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                        pi = subroutineBody(pi); // get the subroutine body.

                        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a resword.
                    }


                }










            }
            else{
                pi.er = closeParenExpected;
                return pi;
            }

        }
        else{ // if expression returns an error then return it.
            return pi;
        }
    }
    else{
        pi.er = openParenExpected;
        return pi;
    }


    return pi;
}

ParserInfo whileStatement(ParserInfo pi)
{



    pi.tk = GetNextToken(); // get the next token, which is while.

    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

    if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){

        pi.tk = GetNextToken(); // get the next token, which is the symbol. ( expected.


        pi = expression(pi); // get the expression.

        if(pi.er == none){
            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                pi.tk = GetNextToken(); // get the next token, which is the symbol. ) expected.

                pi = subroutineBody(pi); // get the subroutine body.


            }
            else{
                pi.er = closeParenExpected;
                return pi;
            }
        }
    }
    else{
        pi.er = openParenExpected;
        return pi;
    }


    return pi;
}


ParserInfo subroutineCall(ParserInfo pi)
{
    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

    if(pi.tk.tp == ID){
        pi.tk = GetNextToken(); // get the next token, which is the identifier.

        /////////////////////////// check if the identifier is a subroutine name in th symbol table.
        pass = get_current_pass();

        if(pass % 2 == 0){ ///////////////////////// if it's an even pass, then check if the identifier is in the symbol table.
            Symbol* symbol_x = get_symbol(pi.tk.lx);

            Symbol* symbol_y = get_subroutine(pi.tk.lx);


            if(symbol_x == NULL && symbol_y == NULL){
                pi.er = undecIdentifier;
                return pi;
            }
        }



        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '.'){
            pi.tk = GetNextToken(); // get the next token, which is the symbol.

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's an identifier.

            if(pi.tk.tp == ID){
                pi.tk = GetNextToken (); // get the next token, which is the identifier.

                pass = get_current_pass();

                if(pass % 2 ==0){
                    Symbol* symbol_x = get_subroutine(pi.tk.lx);

                    Symbol* symbol_y = get_symbol(pi.tk.lx);

                    if(symbol_x == NULL && symbol_y == NULL){
                        pi.er = undecIdentifier;
                        return pi;
                    }
                }

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){
                    pi.tk = GetNextToken(); // get the next token, which is the symbol.

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                    if(pi.tk.lx[0] != ')'){
                        pi = expressionList(pi); // get the expression list.
                    }

                    if(pi.er == none){ // if there is no error getting the expression list.

                        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                            pi.tk = GetNextToken(); // get the next token, which is the symbol.
                        }
                        else{
                            pi.er = closeParenExpected;
                            return pi;
                        }
                    }
                    else{
                        return pi;
                    }
                }
                else{
                    pi.er = openParenExpected;
                    return pi;
                }


            }

            else{
                pi.er = idExpected;
                return pi;
            }


        }
        else{
            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){
                pi.tk = GetNextToken(); // get the next token, which is the symbol.

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                if(pi.tk.lx[0] != ')'){
                    pi = expressionList(pi); // get the expression list.
                }



                if(pi.er == none){ // if there is no error getting the expression list.

                    pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

                    if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                        pi.tk = GetNextToken(); // get the next token, which is the symbol.
                    }
                    else{
                        pi.er = closeParenExpected;
                        return pi;
                    }
                }
                else{
                    return pi;
                }
            }
            else{
                pi.er = openParenExpected;
                return pi;
            }
        }
    }


    else{
        pi.er = idExpected;
        return pi;
    }

    return pi;
}


ParserInfo doStatement(ParserInfo pi)
{

    pi.tk = GetNextToken(); // get the next token, which is do.

    pi = subroutineCall(pi); // get the subroutine call.

    if(pi.er == none){
        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
            pi.tk = GetNextToken(); // get the next token, which is the symbol.
        }
        else{
            pi.er = semicolonExpected;
            return pi;
        }
    }
    else{
        return pi; // if there is an error in the subroutine call, return it.
    }



    return pi;
}

ParserInfo returnStatement(ParserInfo pi)
{

    pi.tk = GetNextToken(); // get the next token, which is return.

    pi = expression(pi); // get the expression.

    if(pi.er == none){
        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
            pi.tk = GetNextToken(); // get the next token, which is the symbol.
        }
        else{
            pi.er = semicolonExpected;
            return pi;
        }
    }
    else{
        // if there is no expression return with no error.
        pi.er = none;

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
            pi.tk = GetNextToken(); // get the next token, which is the symbol.
        }
        else{
            pi.er = semicolonExpected;
            return pi;
        }
        return pi; // if there is an error in the expression, return it.
    }

    return pi;
}


ParserInfo Statement(ParserInfo pi)
{

//    pi.tk = PeekNextToken(); // peak at the next token, to see what type of statement it is.

    if(pi.tk.tp == RESWORD){
        if(strcmp(pi.tk.lx, "var") == 0){

            pi = varDeclarStatement(pi); // get the varDec.


        }
        else if(strcmp(pi.tk.lx, "let") == 0){

            pi = letStatement(pi); // get the let statement.

        }
        else if(strcmp(pi.tk.lx, "do") == 0){

            pi = doStatement(pi); // get the do statement.

        }
        else if(strcmp(pi.tk.lx, "if") == 0){

            pi = ifStatement(pi); // get the if statement.

        }
        else if(strcmp(pi.tk.lx, "while") == 0){

            pi = whileStatement(pi); // get the while statement.

        }
        else if(strcmp(pi.tk.lx, "return") == 0){

            pi = returnStatement(pi); // get the return statement.

        }
        else{
            pi.er = illegalType;
            return pi;
        }
    }


    return pi;
}



ParserInfo expressionList(ParserInfo pi)
{

    pi.tk = PeekNextToken(); // peak at the next token, to see if it's an expression.

    if(strcmp(pi.tk.lx, "") == 0){ // if it's an empty string, then there is no expression list.
        return pi;
    }
    else{ // if it's not an empty string, then there is an expression list.

        pi = expression(pi); // get the expression.

        if(pi.er == none){// if there is no error in the expression.

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.

            while(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ','){
                pi.tk = GetNextToken(); // get the next token, which is the symbol.

                pi = expression(pi); // get the expression.

                if(pi.er != none){ // if there is an error in the expression.
                    return pi;
                }

                pi.tk = PeekNextToken(); // peak at the next token, to see if it's a symbol.
            }
        }
    }



    return pi;
}





ParserInfo subroutineBody(ParserInfo pi){

    pi.tk = PeekNextToken(); // peak at the next token, to see if it's an open brace.

    if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '{') {
        pi.tk = GetNextToken(); // get the next token, which is an open brace.
        ParanthesisCount += 1;

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a statement.

        //make a while loop to get all the statements.
        while(pi.tk.tp == RESWORD && (pi.tk.lx[0] == 'v' || pi.tk.lx[0] == 'l' || pi.tk.lx[0] == 'd'
        || pi.tk.lx[0] == 'i' || pi.tk.lx[0] == 'w' || pi.tk.lx[0] == 'r')){
            pi = Statement(pi); // get the statement.

            if(pi.er != none){ // if there is an error getting the statement.
                return pi;
            }

            pi.tk = PeekNextToken(); // peak at the next token, to see if it's a statement.
        }

        pi.tk = PeekNextToken(); // peak at the next token, to see if it's a close brace.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '}'){
            ParanthesisCount -= 1;
            pi.tk = GetNextToken(); // get the next token, which is a close brace.
        }

        else{
            pi.er = closeBraceExpected;
            return pi;
        }

    }

    else{
        pi.er = openBraceExpected;
        return pi;
    }


    return pi;
}


ParserInfo subroutineDeclar(ParserInfo pi){
    //    add_symbol("x", INT, VAR);

    pi.tk = GetNextToken();
    // if it is a subroutine declaration then get the next token,
    // which is (constructor, function, or method).

    pi.tk = PeekNextToken(); // peak at the next token, to see if it is a type.


    int initial_bool = 0;

    if(pi.tk.tp == RESWORD){ // if it is a reserved word.
        if(strcmp(pi.tk.lx, "int") == 0)
        {
            pi.tk = GetNextToken(); // get the next token.
            temp_type = INT; //////// set the temp type to int.
            initial_bool = 1;
        }
        else if(strcmp(pi.tk.lx, "char") == 0){
            pi.tk = GetNextToken(); // get the next token.
            temp_type = CHAR; //////// set the temp type to char.

            initial_bool = 1;
        }
        else if(strcmp(pi.tk.lx, "boolean") == 0){
            temp_type = BOOLEAN; //////// set the temp type to boolean.
            pi.tk = GetNextToken(); // get the next token.
            initial_bool = 1;
        }
        else if(strcmp(pi.tk.lx, "void") == 0){
            temp_type = VOID; //////// set the temp type to void.
            pi.tk = GetNextToken(); // get the next token.
            initial_bool = 1;
        }
    }

    else if(pi.tk.tp == ID){ // if it is an ID.


            pi.tk = GetNextToken(); // get the next token.

            strcpy(temp_lexeme, pi.tk.lx); ///////////////////////////// set the temp lexeme to the ID.

            if(pass % 2 == 0){
                Symbol* symbol_x = get_subroutine(pi.tk.lx);

                if(symbol_x == NULL){
                    pi.er = undecIdentifier;
                    return pi;
                }
            }


            if(current_pass % 2 != 0){  // adding a subroutine to the symbol table.
                add_symbol(temp_lexeme, temp_type, SUBROUTINE); // add the subroutine to the symbol table.
            }
            initial_bool = 1;
        }

    if(initial_bool == 0){
        pi.er = illegalType;
        return pi;
    }
    else{
        pi.tk = PeekNextToken(); // peak at the next token, to see if it is an ID.

        if(pi.tk.tp == ID){ // if it is an ID.
            pi.tk = GetNextToken(); // get the next token.

            strcpy(temp_lexeme, pi.tk.lx); ///////////////////////////// set the temp lexeme to the ID.

            if(current_pass % 2 != 0){  // adding a subroutine to the symbol table.
                add_symbol(temp_lexeme, temp_type, SUBROUTINE); // add the subroutine to the symbol table.
            }


            pi.tk = PeekNextToken(); // peak at the next token, to see if it is an open parenthesis.

            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){ // if it is an open parenthesis.
                pi.tk = GetNextToken(); // get the open parenthesis.

                pi = paramlist(pi); // call's the paramlist function.

                pi.tk = PeekNextToken(); // see's if there is a close parenthesis.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){ // if it is a close parenthesis.
                    pi.tk = GetNextToken(); // get the close parenthesis.

                    pi = subroutineBody(pi); // call's the subroutineBody function.




//                    return pi; // return the parser info.


                }
                else{
                    pi.er = closeParenExpected;
                    return pi;
                }
            }
            else{
                pi.er = openParenExpected;
                return pi;
            }
        }
        else{
            pi.er = idExpected;
            return pi;
        }
    }





    return pi;
}


ParserInfo classVarDeclar(ParserInfo pi){


    pi.tk = GetNextToken(); // if it is a class variable declaration then get the next token, which is (static or field).



    pi = type(pi); // call's the type function.

    if(pi.er != none){ // if there is an error getting the type.
        return pi;
    }

    // add type to the symbol table.

    //assign type to a temporary variable.
    temp_type = pi.tk.tp; /////////////////////////////



    //add type to the symbol table.



    pi.tk = PeekNextToken(); // peak at the next token, to see if it is an ID.

    if(pi.tk.tp == ID){ // if it is an ID.
        strcpy(temp_lexeme, pi.tk.lx); /////////////////////////////

        pi.tk = GetNextToken(); // Gets the ID.




        // assigns the lexeme to a temporary variable.


        //before adding the symbol, checking if the symbol already exists.
        pass = get_current_pass(); /////////////////////////////
        if(pass % 2 != 0){ // for this one you need to check before you add it to the symbol table. so it's going to be int the same loop as the adding.
            Symbol* symbol_x = get_symbol(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.

            Symbol* symbol_y = get_class(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.


            //check that the symbol is the same type.


            if(symbol_x != NULL && symbol_y == NULL){ // if the symbol exists.
                pi.er = redecIdentifier;
                printf("it came here 1");
                return pi;
            }
        }


        // add the ID to the symbol table.
        pass = get_current_pass();
        if(pass % 2 != 0){
            add_symbol(temp_lexeme, temp_type, VAR); /////////////////////////////
        }

        pi.tk = PeekNextToken(); // peak at the next token, to see if it is a comma or a semicolon.
        while(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ','){
            pi.tk = GetNextToken(); // get the comma.

            pi.tk = PeekNextToken(); // peak at the next token, to see if it is an ID.

            if(pi.tk.tp == ID){ // if it is an ID.
                pi.tk = GetNextToken(); // get the ID.

                if(pass % 2 != 0){
                    strcpy(temp_lexeme, pi.tk.lx); /////////////////////////////
                    add_symbol(temp_lexeme, temp_type, VAR);
                }

                pi.tk = PeekNextToken(); // peak at the next token, to see if it is a comma or a semicolon.
            }
            else{
                pi.er = idExpected;
                return pi;
            }
        }


        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ';'){
            pi.tk = GetNextToken();

            // get the semicolon.
        }
        else{
            pi.er = semicolonExpected;
            return pi;
        }


    }
    else{
        pi.er = idExpected;
        return pi;
    }



    return pi;
}




ParserInfo memberDeclar(ParserInfo pi) {
    // Gets the next token after the bracket.

    pi.tk = PeekNextToken();
    // if first token is static or field then it is a class variable declaration.

    if(pi.tk.tp == RESWORD){ // if it is a reserved word.
        if(strcmp(pi.tk.lx, "static") == 0){

            pi = classVarDeclar(pi);
        }
        else if(strcmp(pi.tk.lx, "field") == 0){
            pi = classVarDeclar(pi);
        }
        // if first token is constructor, function, or method then it is a subroutine declaration.
        else if( strcmp(pi.tk.lx, "constructor") == 0){
            pi = subroutineDeclar(pi);
        }
        else if( strcmp(pi.tk.lx, "function") == 0){
            pi = subroutineDeclar(pi);
        }
        else if(strcmp(pi.tk.lx, "method") == 0){
            pi = subroutineDeclar(pi);
        }
        else{
            return pi;
        }
    }

    return pi;
}


ParserInfo classDeclar(ParserInfo pi)
{
    if (pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "class") == 0)
    { // if it has the word class

        temp_type = RESWORD; /////////////////////////////


        pi.tk = PeekNextToken(); // Peak at the identifier.

        if (pi.tk.tp == ID)
        { // if it is an ID.
// set the current class name.


            //            pi.tk = GetNextToken();
            //            pi.tk = PeekNextToken();

            //            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] != '{'){
            //                pi.er = openBraceExpected;
            //                return pi;
            //            }

            pi.tk = GetNextToken(); // get the identifier.

            strcpy(temp_lexeme, pi.tk.lx); /////////////////////////////

            add_symbol(temp_lexeme, temp_type, STATIC); /////////////////////////////

            set_current_class_name(pi.tk.lx); /////////////////////////////


            pi.tk = PeekNextToken(); // peak and see if there is an open bracket.

            if (pi.tk.tp == SYMBOL && pi.tk.lx[0] == '{')
            {
                ParanthesisCount += 1;

                pi.tk = GetNextToken(); // gets the Open bracket.

//                pi.tk = PeekNextToken(); //peek's at the next token.


                // call the member declar function.

                pi = memberDeclar(pi);

                if(pi.er != none){
                    return pi;
                }

                if(pi.er == none){ // if there is no error.

                    //eats all the member declarations.

                    pi.tk = PeekNextToken();





                    while ( pi.tk.lx[0] != '}' && pi.er == none)
                    {
                        //check if we have reached end of file.
                        if(pi.tk.tp == EOFile){
                            pi.er = closeBraceExpected;
                            return pi;
                        }

                        pi = memberDeclar(pi);
                        if(pi.er != none){
                            return pi;
                        }

                        pi.tk = PeekNextToken();





                    }

                    return pi;
                }








                pi.tk = GetNextToken(); // get's the closing bracket.

                if( pi.tk.tp != SYMBOL && pi.tk.lx[0] != '}'){ // if it's not a closing bracket returns an error.
                    pi.er = closeBraceExpected;
                    pi.tk = pi.tk;
                    return pi;
                }







//                pi.er = none;
                return pi;


            }
            else{
                //if there is not an opening bracket.
                pi.er = openBraceExpected;
                pi.tk = pi.tk;
                return pi;
            }

            pi.er = none;
            return pi;



        }
        else
        { // there isn't an id.
            pi.er = idExpected;
            return pi;
        }
    }
    else
    {
        pi.er = classExpected;
        return pi;
    }

    return pi;
}


ParserInfo operand(ParserInfo pi)
{
    pi.tk = PeekNextToken(); // peak at the next token.


    if(pi.tk.tp == INT){ // check if it is an integer.
        pi.tk = GetNextToken();
        return pi;
    }
    else if(pi.tk.tp == ID){ // check if it is an ID.
        pi.tk  = GetNextToken(); // get the ID

        if(current_pass % 2 == 0){
            Symbol* symbol_x = get_symbol(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.

            Symbol* symbol_y = get_subroutine(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.

            strcpy(temp_lexeme, pi.tk.lx);
            temp_type = pi.tk.tp;

            //check if the expression exists, in the symbol table.



            if(symbol_x == NULL && temp_type != INT && symbol_y == NULL){
                pi.tk.tp = temp_type;
                strcpy(pi.tk.lx, temp_lexeme);
                pi.er = undecIdentifier;
                return pi;
            }
        }


        pi.tk = PeekNextToken(); // peak at the next token.

        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '.'){
            pi.tk = GetNextToken(); // get the dot.

            pi.tk = PeekNextToken(); // peak at the next token.

            if(pi.tk.tp == ID){

                pi.tk = GetNextToken(); // get the ID.

                pass = get_current_pass();

                if(pass % 2 == 0){
                    Symbol* symbol_x = get_symbol(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.

                    Symbol* symbol_y = get_subroutine(pi.tk.lx); ///////////////////////// get the symbol from the symbol table.

                    strcpy(temp_lexeme, pi.tk.lx);
                    temp_type = pi.tk.tp;

                    //check if the expression exists, in the symbol table.



                    if(symbol_x == NULL && temp_type != INT && symbol_y == NULL){
                        pi.tk.tp = temp_type;
                        strcpy(pi.tk.lx, temp_lexeme);
                        pi.er = undecIdentifier;
                        return pi;
                    }
                }

                pi.tk = PeekNextToken(); // peak at the next token.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '['){ // Does the open bracket option [expression].
                    pi.tk = GetNextToken(); // get the open bracket.

                    pi = expression(pi);

                    if(pi.er == none){
                        pi.tk = PeekNextToken(); // peak at the next token.

                        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ']'){
                            pi.tk = GetNextToken(); // get the close bracket.
                        }
                        else{
                            pi.er = closeBracketExpected;
                            return pi;
                        }
                    }
                    else{
                        return pi;
                    }
                }
                else if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){ // Does the open paran option (expression).
                    pi.tk = GetNextToken(); // get the open bracket.

                    pi.tk = PeekNextToken(); // peak at the next token.

                    if(pi.tk.lx[0] != ')'){
                        pi =  expressionList(pi);
                    }


                    if(pi.er == none){
                        pi.tk = PeekNextToken(); // peak at the next token.

                        if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                            pi.tk = GetNextToken(); // get the close bracket.
                        }
                        else{
                            pi.er = closeParenExpected;
                            return pi;
                        }
                    }
                    else{
                        return pi;
                    }
                }
                else{
                    return pi; // if it is not a [ or ( then it is just an ID.
                }


            }


        }

        else if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '['){ // Does the open bracket option [expression]. with no .ID.
            pi.tk = GetNextToken(); // get the open bracket.

            pi = expression(pi);

            if(pi.er == none){
                pi.tk = PeekNextToken(); // peak at the next token.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ']'){
                    pi.tk = GetNextToken(); // get the close bracket.
                }
                else{
                    pi.er = closeBracketExpected;
                    return pi;
                }
            }
            else{
                return pi;
            }
        }
        else if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){ // Does the open paran option (expression).
            pi.tk = GetNextToken(); // get the open bracket.

            if(pi.tk.lx[0] != ')'){
                pi = expression(pi);
            }

            if(pi.er == none){
                pi.tk = PeekNextToken(); // peak at the next token.

                if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                    pi.tk = GetNextToken(); // get the close bracket.
                }
                else{
                    pi.er = closeParenExpected;
                    return pi;
                }
            }
            else{
                return pi;
            }
        }
        else{
            return pi; // if it is not a [ or ( then it is just an ID.
        }






    }

    else if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == '('){ // if it's not an ID or an integer it could be an (expression).
        pi.tk = GetNextToken(); // get the open bracket.

        pi = expression(pi);

        if(pi.er == none){
            pi.tk = PeekNextToken(); // peak at the next token.

            if(pi.tk.tp == SYMBOL && pi.tk.lx[0] == ')'){
                pi.tk = GetNextToken(); // get the close bracket.
            }
            else{
                pi.er = closeParenExpected;
                return pi;
            }
        }
        else{
            return pi;
        }
    }
    // check for a string literal now.
    else if(pi.tk.tp == STRING){
        pi.tk = GetNextToken(); // get the string literal.
        return pi;
    }
    //check if it's the word true.
    else if(pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "true") == 0){
        pi.tk = GetNextToken(); // get the true.
        return pi;
    }
    //check if it's the word false.
    else if(pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "false") == 0){
        pi.tk = GetNextToken(); // get the false.
        return pi;
    }
    //check if it's the word null.
    else if(pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "null") == 0){
        pi.tk = GetNextToken(); // get the null.
        return pi;
    }
    //check if it's the word this.
    else if(pi.tk.tp == RESWORD && strcmp(pi.tk.lx, "this") == 0){
        pi.tk = GetNextToken(); // get the this.
        return pi;
    }
    else{ // if it's not any of the above then it's an error.
        pi.er = syntaxError;
        return pi;
    }





    return pi;
}



ParserInfo factor(ParserInfo pi)
{

    pi.tk = PeekNextToken();

    if(pi.tk.tp == SYMBOL && (strcmp(pi.tk.lx, "-") == 0 || strcmp(pi.tk.lx, "~") == 0)){
        pi.tk = GetNextToken();

        pi = operand(pi);

    }
    else{
        pi = operand(pi);


    }



    return pi;
}


ParserInfo Parse()
{
    ParserInfo pi;




    Token t = GetNextToken(); //get the class name.

    // Check for lexical error
    if (t.tp == ERR)
    {

        pi.tk = t;
        pi.er = lexerErr;
        return pi;
    }


    // if first word is not class then it gives a class expected error.
    pi.tk = t;
    pi.er = none;

    pi = classDeclar(pi);

    if (pi.er != none)
    {
        return pi;
    }



    pi.er = none;
//    printf("Paranthesis: %d\n", ParanthesisCount);
//    printf("This is the id: %s\n", pi.tk.lx);
//    printf("Line number: %d\n", pi.tk.ln);
    return pi;
}

int StopParser()
{
    StopLexer();
    return 1;
}



