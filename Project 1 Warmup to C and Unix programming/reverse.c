// https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
// chatgpt: strcmp when comparing file names

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

    if (argc > 3)
    {
        fprintf(stderr, "Usage: reverse <input file> <output file>\n");
        exit(1);
    }

    // allocation of memory for the array of strings
    char **text = (char **)malloc(size * sizeof(char *));

    // checking if memory allocation is succesful
    if (text == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Open the input file for reading
    rfile = fopen(argv[1], "r");

    // Open the output file for writing
    wfile = fopen(argv[2], "w");

    // input is the same as output
    if (strcmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "Input and output file must differ");
        exit(1);
    };

    // Read each line from the file and add it to the list
    while ((read = getline(&line, &len, rfile)) != -1)
    {
        // allocate memory if needed
        if (count >= size)
        {
            size += 10;
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
    if (wfile == NULL)
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