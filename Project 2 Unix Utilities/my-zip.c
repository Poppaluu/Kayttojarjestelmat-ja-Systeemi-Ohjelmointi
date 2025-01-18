// chatgpt: newline stripping

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *rfile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int nextFile = 1;
    char checkChar = '\0';
    int count = 0;

    // Only the ./my-zip is given
    if (argc < 2)
    {
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");
        return 1;
    }

    // Read each file given
    while (argv[nextFile] != NULL)
    {
        // Open input file
        rfile = fopen(argv[nextFile], "r");

        // Cant open input file
        if (rfile == NULL)
        {
            fprintf(stderr, "my-grep: cannot open file %s\n", argv[nextFile]);
            exit(1);
        }

        // Read each line from the file
        while ((read = getline(&line, &len, rfile)) != -1)
        {
            // Strip the newline character, if present
            if (line[read - 1] == '\n')
            {
                line[--read] = '\0'; // Reduce the length and null-terminate the string
            }

            checkChar = '\0';
            count = 0;

            // Loop through each character in the line
            for (size_t i = 0; i < read; i++)
            {
                // If the current character changes or it's the first character
                if (checkChar != line[i] && checkChar != '\0')
                {
                    // printf("%i%c", count, checkChar); // Print count and character
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&checkChar, sizeof(char), 1, stdout);
                    count = 0; // Reset count
                }

                checkChar = line[i];
                count++;
            }

            // printf("\n"); // Add a newline after processing each line
        }

        // Print the last group after the loop ends
        if (checkChar != '\0' && checkChar != '\n')
        {
            // printf("%i%c", count, checkChar);
            fwrite(&count, sizeof(int), 1, stdout);
            fwrite(&checkChar, sizeof(char), 1, stdout);
        }

        // close file
        fclose(rfile);
        nextFile++;
    }

    // free memory
    free(line);

    exit(0);
}