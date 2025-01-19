// the behaviour of the shell https://brennan.io/2015/01/16/write-a-shell-in-c/
// snprintf https://www.geeksforgeeks.org/snprintf-c-library/
// chatGPT: execute command

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define MAX_PATHS 100
#define PROMPT "wish> "
#define ERROR_MSG "An error has occurred\n"

char *paths[MAX_PATHS] = {"/bin", NULL}; // Default path

void print_error()
{
    write(STDERR_FILENO, ERROR_MSG, strlen(ERROR_MSG));
}

// Execute an external command
void execute_command(char **args)
{
    for (int i = 0; paths[i] != NULL; i++)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], args[0]);

        if (access(full_path, X_OK) == 0)
        {
            execv(full_path, args);
            print_error(); // Should not reach here if execv succeeds
            exit(1);
        }
    }
    print_error();
    exit(1);
}

// Parse and execute a single command
void parse_and_execute(char *line)
{
    if (line == NULL || strlen(line) == 0)
    {
        return; // Ignore empty or NULL input
    }

    char *args[MAX_ARGS];
    char *token = strtok(line, " \a\r\t\n");

    int argc = 0;
    while (token != NULL && argc < MAX_ARGS - 1)
    {
        args[argc++] = token;
        token = strtok(NULL, " \a\r\t\n");
    }
    args[argc] = NULL;

    if (argc == 0)
    {
        return; // No arguments to execute
    }

    // Built-in command: exit
    if (strcmp(args[0], "exit") == 0)
    {
        if (argc != 1)
        {
            print_error();
        }
        else
        {
            exit(0);
        }
    }

    // Built-in command: cd
    if (strcmp(args[0], "cd") == 0)
    {
        if (argc != 2)
        { // `cd` must have exactly one argument
            printf("this is the error");
            print_error();
        }
        else if (chdir(args[1]) != 0)
        { // Change directory
            printf("args %s\n", args[1]);
            print_error();
        }
        return; // Exit after handling `cd`
    }

    // Built-in command: path
    if (strcmp(args[0], "path") == 0)
    {
        memset(paths, 0, sizeof(paths));
        for (int i = 1; i < argc && i < MAX_PATHS - 1; i++)
        {
            paths[i - 1] = args[i];
        }
        return;
    }

    // Check for redirection
    char *output_file = NULL;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            if (i + 1 < argc && output_file == NULL)
            {
                output_file = args[i + 1];
                args[i] = NULL; // Terminate args here
            }
            else
            {
                print_error();
                return;
            }
        }
    }

    // Fork and execute the command
    pid_t pid = fork();
    if (pid == 0)
    { // Child process
        if (output_file != NULL)
        {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0)
            {
                print_error();
                exit(1);
            }
            close(fd);
        }
        execute_command(args);
    }
    else if (pid > 0)
    {               // Parent process
        wait(NULL); // Wait for the child to finish
    }
    else
    {
        print_error();
    }
}

// Main shell loop
void shell_loop(FILE *input)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        // Print prompt for interactive mode
        if (input == stdin)
        {
            printf(PROMPT);
        }

        // Read input line
        if (input != stdin)
        {

            while (getline(&line, &len, input) != -1)
            {
                line[strcspn(line, "\n")] = '\0';
                parse_and_execute(line);
            }
            // EOF
            break;
        }

        // Read input line
        if (getline(&line, &len, input) == -1)
        {
            break; // EOF or error
        }

        // Handle parallel commands
        char *command = strtok(line, "&");
        while (command != NULL)
        {
            parse_and_execute(command);
            command = strtok(NULL, "&");
        }
    }

    free(line);
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        print_error();
        exit(1);
    }

    FILE *input = stdin;

    // Batch mode
    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            print_error();
            exit(1);
        }
    }

    shell_loop(input);

    if (input != stdin)
    {
        fclose(input);
    }

    return 0;
}
