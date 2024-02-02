/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Hammad Shahid
Student ID: 201442557
Email: ed19h4s@leeds.ac.uk
Date Work Commenced:
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

FILE *jack_file;
char *File_Name;

int LineNumber = 1;
int previous_Line_number = 0;

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
const char *reserved[] = {
    "class", "constructor", "function", "method", "field",
    "static", "var", "int", "char", "boolean", "void",
    "true", "false", "null", "this", "let", "do", "if",
    "else", "while", "return"};

const char *symbols[] = {
    "{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/",
    "%", "=", "==", "!", "!=", "<", ">", "<=", ">=", "&", "|", "~",
    "<<", ">>", "//", "/* */"};

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer(char *file_name)
{
    LineNumber = 1;

//    //remove .jack from filename for e.g. Main.jack to Main
//    char *dot = strrchr(file_name, '.');
//    char vm_file[128];
//
//
//    strncpy(vm_file, file_name, dot - file_name);
//
//    strcat(vm_file, ".vm");
//
//    printf("vm file name is %s\n", vm_file);



    jack_file = fopen(file_name, "r");

//    File_Name = file_name;

    //if there is a slash then it is a filepath.
    File_Name = strrchr(file_name, '/');

    // If the slash is found, extract the filename.
    if (File_Name != NULL) {
        File_Name++;  // Move past the slash
    } else {
        // If the slash is not found, use the whole string as the filename.
        File_Name = file_name;
    }

    if (jack_file == NULL)
    {
        printf("Error: Failed to open file, Jack File is Null\n");
        return 0;
    }


    return 1;
}

int Skip_Comment(int ch)
{
    //    int ch = getc(jack_file);
    //    printf("it comes here");

    if (ch == '/')
    {

        ch = getc(jack_file);

        ////            ch = getc(jack_file);
        ////            if(ch == '\n'){
        ////                LineNumber += 1;
        ////            }
        ////
        ////            while(ch != EOF){
        ////                if(ch == '\n'){
        ////                    LineNumber += 1;
        ////                }
        ////                ch = getc(jack_file);
        ////            }
        //
        //            return -1;
        //
        //        }

        //        printf("%c", ch);

        if (ch == '/')
        {
            //            printf("%c", ch);
            // one line comment
            ch = getc(jack_file);

            //            if(ch=='\n'){
            //                printf("it breaks");
            //            }

            while (ch != '\n')
            {
                //                printf("%c", ch);
                ch = getc(jack_file);
            }
            LineNumber += 1;
            return 1;
        }

        else if (ch == '*')
        { // multi-line comment
            //            printf("/");
            //            printf("*");
            //            printf("This is triggered");
            //            LineNumber += 1; // Always add new line after multi-line comment starts
            ch = getc(jack_file);
            int Bool = 0;
            int print_char;
            while (ch != '/' || Bool != 1)
            {

                ch = getc(jack_file);

                //                printf("%c", ch);
                if (ch == '\n')
                {
                    LineNumber += 1;
                }

                if (ch == '*')
                {
                    //                    printf("%c", ch);
                    //                    Bool = 1;
                    ch = getc(jack_file);
                    if (ch == '/')
                    {

                        //                        printf("it comes here");
                        //                        printf("%c", ch);

                        return 1;
                    }
                    else
                    {
                        Bool = 0;
                    }
                }
                else if (ch == EOF)
                {
                    printf("it came here ");
                    //                    LineNumber += 1;
                    ch = getc(jack_file);
                    while (ch != EOF)
                    {
                        ch = getc(jack_file);
                        if (ch == '\n')
                        {
                            LineNumber += 1;
                        }
                    }
                    return -1;
                }
            }

            return 1;
        }
        //        else  if(ch == 13){
        //            printf("it came here ");
        //            LineNumber += 1;
        //            ch = getc(jack_file);
        //            while(ch != EOF){
        //                ch = getc(jack_file);
        //                if(ch == '\n'){
        //                    LineNumber += 1;
        //                }
        //            }
        //            return -1;
        //        }
        else
        {
            ungetc(ch, jack_file);
        }
    }

    return 0;
}
// Get the next token from the source file

Token GetNextToken()

