#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>  // fixed typo

// Define the structure of a node in our linked list
typedef struct node
{
    int number;        // stores the actual number
    struct node *next; // pointer to the next node in the list
} node;

// Function to free all nodes in a linked list
void free_list(node *head)
{
    // head points to the start of the list
    while (head != NULL)  // loop until we reach the end
    {
        node *temp = head;  // temp holds current node
        head = head->next;  // move head to next node
        free(temp);         // free memory for the current node
        // After freeing temp, memory is gone. head now points to the next node
    }
    // At the end, head == NULL, all nodes freed
}

int main(void)
{
    node *list = NULL; // start with empty list; list points to NULL

    // Build list by prepending 3 numbers
    for (int i = 0; i < 3; i++)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            // If malloc fails, clean up any nodes already in the list
            free_list(list);
            return 1;  // exit with error
        }

        // Get number from user
        n->number = get_int("Number: ");  // example: user enters 5, then 10, then 15

        // Prepend node to the front of the list
        n->next = list;  // n->next points to current head of the list
        list = n;        // list now points to new node, which is the new head

        // Example memory after first iteration:
        // list -> [5 | next=NULL]
        // After second iteration with 10:
        // list -> [10 | next]->[5 | next=NULL]
        // After third iteration with 15:
        // list -> [15 | next]->[10 | next]->[5 | next=NULL]
    }

    // Print the list
    for (node *ptr = list; ptr != NULL; ptr = ptr->next)
    {
        // ptr points to current node in the list
        // ptr->number accesses the value
        // ptr->next points to the next node
        printf("%i\n", ptr->number);

        // Example for third iteration (ptr points to 15 node):
        // ptr->number = 15
        // ptr->next -> [10 | next]->[5 | next=NULL]
    }

    // Free all allocated memory using our function
    free_list(list);
    // After free_list, all memory is released, list nodes no longer exist in memory

    return 0;
}
