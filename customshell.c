#include "customshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char buf[MAX_BUFFER]; // Buffer to store user input
    char *args[MAX_ARGS]; // Array to store parsed command arguments
    char prompt[MAX_BUFFER]; // String for the shell prompt

    // Set the SHELL environment variable
    char shell_path[MAX_BUFFER];
    if (getcwd(shell_path, sizeof(shell_path))) {
        strcat(shell_path, "/customshell");
        setenv("shell", shell_path, 1);
    } else {
        perror("getcwd");
    }

    // Open input file if provided (batch mode), otherwise use standard input
    FILE *input = stdin;
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            perror("fopen");
            return 1;
        }
    }

    // Shell loop
    while (1) {
        // Get current directory for the prompt
        if (getcwd(prompt, sizeof(prompt))) {
            strcat(prompt, "==> ");
        } else {
            perror("getcwd");
            strcpy(prompt, "==> ");
        }

        fputs(prompt, stdout); // Display the prompt

        if (fgets(buf, MAX_BUFFER, input)) { // Read user input
            int arg_count = 0;
            char *input_file = NULL;
            char *output_file = NULL;
            int append_mode = 0;
            int background = 0;

            // Tokenize the input
            char *token = strtok(buf, SEPARATORS);
            while (token != NULL && arg_count < MAX_ARGS - 1) {
                if (strcmp(token, "<") == 0) {
                    // Input redirection
                    token = strtok(NULL, SEPARATORS);
                    if (token == NULL) {
                        fprintf(stderr, "Syntax error: missing filename after <\n");
                        break;
                    }
                    input_file = token;
                } else if (strcmp(token, ">") == 0) {
                    // Output redirection (overwrite)
                    token = strtok(NULL, SEPARATORS);
                    if (token == NULL) {
                        fprintf(stderr, "Syntax error: missing filename after >\n");
                        break;
                    }
                    output_file = token;
                } else if (strcmp(token, ">>") == 0) {
                    // Output redirection (append)
                    token = strtok(NULL, SEPARATORS);
                    if (token == NULL) {
                        fprintf(stderr, "Syntax error: missing filename after >>\n");
                        break;
                    }
                    output_file = token;
                    append_mode = 1;
                } else if (strcmp(token, "&") == 0) {
                    // Background execution
                    background = 1;
                } else {
                    args[arg_count] = token; // Regular argument
                    arg_count++;
                }
                token = strtok(NULL, SEPARATORS);
            }
            args[arg_count] = NULL; // Null-terminate the argument list

            // Check and execute internal and external commands
            if (args[0]) {
                if (!strcmp(args[0], "cd")) handle_cd(args);
                else if (!strcmp(args[0], "clr")) handle_clr();
                else if (!strcmp(args[0], "dir")) handle_dir(args);
                else if (!strcmp(args[0], "environ")) handle_environ();
                else if (!strcmp(args[0], "echo")) handle_echo(args, output_file, append_mode);
                else if (!strcmp(args[0], "help")) display_help();
                else if (!strcmp(args[0], "pause")) handle_pause();
                else if (!strcmp(args[0], "quit")) handle_quit();
                else {
                    handle_external_command(args, background, input_file, output_file, append_mode);
                }
            }
        } else {
            break; // Exit loop on EOF
        }
    }

    if (input != stdin) fclose(input); // Close file if batch mode was used
    return 0;
}
