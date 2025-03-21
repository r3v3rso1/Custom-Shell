// Mykhailo Puhach, 23329993, csc1021, customshell stage 2

/*
**********************************************

This work was done by Mykhailo Puhach for csc1021 project 1, stage 2.
To complete this stage I took a lot of experience from csc1021 Lab 4/5 instructions and code.

**********************************************
*/

#ifndef CUSTOMSHELL_H
#define CUSTOMSHELL_H

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

// functions from  utility.c
void display_help();
void handle_cd(char **args);
void handle_clr();
void handle_dir(char **args);
void handle_environ();
void handle_echo(char **args, const char *output_file, int append_mode);
void handle_pause();
void handle_quit();
void handle_external_command(char **args, int background, const char *input_file, const char *output_file, int append_mode);

#endif
