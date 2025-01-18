#include <stdio.h>

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};

    // Write binary data to stdout
    size_t count = fwrite(numbers, sizeof(int), 5, stdout);

    // Print a status message to stderr to avoid mixing with binary output
    fprintf(stderr, "Wrote %zu integers to stdout.\n", count);

    return 0;
}