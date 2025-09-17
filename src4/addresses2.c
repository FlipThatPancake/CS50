// Stores and prints an integer's address

#include <stdio.h>

int main(void)
{
    int n = 50; // declares a variable n and initializes it with value 50
    int *p = &n; // declares a pointer to an integer; initialized to point to address of n
    printf("%p\n", p); // prints the address of the value of the pointer which is the memory address of n "0x7fff6adcb3fc"
}



