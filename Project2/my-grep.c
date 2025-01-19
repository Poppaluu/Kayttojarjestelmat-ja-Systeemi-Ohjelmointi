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
    int nextFile = 2;
    int check = 0;

    // Only the ./my-grep is given
    if (argc == 1)
    {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        exit(1);
    }

    size_t length = strlen(argv[1]);

    // No search term
    if (length == 0)
    {
        fprintf(stderr, "my-grep: Empty search term is ambiguous\n");
        exit(1);
    }

    if (argc == 2)
    {
        rfile = stdin;

        // read lines from stdin
        while ((read = getline(&line, &len, rfile)) != -1)
        {
            for (size_t i = 0; i < read; i++)
            {
                // If the letters match
                if (line[i] == argv[1][check])
                {
                    check++;
                    if (check == length)
                    {
                        printf("%s", line);
                        check = 0;
                        break;
                    }
                }
                else if (line[i] == argv[1][0])
                {
                    check = 1;
                }
                else
                {
                    check = 0;
                }
            }
        }
    }
    else
    {

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
            // read file line by line
            while ((read = getline(&line, &len, rfile)) != -1)
            {
                // print letter one at a time
                for (size_t i = 0; i < read; i++)
                {
                    // printf("line[i] %c argv[2][i] %c\n", line[i], argv[1][check]);
                    if (line[i] == argv[1][check])
                    {
                        check++;
                        if (check == length)
                        {
                            printf("%s", line);
                            check = 0;
                            break;
                        }
                    }
                    else if (line[i] == argv[1][0])
                    {
                        check = 1;
                    }
                    else
                    {
                        check = 0;
                    }
                }
            }

            // close file
            fclose(rfile);
            nextFile++;
        }
    }

    // free memory
    free(line);

    exit(0);
}