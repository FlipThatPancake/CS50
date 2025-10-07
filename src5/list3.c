// Start to build a linked list by prepending nodes

#include <cs50.h>  // need this for get_int() function from CS50 library
#include <stdio.h>  // standard input/output library (for printf, etc)
#include <stdlib.h>  // need this for malloc() AND free() to manage memory

typedef struct node  // creating a custom data type called "node" - this is how we define linked list nodes
{
    int number;  // this stores the actual data (an integer in this case)
    struct node *next;  // pointer to the next node in the list (self-referential struct!)
} node;  // now we can use "node" as a type name instead of writing "struct node" every time

int main(void)  // main function where program starts
{
    // Memory for numbers
    node *list = NULL;  // initialize list pointer to NULL (empty list to start) - this will point to the first node

    // Build list
    for (int i = 0; i < 3; i++)  // loop 3 times to get 3 numbers from user
    {
        // Allocate node for number
        node *n = malloc(sizeof(node));  // allocate memory for a new node on the heap (not stack!) - malloc returns void* so it gets casted to node*
        if (n == NULL)  // ALWAYS check if malloc failed (returns NULL if out of memory)
        {
            // Free any nodes we already allocated before exiting
            while (list != NULL)  // loop through existing list
            {
                node *temp = list;       // temp and list both point to node at 0x100
                list = list->next;       // list now points to 0x200 (moved forward)
                free(temp);              // frees the node at 0x100
                                         // list is safe because it already moved to 0x200!
            }
            return 1;  // exit program with error code if malloc failed
        }
        n->number = get_int("Number: ");  // prompt user and store their input in the number field (using -> because n is a pointer)
        n->next = NULL;  // initialize next pointer to NULL (good practice even though we're about to change it)

        // Prepend node to list
        n->next = list;  // make new node point to current first node (or NULL if list is empty) - this is KEY for prepending
        list = n;  // update list to point to new node (making it the new head of the list)
    }

    // FREE THE MEMORY! Super important - every malloc needs a matching free
    while (list != NULL)  // loop through the entire list
    {
        node *temp = list;       // temp and list both point to node at 0x100
        list = list->next;       // list now points to 0x200 (moved forward)
        free(temp);              // frees the node at 0x100
                                 // list is safe because it already moved to 0x200!
    }

    return 0;  // program ends successfully - no memory leaks now!
}
