// Appends numbers to a linked list (adds to the END, not the front!)

#include <cs50.h>  // need this for get_int() function from CS50 library
#include <stdio.h>  // standard input/output library (for printf to print stuff)
#include <stdlib.h>  // need this for malloc() AND free() to manage memory

typedef struct node  // creating a custom data type called "node" - this is how we define linked list nodes
{
    int number;  // this stores the actual data (an integer in this case)
    struct node *next;  // pointer to the next node in the list (self-referential struct!)
} node;  // now we can use "node" as a type name instead of writing "struct node" every time

// Helper function to free the entire linked list - keeps code DRY (Don't Repeat Yourself!)
void free_list(node *list)  // takes a pointer to the first node of the list
{
    while (list != NULL)  // loop through entire list until we hit NULL (end)
    {
        node *next = list->next;  // save pointer to next node BEFORE freeing (can't access list->next after free!)
        free(list);  // free the current node's memory
        list = next;  // move to the next node (using saved pointer)
    }  // every malloc needs a matching free - this function handles all of them!
}

int main(void)  // main function where program starts
{
    // Memory for numbers
    node *list = NULL;  // initialize list pointer to NULL (empty list to start) - this will point to the first node
    node *tail = NULL;  // IMPROVEMENT: keep track of the last node so we don't have to search for it every time! much faster

    // Build list
    for (int i = 0; i < 3; i++)  // loop 3 times to get 3 numbers from user
    {
        // Allocate node for number
        node *n = malloc(sizeof(node));  // allocate memory for a new node on the heap (not stack!) - malloc returns void* so it gets casted to node*
        if (n == NULL)  // ALWAYS check if malloc failed (returns NULL if out of memory)
        {
            // FIXED: Use helper function to free any nodes we already allocated before exiting
            free_list(list);  // much cleaner! no code duplication
            return 1;  // exit program with error code if malloc failed
        }
        n->number = get_int("Number: ");  // prompt user and store their input in the number field (using -> because n is a pointer)
        n->next = NULL;  // set next to NULL because this will be the last node in the list

        // If list is empty
        if (list == NULL)  // checking if this is the very first node we're adding
        {
            // This node is the whole list
            list = n;  // list now points to our first (and only) node
            tail = n;  // IMPROVEMENT: tail also points to this node (since it's both first and last!)
        }

        // If list has numbers already
        else  // list already has at least one node
        {
            // IMPROVED VERSION: Instead of searching for the end, use tail pointer!
            tail->next = n;  // attach new node to the current last node (O(1) instead of O(n)!)
            tail = n;  // update tail to point to the new last node

            /* OLD INEFFICIENT WAY (commented out):
            // Iterate over nodes in list
            for (node *ptr = list; ptr != NULL; ptr = ptr->next)  // walk through entire list every time - slow! O(n)
            {
                // If at end of list
                if (ptr->next == NULL)  // found the last node (next is NULL)
                {
                    // Append node
                    ptr->next = n;  // attach new node to end
                    break;  // exit loop (important! otherwise we'd keep going with ptr = ptr->next and crash)
                }
            }
            */
        }
    }  // NOTE: because we APPEND, if user enters 1,2,3 the list will be: 1→2→3→NULL (same order as input!)

    // Print numbers
    for (node *ptr = list; ptr != NULL; ptr = ptr->next)  // traverse the list from start to end
    {
        printf("%i\n", ptr->number);  // print each number - will print in the order user entered them!
    }  // this is a "for loop traversal" - more compact than while loop but does the same thing

    // Free memory - use helper function instead of duplicating code!
    free_list(list);  // clean, simple, reusable! same logic as error case
    // NOTE: tail pointer doesn't need to be freed - it was just pointing to nodes we already freed

    return 0;  // program ends successfully - no memory leaks!
}


// Just code
#include <cs50.h>
#include <stlib.h>
#include <stdio.h>

typedef struc node
{
    int number;
    node struc *node;
} node;

void free_list(node *head)
{
    while (head != NULL)
    {
        node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    node *head = NULL;
    node *tail = NULL;

    for (int i = 0; i < 3; i++)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free_list(head);
            return 1;
        }

        n->number = get_int("");
        n->next = NULL;
        if (head == NULL)
        {
            head = n;
            tail = n;
        }
        else
        {
            tail->next = n;
            tail = n;
        }
    }

    for (node *ptr = head; ptr != NULL; ptr = ptr->next)
    {
        printf("%i\n", ptr->number);
    }

    free_list(head);
    return 0;

}