{

    Token t;



    // copy the file name
    strcpy(t.fl, File_Name);


    // character count to store the characters
    int char_count = 0;

    // get the first character
    int c = getc(jack_file);

    if (c == EOF)
    {
        strcpy(t.lx, "End of File");
        t.ec = EofInStr;
        t.tp = EOFile;
    }

    int Comment = Skip_Comment(c);

    char keyword[128];
    memset(keyword, 0, sizeof(keyword));

    while (isspace(c) || c == '\n' || Comment == 1)
    {
        if (c == '\n')
        {
            LineNumber += 1;
        }

        c = getc(jack_file);

        Comment = Skip_Comment(c);

        if (Comment == -1)
        {

            strcpy(keyword, "Error: unexpected eof in comment");
            t.ec = EofInCom;
            t.tp = ERR;
            t.ln = LineNumber;
        }
    }

    // store the first character
    if (Comment != -1)
    {
        keyword[char_count] = (char)c;
    }

    //    printf("Letter %c ", c);

    //        c = getc(jack_file);

    //        printf("Letter %c ", c);
    //    printf("%c", c);

    if (c == EOF)
    {
        strcpy(t.lx, "End of File");
        t.ec = EofInCom;
        t.tp = EOFile;
        t.ln = LineNumber;
        strcpy(t.lx, File_Name);
        strcpy(t.lx, "End of File");
    }

    else if ((isalpha(c) || c == '_'))
    { // is an id

        t.tp = ID;

        while (c != '\n' && !isspace(c))
        {
            c = getc(jack_file);
            if ((isalpha(c) || isdigit(c) || c == '_'))
            {
                char_count += 1;
                keyword[char_count] = (char)c;
            }

            //            if(c == '\n'){
            //                printf("it came here");
            //                next_Line_number = 0;
            //            }

            if (c == '(' || c == ';' || c == ',' || c == '.' || c == ')' || c == '<' || c == '>' || c == '[' || c == ']' || c == '"' || c == '/' || c=='\n')
            {
                ungetc(c, jack_file);
                break;
            }

            if (c == EOF)
            {
                break;
            }

            //            printf("Letter %c ", c);
        }

        for (int x = 0; x < 21; x++)
        { // is a keyword
            if (strcmp(keyword, reserved[x]) == 0)
            {

                t.tp = RESWORD;
            }
        }

        for (int x = 0; x < 28; x++)
        {
            if (strcmp(keyword, symbols[x]) == 0)
            {
                t.tp = SYMBOL;
            }
        }

        t.ln = LineNumber;

        strcpy(t.lx, keyword);
    }

    else if (isdigit(c))
    {
        t.tp = INT;
        while (c != '\n' && !isspace(c))
        {
            c = getc(jack_file);
            if (isdigit(c))
            {
                char_count += 1;
                keyword[char_count] = (char)c;
            }

            if (c == '(' || c == ';' || c == ',' || c == '.' || c == ')' || c == '<' || c == '>' || c == '[' || c == ']' || c == '"' || c == '/')
            {
                ungetc(c, jack_file);
                break;
            }

            if (c == EOF)
            {
                break;
            }
        }

        t.ln = LineNumber;

        strcpy(t.lx, keyword);
    }

    else if (c == '?')
    {
        strcpy(t.lx, "End of File");
        t.ec = ERR;
        t.tp = ERR;
        t.ln = LineNumber;
        strcpy(t.lx, "Error: illegal symbol in source file");
    }
    else if (!isalpha(c) && !isdigit(c) && c != '"')
    { // is a symbol

        if (Comment != -1)
        {
            t.tp = SYMBOL;
        }

        while (c != '\n' && isspace(c))
        {
            c = getc(jack_file);

            for (int x = 0; x < 28; x++)
            {
                if ((char)c == *symbols[x])
                {
                    char_count += 1;
                    keyword[char_count] = (char)c;
                }
            }

            //            if(char_count > 1){
            //                ungetc(c, jack_file);
            //                break;
            //            }

            //            if(!isalpha(c) && !isnumber(c)){
            //                char_count += 1;
            //                keyword[char_count] = (char) c;
            //
            //            }
            if (c == EOF)
            {
                break;
            }
        }
        t.ln = LineNumber;

        strcpy(t.lx, keyword);
    }
    else if (c == '"')
    { // is a comment.

        t.tp = STRING;
        c = getc(jack_file);
        //        printf("%c", c);

        while (c != '"')
        {

            if (c == '\n')
            {
                strcpy(keyword, "EError: new line in string constant"); // extra e character
                t.ec = EofInCom;
                t.tp = ERR;
                t.ln = LineNumber;
                strcpy(t.lx, File_Name);
                break;
            }

            char_count += 1;
            keyword[char_count] = (char)c;
            c = getc(jack_file);

            if (c == EOF)
            {
                strcpy(keyword, "EError: unexpected eof in string constant");
                t.tp = ERR;
                break;
            }
            //            printf("%d", char_count);
            //            printf("%c", c);
        }

        // removes the first character
        memmove(&keyword[0], &keyword[0 + 1], strlen(keyword) - 0);

        t.ln = LineNumber;

        strcpy(t.lx, keyword);
    }

    //        else if(c == EOF){
    //            t.tp = EOFile;
    //
    //            return t;
    //
    //        }
    //        else if(isdigit((char) c)){
    //            t.tp = INT;
    //            t.ec = 0;
    //            t.ln = 1;
    //
    //
    //            return t;
    //        }

    //    printf("id: %s, LineNumber: %d, CountLines: %d\n", t.lx, LineNumber, Count_new_lines);

    return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken()
{

    Token t;

    long int saved_pos = ftell(jack_file);

    previous_Line_number = LineNumber;

    // Read the next token
    t = GetNextToken();

    // Restore the file position
    fseek(jack_file, saved_pos, SEEK_SET);
    LineNumber = previous_Line_number;

    return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer()
{

    fclose(jack_file);
    File_Name = 0;
    return 0;
}

