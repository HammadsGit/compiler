/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Compiler Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Hammad Shahid
Student ID: 201442557
Email: ed19h4s@leeds.ac.uk
Date Work Commenced: 10/04/2023
*************************************************************************/

#include "compiler.h"
#include "symbols.h"
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#ifndef DT_REG
#define DT_REG 8
#endif

int InitCompiler ()
{
    init_symbol_table();

    // initailize the symbol table.
    return 1;
}
ParserInfo compile (char* dir_name)
{
    ParserInfo p;

    // open the directory
    DIR *dir;
    struct dirent *ent;

    // create a full path.
    char full_path[128] = "SymbolTableSelfGrader";
    strcpy(full_path, dir_name);

    getcwd(full_path, sizeof(full_path)); // get the current working directory

    // remove's the cmake-build-debug from the path. by finding the last occurrence of '/' and then copying everything before it.
    char new_path[128];

    strcpy(new_path, full_path);

    strcat(new_path, "/");

    strcat(new_path, dir_name);

    dir = opendir(new_path);

    if (dir == NULL) { // if the directory is not opened successfully
        perror("Unable to open directory");
        p.er = none; // set the error to lexer error. for directory not found.
        return p;
    }

    else{ // if the directory is opened successfully
        append_current_pass();

        //parse all the main libraries and add them to the symbol table.
        char* libraries[8] =
                {
                        "Math.jack",
                        "Memory.jack",
                        "Output.jack",
                        "Screen.jack",
                        "String.jack",
                        "Sys.jack",
                        "Keyboard.jack",
                        "Array.jack"
                };

        for(int i=0; i<8; i++){
            InitParser(libraries[i]);
            p = Parse();
            StopParser();

        }

        add_symbol("null", 0, VAR);


        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == 8) { // Only print regular files, not directories or special files

                // create a new path for the file.
                char file_path[128];

                strcpy(file_path, new_path);
                strcat(file_path, "/");
                strcat(file_path, ent->d_name);
                InitParser(file_path);
                p = Parse();
                StopParser();

            }
        }
        closedir(dir);
        append_current_pass();

        dir = opendir(new_path);
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == 8) { // Only print regular files, not directories or special files

                // create a new path for the file.
                char file_paths[256];



                strcpy(file_paths, new_path);

                strcat(file_paths, "/");

                strcat(file_paths, ent->d_name);


                if(p.er == none){

                    InitParser(file_paths);
                    p = Parse();
                    StopParser();
                }

                //remove .jack from the name in a new variable.

                char* extension = strrchr(file_paths, '.'); // find last occurrence of '.'

                if (extension != NULL && strcmp(extension, ".jack") == 0) {
                    *extension = '\0'; // overwrite extension with null character
                    strcat(file_paths, ".vm");
                }



//                printf("%s\n", file_paths);

                FILE *fp;


                fp = fopen(file_paths, "w");

                fclose(fp);


            }
        }

        //clears the path for the next compilation.

        strcpy(new_path, "");
        closedir(dir);
//        free(ent);

        p.er = none;
        return p;
    }


    p.er = none;
    return p;
}

int StopCompiler ()
{
    clear_symbol_table();
    return 1;
}

#ifndef TEST_COMPILER
int main ()
{
	InitCompiler ();
	ParserInfo p = compile ("pong");
	StopCompiler ();
	return 1;
}

#endif
