// chatgpt: helped me with the fread

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    FILE *rfile;
    int nextFile = 1;
    int length;
    char character;

    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    // Read each file given
    while (argv[nextFile] != NULL)
    {
        // open the file in binary
        rfile = fopen(argv[nextFile], "rb");

        // error checking the file open
        if (rfile == NULL)
        {
            fprintf(stderr, "my-unzip: cannot open file %s\n", argv[nextFile]);
            exit(1);
        }

        // Read and decompress
        while (fread(&length, sizeof(int), 1, rfile) == 1)
        {
            // error checking the fread and assigning the character value
            if (fread(&character, sizeof(char), 1, rfile) != 1)
            {
                fprintf(stderr, "my-unzip: file format error\n");
                fclose(rfile);
                exit(1);
            }

            // print the character as many times as it is in the file
            for (int j = 0; j < length; j++)
            {
                putchar(character);
            }
        }

        // close file
        fclose(rfile);
        nextFile++;
    }

    exit(0);
}
