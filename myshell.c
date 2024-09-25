/* 
Operating Systems Repeat Assignment - 07/24
Maria Sapovalova
ID - 22336983
I acknowledge that this work falls under DCU's Academic Integrity Policy
*/

#include "myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

int main(int argc, char **argv) {
    char buf[MAX_BUFFER];    // line buffer
    char *args[MAX_ARGS];    // pointers to arg strings
    char **arg;              // working pointer through args
    char *prompt = "--> ";  // shell prompt

    FILE *input = stdin;
    int batchFlag = 0;

    // check for batchfile
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("Error opening batchfile");
            return EXIT_FAILURE;
        }
        batchFlag = 1;
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchfile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Main loop: read commands until "quit" or end of input
    while (1) {
        if (!batchFlag) {
            // Interactive mode: print prompt and read from stdin
            fputs(prompt, stdout);
            if (!fgets(buf, MAX_BUFFER, input)) {
                // End of input (e.g., Ctrl+D in Unix)
                break;
            }
        } else {
            // Batch mode: read commands from batchfile
            if (!fgets(buf, MAX_BUFFER, input)) {
                // End of file reached
                break;
            }
            printf("\n------------------------\nExecuting: %s------------------------\n", buf); // Print the command being executed
        }

        // Tokenize the input into args array
        arg = args;
        *arg++ = strtok(buf, SEPARATORS);   // Tokenize input

        while ((*arg++ = strtok(NULL, SEPARATORS)));

        // Last entry will be NULL
        if (args[0]) { // If there's anything there
            init(args, argc); // Call the shell initialization function
        }
    }

    if (batchFlag) {
        fclose(input); // Close the batchfile if opened
    }

    printf("Exiting the shell... Goodbye!\n");
    return EXIT_SUCCESS;
}
