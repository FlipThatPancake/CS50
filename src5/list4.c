// Print nodes in a linked list with a while loop

#include <cs50.h>  // need this for get_int() function from CS50 library
#include <stdio.h>  // standard input/output library (for printf to print stuff)
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
            // FIXED: Free any nodes we already allocated before exiting
            while (list != NULL)  // loop through existing list
            {
                node *temp = list;  // save current node
                list = list->next;  // move to next node
                free(temp);  // free the saved node
            }
            return 1;  // exit program with error code if malloc failed
        }
        n->number = get_int("Number: ");  // prompt user and store their input in the number field (using -> because n is a pointer)
        n->next = NULL;  // initialize next pointer to NULL (good practice even though we're about to change it)

        // Prepend node to list
        n->next = list;  // make new node point to current first node (or NULL if list is empty) - this is KEY for prepending
        list = n;  // update list to point to new node (making it the new head of the list)
    }  // NOTE: because we prepend, if user enters 1,2,3 the list will be: 3→2→1→NULL (reverse order!)

    // Print numbers
    node *ptr = list;  // create a "traversal pointer" that starts at the beginning of the list - we use ptr so we don't lose the list pointer
    while (ptr != NULL)  // keep looping as long as ptr points to an actual node (not NULL/end of list)
    {
        printf("%i\n", ptr->number);  // print the number in the current node - %i is format specifier for int, \n is newline
        ptr = ptr->next;  // move ptr to the next node in the list (this is how we traverse/walk through the list!)
    }  // when ptr becomes NULL, we've reached the end of the list and the loop stops

    // FIXED: Free all the memory we allocated!
    while (list != NULL)  // loop through the entire list
    {
        node *temp = list;  // save pointer to current node (can't access list->next after freeing list!)
        list = list->next;  // move list pointer to next node BEFORE freeing
        free(temp);  // free the memory we allocated for this node - order matters!
    }  // every malloc needs a matching free - golden rule of C!

    return 0;  // program ends successfully - no memory leaks now!
}
