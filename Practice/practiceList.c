#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Initial size of the array
    int size = 5;
    int *numbers = (int *)malloc(size * sizeof(int));

    // Check if memory has been allocated successfully
    if (numbers == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < size; i++)
    {
        numbers[i] = i + 1;
    }

    // Print the original array
    printf("Original array:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d\n", numbers[i]);
    }

    // Increase the size of the array
    size = 10;
    numbers = (int *)realloc(numbers, size * sizeof(int));

    // Check if memory has been reallocated successfully
    if (numbers == NULL)
    {
        printf("Memory reallocation failed\n");
        return 1;
    }

    // Add new elements to the array
    for (int i = 5; i < size; i++)
    {
        numbers[i] = i + 1;
    }

    // Print the new array
    printf("New array:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d\n", numbers[i]);
    }

    // Print the array in reverse order
    printf("Array in reverse order:\n");
    for (int i = size - 1; i >= 0; i--)
    {
        printf("%d\n", numbers[i]);
    }

    // Free the allocated memory
    free(numbers);

    printf("Hello, World!\n");
    return 0;
}