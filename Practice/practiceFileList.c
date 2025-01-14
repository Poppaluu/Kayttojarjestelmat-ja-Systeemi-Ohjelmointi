#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int size = 5;
    int count = 0;

    // allocation of memory for the array of strings
    char **text = (char **)malloc(size * sizeof(char *));

    // checking if memory allocation is succesful
    if (text == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Open the file for reading
    file = fopen("example.txt", "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    // Read each line from the file and add it to the list
    while ((read = getline(&line, &len, file)) != -1)
    {
        // allocate memory if needed
        if (count >= size)
        {
            size += 10;
            text = (char **)realloc(text, size * sizeof(char *));
            if (text == NULL)
            {
                printf("Memory reallocation failed\n");
                return 1;
            }
        }
        text[count++] = strdup(line);
    }

    // Close the file
    fclose(file);

    // print the array
    printf("\n\nText array:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s", text[i]);
    }

    // print the array inn reverse order
    printf("\n\nText array:\n");
    for (int i = count - 1; i >= 0; i--)
    {
        printf("%s", text[i]);
        if (text[i][strlen(text[i]) - 1] != '\n')
        {
            printf("\n");
        }
    }

    // Free the allocated memory
    for (int i = 0; i < count; i++)
    {
        free(text[i]);
    }

    free(text);
    free(line);

    return 0;
}