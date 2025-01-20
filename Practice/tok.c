#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void test(char *line)
{
    // Remove unwanted characters and print the result
    char clean_line[1024]; // Temporary buffer for sanitized string
    int j = 0;

    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] != '\n' && line[i] != '\r') // Exclude newlines and carriage returns
        {
            clean_line[j++] = line[i];
        }
    }

    clean_line[j] = '\0'; // Null-terminate the sanitized string

    printf("%s\n", clean_line); // Print without any unwanted characters
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
            printf("wish>");
        }

        // Read input line
        if (getline(&line, &len, input) == -1)
        {
            // EOF or error
            break;
        }

        // Handle parallel commands
        char *command = strtok(line, " & ");
        while (command != NULL)
        {
            test(command);
            command = strtok(NULL, "&");
        }
    }

    free(line);
}

int main(int argc, char *argv[])
{

    FILE *input = stdin;

    shell_loop(input);

    return 0;
}
