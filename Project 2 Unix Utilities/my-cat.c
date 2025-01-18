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

    // Only the ./my-cat is given
    if (argc == 1)
    {
        exit(0);
    }

    while (argv[nextFile] != NULL)
    {
        // Open input file
        rfile = fopen(argv[nextFile], "r");

        // Cant open input file
        if (rfile == NULL)
        {
            fprintf(stderr, "my-cat: Cannot open file %s\n", argv[nextFile]);
            exit(1);
        }
        // Read each line from the file and print it
        while ((read = getline(&line, &len, rfile)) != -1)
        {
            printf("%s", line);
        }

        // close file
        fclose(rfile);
        nextFile++;
    }

    // free memory
    free(line);

    exit(0);
}