// This program generates a pyramid of hashes to simulate Mario blocks
// Week 1 Assignment 1


#include <cs50.h>
#include <stdio.h>

void print_row(int n, int n_of_hashes);
int get_correct_input(void);

int main(void)
{
    int n = get_correct_input();
    for (int i = 0; i < n; i++)
    {
        print_row(n, i + 1);
    }
}

// Print single row
void print_row(int height, int n_of_hashes)
{

    // Print left side
    for (int i = 0; i < height - n_of_hashes; i++)
    {
        printf(" ");
    }
    for (int i = 0; i < n_of_hashes; i++)
    {
        printf("#");
    }

    // Print right side
    printf("  ");
    for (int i = 0; i < n_of_hashes; i++)
    {
        printf("#");
    }
    printf("\n");
}

// Get user input
int get_correct_input(void)
{
    int n;
    do
    {
        n = get_int("Height (1-8): \n");
    }
    while (n < 1 || n > 8);
    return n;
}
