// Free a linked list

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
} node;

int main(void)
{
    // Memory for numbers
    node *list = NULL;

    // Build list
    for (int i = 0; i < 3; i++)
    {
        // Allocate node for number
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }
        n->number = get_int("Number: ");
        n->next = NULL;

        // Prepend node to list
        n->next = list;
        list = n;
    }

    // Free memory
    node *ptr = list;
    while (ptr != NULL)
    {
        node *next = ptr->next;
        free(ptr);
        ptr = next;
    }
    return 0;
}


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

void free_memory(node *head)
{
    while (head != NULL)
    {
        node *next = head->next;  // save pointer to next node
        free(head);               // free current node
        head = next;              // move head to next node
    }
}
