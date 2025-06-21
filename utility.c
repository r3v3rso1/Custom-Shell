#include "customshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

// Display help information
void display_help() {
    FILE *help_file = fopen("help.txt", "r");
    if (help_file == NULL) {
        perror("fopen");
        return;
    }
    char line[MAX_BUFFER];
    while (fgets(line, sizeof(line), help_file)) {
        printf("%s", line);
    }
    fclose(help_file);
}

// Change directory
void handle_cd(char **args) {
    char *directory = args[1];
    char cwd[MAX_BUFFER];

    if (directory == NULL) {
        // If no argument is provided
        directory = getenv("HOME");
        if (directory == NULL) {
            fprintf(stderr, "cd: HOME environment not set\n");
            return;
        }
    }

    if (chdir(directory) == 0) {
        if (getcwd(cwd, sizeof(cwd))) {
            if (setenv("PWD", cwd, 1) != 0) { // Update PWD environment variable
                perror("setenv");
            }
        } else {
            perror("getcwd");
        }
    } else {
        perror("chdir");
    }
}

// Clear the screen
void handle_clr() {
    system("clear"); 
}

// List directory contents
void handle_dir(char **args) {
    char command[MAX_BUFFER];
    if (args[1]) {
        snprintf(command, sizeof(command), "ls -al %s", args[1]);
    } else {
        snprintf(command, sizeof(command), "ls -al");
    }
    system(command);
}

// List environment variables
void handle_environ() {
    extern char **environ;
    for (char **env = environ; *env; env++) {
        printf("%s\n", *env);
    }
}

// Echo command with fixed I/O support
void handle_echo(char **args, const char *output_file, int append_mode) {
    FILE *output = stdout;

    // If output_file is specified, open the file for writing
    if (output_file) {
        const char *mode = append_mode ? "a" : "w"; // Append or overwrite mode
        output = fopen(output_file, mode);
        if (output == NULL) {
            perror("fopen");
            return;
        }
    }

    // Print the arguments
    for (int i = 1; args[i] != NULL; i++) {
        fprintf(output, "%s ", args[i]);
    }
    fprintf(output, "\n");

    // Close the file if it was opened
    if (output != stdout) {
        fclose(output);
    }
}

// Pause
void handle_pause() {
    printf("Paused. Press Enter to continue");
    while (getchar() != '\n');
}

// Quit the shell
void handle_quit() {
    exit(0);
}

// Execute external commands
void handle_external_command(char **args, int background, const char *input_file, const char *output_file, int append_mode) {
    pid_t pid = fork(); // Create a new process

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { // Child process
        // Redirect input
        if (input_file) {
            FILE *input_stream = freopen(input_file, "r", stdin); // Redirect stdin to the file
            if (input_stream == NULL) {
                perror("freopen (input)");
                exit(EXIT_FAILURE);
            }
        }

        // Redirect output
        if (output_file) {
            const char *mode = append_mode ? "a" : "w"; // Append or overwrite mode
            FILE *output_stream = freopen(output_file, mode, stdout); // Redirect stdout to the file
            if (output_stream == NULL) {
                perror("freopen (output)");
                exit(EXIT_FAILURE);
            }
        }

        // Execute the command
        execvp(args[0], args);
        perror("execvp"); // Error executing the command
        exit(EXIT_FAILURE);
    } else if (!background) { // Parent process
        wait(NULL); // Wait for child process to complete
    }
}
