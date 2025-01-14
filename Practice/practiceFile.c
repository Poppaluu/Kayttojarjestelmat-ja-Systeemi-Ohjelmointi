#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *rfile;
    FILE *wfile;
    char line[256];

    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    // Open the input file for reading
    rfile = fopen(argv[1], "r");
    if (rfile == NULL)
    {
        printf("Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Open the output file for writing
    wfile = fopen(argv[2], "w");
    if (wfile == NULL)
    {
        printf("Could not open output file %s\n", argv[2]);
        fclose(rfile);
        return 1;
    }

    // Read each line from the input file and write it to the output file
    while (fgets(line, sizeof(line), rfile))
    {
        fprintf(wfile, "%s", line);
    }

    // Close the files
    fclose(rfile);
    fclose(wfile);

    return 0;
}