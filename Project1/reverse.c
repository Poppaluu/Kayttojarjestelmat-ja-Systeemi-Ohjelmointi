// https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
// chatgpt: strcmp when comparing file names
// chatgpt: helped to set up the stdin and stdout in case of only one argument

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *rfile;
    FILE *wfile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int size = 5;
    int count = 0;

    // Memory allocation for the array of strings
    char **text = (char **)malloc(size * sizeof(char *));

    // Error checking memory allocation
    if (text == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Only the ./reverse is given
    if (argc == 1)
    {
        rfile = stdin;
        wfile = stdout;
    }

    // input file is given
    else if (argc == 2)
    {
        rfile = fopen(argv[1], "r");

        // Cant open input file
        if (rfile == NULL)
        {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            exit(1);
        }
        wfile = stdout;
    }

    // both input and output files are given
    else if (argc == 3)
    {
        rfile = fopen(argv[1], "r");

        // Cant open input file
        if (rfile == NULL)
        {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            exit(1);
        }

        wfile = fopen(argv[2], "w");

        // Cant open output file
        if (wfile == NULL)
        {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[2]);
            fclose(rfile);
            exit(1);
        }

        // Check if input and output files are the same
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, "Error: Input and output file must differ\n");
            fclose(rfile);
            fclose(wfile);
            exit(1);
        }
    }
    // Too many arguments
    else
    {
        fprintf(stderr, "Usage: reverse [input file] [output file]\n");
        exit(1);
    }

    // Read each line from the file and add it to the list
    while ((read = getline(&line, &len, rfile)) != -1)
    {
        // allocate memory if needed
        if (count >= size)
        {
            size += 10;
            // reallocation if there are more lines than there is space on the array
            text = (char **)realloc(text, size * sizeof(char *));
            if (text == NULL)
            {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(1);
            }
        }
        text[count++] = strdup(line);
    }

    // print the array in reverse order
    if (wfile == stdout)
    {
        printf("\n\nText array:\n");
        for (int i = count - 1; i >= 0; i--)
        {
            printf("%s", text[i]);
            if (text[i][strlen(text[i]) - 1] != '\n')
            {
                printf("\n");
            }
        }
    }
    // write the array to the output file in reverse order
    else
    {
        for (int i = count - 1; i >= 0; i--)
        {
            fprintf(wfile, "%s", text[i]);
            // stopping the last item written from being
            // written next to the second last item
            if (text[i][strlen(text[i]) - 1] != '\n')
            {
                fprintf(wfile, "%s", "\n");
            }
        }
    }

    // Free the allocated memory
    for (int i = 0; i < count; i++)
    {
        free(text[i]);
    }

    // close files
    fclose(rfile);
    fclose(wfile);

    // free memory
    free(text);
    free(line);

    exit(0);
}