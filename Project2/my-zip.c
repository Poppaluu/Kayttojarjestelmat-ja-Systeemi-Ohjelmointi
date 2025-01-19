// chatgpt: removal of the newline character
// chatgpt: newline write
// chatgpt: fwrite

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
    int newlineCount = 1;

    // Check for correct usage
    if (argc < 2)
    {
        fprintf(stderr, "my-zip: file1 [file2 ...] > filename.z\n");
        exit(1);
    }

    // Process each file
    while (argv[nextFile] != NULL)
    {
        // Open the file
        rfile = fopen(argv[nextFile], "r");

        // error in file reading
        if (rfile == NULL)
        {
            fprintf(stderr, "my-zip: cannot open file %s\n", argv[nextFile]);
            exit(1);
        }

        // Read each line
        while ((read = getline(&line, &len, rfile)) != -1)
        {
            // Remove the newline character if present
            if (line[read - 1] == '\n')
            {
                // Null-terminate and reduce length
                line[--read] = '\0';
            }

            checkChar = '\0';
            count = 0;

            // go through each character
            for (size_t i = 0; i < read; i++)
            {

                // letter changes
                if (checkChar != line[i] && checkChar != '\0')
                {
                    // write the amount of the letter
                    fwrite(&count, sizeof(int), 1, stdout);
                    // write the character
                    fwrite(&checkChar, sizeof(char), 1, stdout);
                    // reset count
                    count = 0;
                }

                // change the character to the next one
                checkChar = line[i];
                // add to the count of a single character
                count++;
            }

            // Write the last character group of the line
            if (checkChar != '\0')
            {
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&checkChar, sizeof(char), 1, stdout);
            }

            // Write the newline
            fwrite(&newlineCount, sizeof(int), 1, stdout);
            fwrite("\n", sizeof(char), 1, stdout);
        }

        // Close the file
        fclose(rfile);
        nextFile++;
    }

    // Free allocated memory
    free(line);

    exit(0);
}
