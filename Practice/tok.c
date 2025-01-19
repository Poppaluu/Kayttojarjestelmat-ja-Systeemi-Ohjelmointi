#include <stdio.h>
#include <string.h>

void test(char *line)
{
    printf("%s", line);
    char *token = strtok(line, "&");

    while (token != NULL)
    {
        printf(" %s\n", token);
        printf("hello wello");
        token = strtok(NULL, "&");
    }
}

int main()
{
    char line[] = "Gookk & Geeks";

    // Returns first token

    // Keep printing tokens while one of the
    // delimiters present in str[].
    test(line);

    return 0;
}