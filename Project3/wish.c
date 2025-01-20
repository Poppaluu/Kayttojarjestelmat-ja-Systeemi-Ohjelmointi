// the behaviour of the shell https://brennan.io/2015/01/16/write-a-shell-in-c/
// snprintf https://www.geeksforgeeks.org/snprintf-c-library/
// chatGPT: execute command
// memset https://www.geeksforgeeks.org/memset-c-example/
// forking: https://www.scaler.com/topics/c-fork/
// chatGPT: fork file opening

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define MAX_PATHS 100
#define PROMPT "wish> "

// Default path
char *paths[MAX_PATHS] = {"/bin", NULL};

void print_error(char *error)
{
    write(STDERR_FILENO, error, strlen(error));
}

// Execute an external command
void execute_command(char **args)
{
    // search for the command in args from the directories in path variable
    for (int i = 0; paths[i] != NULL; i++)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], args[0]);

        // access to the path is successful
        if (access(full_path, X_OK) == 0)
        {
            // execute the command
            execv(full_path, args);
            // cant reach this if execv is a success
            print_error("executing the command has failed\n");
            exit(1);
        }
    }
    print_error("could not find the given command in the path variables\n");
    exit(1);
}

// Parse and execute a single command
void parse_and_execute(char *line)
{
    // Ignore empty or NULL input
    if (line == NULL || strlen(line) == 0)
    {
        return;
    }

    char *args[MAX_ARGS];

    // parse
    char *token = strtok(line, " \a\r\t\n");
    int argc = 0;
    while (token != NULL && argc < MAX_ARGS - 1)
    {
        args[argc++] = token;
        // more parse
        token = strtok(NULL, " \a\r\t\n");
    }
    args[argc] = NULL;

    // No arguments to execute
    if (argc == 0)
    {
        return;
    }

    // exit the program
    if (strcmp(args[0], "exit") == 0)
    {
        if (argc != 1)
        {
            print_error("Can't exit due to too many arguments. Write 'exit' to exit the program\n");
            return;
        }
        else
        {
            exit(0);
        }
    }

    // cd command
    if (strcmp(args[0], "cd") == 0)
    {
        // cd needs two arguments
        if (argc != 2)
        {
            print_error("not enough or too many arguments for cd. Write 'cd' '[directory]'\n");
        }
        // changing the directory
        else if (chdir(args[1]) != 0)
        {
            print_error("changing directory has failed\n");
        }
        // exiting to while loop after cd successful
        return;
    }

    // Path command
    if (strcmp(args[0], "path") == 0)
    {
        // resetting path variable
        memset(paths, 0, sizeof(paths));
        for (int i = 1; i < argc && i < MAX_PATHS - 1; i++)
        {
            // assigning new values to path variable
            paths[i - 1] = args[i];
        }
        return;
    }

    // Check for redirection
    char *output_file = NULL;
    for (int i = 0; i < argc; i++)
    {
        // going through arguments to find ">"
        if (strcmp(args[i], ">") == 0)
        {
            // output file was given and the init of the file NULL has been succesful
            if (i + 1 < argc && output_file == NULL)
            {
                // assign value to output_file
                output_file = args[i + 1];
                // Terminate args here
                args[i] = NULL;
            }
            // the file couldnt be opened
            else
            {
                print_error("opening the file has failed\n");
                return;
            }
        }
    }

    // Fork and execute the command
    pid_t pid = fork();
    // Child process
    if (pid == 0)
    {
        // the user has redirected the output to a file
        if (output_file != NULL)
        {
            // O_WRONLY --> write only
            // O_CREAT --> create the file if it doesnt exist
            // O_TRUNC --> truncate, or clear the file of its previous contents
            // 0644 --> permissions for the owner to read and write
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            // redirecting the input to the output file
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0)
            {
                print_error("opening the file has failed\n");
                exit(1);
            }
            // closing the file
            close(fd);
        }
        execute_command(args);
    }
    // parent process
    else if (pid > 0)
    {
        // Wait for the child to finish
        wait(NULL);
    }
    else
    {
        print_error("fork has failed\n");
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
        // input has been specified to be read from a file
        if (input != stdin)
        {
            // Read input line
            while (getline(&line, &len, input) != -1)
            {
                // remove the newline form the command
                line[strcspn(line, "\n")] = '\0';
                // execute the command
                parse_and_execute(line);
            }
            // EOF
            break;
        }

        // Read input line
        if (getline(&line, &len, input) == -1)
        {
            // EOF or error
            break;
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
        print_error("too many arguments\n");
        exit(1);
    }

    FILE *input = stdin;

    // Batch mode
    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            print_error("Opening the file has failed\n");
            exit(1);
        }
    }

    shell_loop(input);

    if (input != stdin)
    {
        fclose(input);
    }

    exit(0);
}
