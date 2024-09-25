/* 
Operating Systems Repeat Assignment - 07/24
Maria Sapovalova
ID - 22336983
I acknowledge that this work falls under DCU's Academic Integrity Policy
*/

#include "myshell.h"

// goes to internal commands
void init(char **args, int argc) {
    int in_redirect = 0, out_redirect = 0, append = 0;
    char *input_file = NULL, *output_file = NULL;

    // Check for I/O redirection
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            in_redirect = 1;
            input_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">") == 0) {
            out_redirect = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            out_redirect = 1;
            append = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        }
    }

    // Handle internal commands with output redirection
    if (!internalCommands(args, argc)) {
        return;
    }

    // If it's not an internal command, handle it as an external command
    externalCommand(args, in_redirect, input_file, out_redirect, append, output_file);
}

// Internal commands function
int internalCommands(char **args, int count) {
    int out_redirect = 0, append = 0;
    char *output_file = NULL;

    // Check for output redirection in internal commands
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            out_redirect = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            out_redirect = 1;
            append = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        }
    }

    int stdout_fd = dup(STDOUT_FILENO);
    FILE *out = NULL;

    if (out_redirect) {
        if (append) {
            out = fopen(output_file, "a");
        } else {
            out = fopen(output_file, "w");
        }
        if (!out) {
            perror("Error opening output file");
            return 0;
        }
        dup2(fileno(out), STDOUT_FILENO);
    }

    // Internal commands logic
    if (!strcmp(args[0], "cd")) {
        cd(args[1]);
    } else if (!strcmp(args[0], "dir")) {
        dir(args);
    } else if (!strcmp(args[0], "clr")) {
        clr();
    } else if (!strcmp(args[0], "environ")) {
        environCommand();
    } else if (!strcmp(args[0], "echo")) {
        for (int i = 1; i < count; i++) {
            if (args[i] == NULL) break;
            printf("%s ", args[i]);
        }
        printf("\n");
    } else if (!strcmp(args[0], "help")) {
        help();
    } else if (!strcmp(args[0], "pause")) {
        pauseCommand();
    } else if (!strcmp(args[0], "quit")) {
        quitCommand();
    } else {
        if (out_redirect) {
            fclose(out);
            dup2(stdout_fd, STDOUT_FILENO);
            close(stdout_fd);
        }
        return 1; // Indicate that the command was not an internal command
    }

    if (out_redirect) {
        fclose(out);
        dup2(stdout_fd, STDOUT_FILENO);
        close(stdout_fd);
    }

    return 0; // Indicate that the command was an internal command
}

// Function to handle external commands with I/O redirection
void externalCommand(char **args, int in_redirect, char *input_file, int out_redirect, int append, char *output_file) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        // Set environment variable
        setenv("parent", "/myshell", 1);

        // Handle input redirection
        if (in_redirect) {
            FILE *in = fopen(input_file, "r");
            if (!in) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fileno(in), STDIN_FILENO);
            fclose(in);
        }

        // Handle output redirection
        if (out_redirect) {
            FILE *out;
            if (append) {
                out = fopen(output_file, "a");
            } else {
                out = fopen(output_file, "w");
            }
            if (!out) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fileno(out), STDOUT_FILENO);
            fclose(out);
        }

        // Execute the command
        if (execvp(args[0], args) == -1) {
            perror("Error executing command");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("Error forking");
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

void cd(char *dir) {
    if (dir == NULL) {
        // No argument provided, report current directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current directory: %s\n", cwd);
        } else {
            perror("Error getting current directory");
        }
    } else {
        // Change directory
        if (chdir(dir) == 0) {
            setenv("PWD", dir, 1); // Update PWD environment variable
        } else {
            perror("Error changing directory");
        }
    }
}

void clr() {
    system("clear"); // Execute the "clear" command
}

void dir(char ** args) {
    const char *directory;
    if (args[1] == NULL){
        directory = ".";
    }
    else{
        directory = args[1];
    }
    char command[100];
    strcpy(command, "ls -al ");
    strcat(command, directory);
    printf("%s\n", command);
    system(command);
}

void echo(char *text) {
    printf("%s\n", text);
}

void help() {
    system("more -d README.md");
}

//extern char **environ; // Declare the global environment variable

void environCommand() {
 // Access the existing global environ variable
    int i = 0;
    while(environ[i] != NULL){
        printf("%s\n", environ[i++]);
    }
}
void pauseCommand() {
    printf("Press Enter to continue...\n");
    getchar(); // wait for user input
}

void quitCommand() {
    printf("Exiting the shell... \nGoodbye!\n");
    exit(0); // terminate program
}