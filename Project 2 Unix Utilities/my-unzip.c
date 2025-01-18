#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        return 1;
    }

    FILE *rfile;
    int nextFile = 1;
    int runLength;
    char character;

    // Read each file given
    while (argv[nextFile] != NULL)
    {
        rfile = fopen(argv[nextFile], "rb"); // Open in binary mode
        if (rfile == NULL)
        {
            fprintf(stderr, "my-unzip: cannot open file %s\n", argv[i]);
            return 1;
        }

        // Read and decompress data
        while (fread(&runLength, sizeof(int), 1, rfile) == 1)
        {
            if (fread(&character, sizeof(char), 1, rfile) != 1)
            {
                fprintf(stderr, "my-unzip: file format error\n");
                fclose(rfile);
                return 1;
            }

            // Output the character `runLength` times
            for (int j = 0; j < runLength; j++)
            {
                putchar(character);
            }
        }

        // close file
        fclose(rfile);
        nextFile++;
    }

    return 0;
}
