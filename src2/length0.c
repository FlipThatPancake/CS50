// Determines the length of a string

#include <cs50.h>
#include <stdio.h>


int main(void)
{
    // Prompt for input
    string name = get_string("What's your name");

    int n = 0;
    // Count number of chars until '\0' (aka NUL)
    while (name[n] != '\0')
    {
        n++;
    }
    printf("%i\n", n);
}
