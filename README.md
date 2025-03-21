# Mykhailo Puhach, 23329993, csc1021, CustomShell - Stage 2

## Internal Commands

The following internal commands are supported:

- `cd` - Change the current default directory. If no argument is provided, it changes to the home directory.
- `clr` - Clear the screen.
- `dir` - List the contents of a directory.
- `environ` - List all environment strings.
- `echo` - Display a message.
- `help` - Display a basic user manual.
- `pause` - Pause operation of the shell until 'Enter' is pressed.
- `quit` - Quit the shell.

## Batch Mode

The shell supports batch mode. If the shell is invoked with a command line argument (e.g., `./customshell batchfile`), it will read and execute commands from the specified batchfile.

## External Commands

CustomShell now supports the execution of external commands. This is achieved using `fork()` and `execvp()` system calls. When an external command is invoked, the shell 
creates a child process to execute the command, while the parent process waits for the child to complete (unless background execution is specified).

**Example**:
  
  /home/misha/homework/os==> ls -l

This will list the contents of the current directory in long format.

## Background Execution

Commands can be executed in the background by appending "&" to the command line. This allows the shell to return to the prompt immediately after launching the command, 
while the command continues to run in the background.

**Example**:

    /home/misha/homework/os==> sleep 10 &

    This will run the sleep command in the background, allowing you to continue using the shell while the command executes.

## I/O Redirection

CustomShell supports input and output redirection for both internal and external commands. The following redirection operators are supported:

    < - Redirect input from a file.

    > - Redirect output to a file.

    >> - Redirect output to a file.

    Examples:

    /home/misha/homework/os==>  ls -l > output.txt

    This will redirect the output of the ls -l command to output.txt.

    /current/directory/prompt$ echo "New line" >> output.txt

    This will append "New line" to output.txt.

## Error Handling

The shell includes basic error handling. For example, if a directory does not exist when using the cd command, an appropriate error message is displayed.
Similarly, errors during file operations.

## How to Compile and Run

    Compile the shell by running:

    make

    Run the shell:

    ./customshell

    To run the shell in batch mode:

    ./customshell batchfile
