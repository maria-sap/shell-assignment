/* 
Operating Systems Repeat Assignment - 07/24
Maria Sapovalova
ID - 22336983
I acknowledge that this work falls under DCU's Academic Integrity Policy
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

// Defines
#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators
#define CURRENT_DIRECTORY_SIZE 200             // Size associated with current directory buffer

void init(char ** args, int argc);
void cd(char *directory);
void clr();
void dir(char **args);
void environCommand();
void echo(char *text);
void help();
void pauseCommand();
void quitCommand();
int internalCommands(char **args, int count);
void externalCommand(char **args, int in_redirect, char *input_file, int out_redirect, int append, char *output_file);

extern char ** environ;