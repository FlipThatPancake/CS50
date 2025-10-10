// Implements a sorted linked list of numbers (keeps list in ascending order automatically!)

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
        n->next = NULL;  // initialize next to NULL (we'll update this when we insert it in the right spot)

        // CASE 1: If list is empty
        if (list == NULL)  // checking if this is the very first node we're adding
        {
            list = n;  // list now points to our first (and only) node - simple case!
        }

        // CASE 2: If number belongs at beginning of list (smaller than current first element)
        else if (n->number < list->number)  // new number is smaller than the head of the list
        {
            n->next = list;  // new node points to current first node (prepending like before!)
            list = n;  // update list to point to new node (making it the new head)
        }  // this keeps the list sorted because we're inserting at the front when appropriate

        // CASE 3: If number belongs later in list (middle or end)
        else  // new number is >= list->number, so it goes somewhere after the first node
        {
            // Iterate over nodes in list to find correct insertion point
            for (node *ptr = list; ptr != NULL; ptr = ptr->next)  // walk through list starting from head
            {
                // CASE 3A: If at end of list (ptr->next is NULL)
                if (ptr->next == NULL)  // we've reached the last node
                {
                    // Append node (new number is largest, goes at end)
                    ptr->next = n;  // attach new node to end of list
                    break;  // done! exit the for loop (IMPORTANT: prevents infinite loop)
                }

                // CASE 3B: If in middle of list (found the right spot to insert)
                if (n->number < ptr->next->number)  // new number fits between ptr and ptr->next
                {
                    // Insert node between ptr and ptr->next (maintains sorted order!)
                    n->next = ptr->next;  // new node points to what ptr was pointing to
                    ptr->next = n;  // ptr now points to new node (sandwich it in!)
                    break;  // done! exit the for loop
                }
                // If neither condition is true, keep looping to find the right spot
            }
        }
    }  // NOTE: List is ALWAYS sorted! If user enters 3,1,2 the list will be: 1→2→3→NULL

    // Print numbers
    for (node *ptr = list; ptr != NULL; ptr = ptr->next)  // traverse the list from start to end
    {
        printf("%i\n", ptr->number);  // print each number - will always print in ascending order!
    }  // this is a "for loop traversal" - more compact than while loop but does the same thing

    // Free memory - use helper function instead of duplicating code!
    free_list(list);  // clean, simple, reusable! same logic as error case

    return 0;  // program ends successfully - no memory leaks!
}




// Just Code

typedef struc node
{
    int number;
    struct node *next;
} node;

void free_list(node *head)
{
    while (head != NULL)
    {
        node *next = head->next;
        free(head);
        head = next
    }
}

int main(void)
{
    node *head = NULL;
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
        if (head = NULL)
        {
            head = n;
        }
        else if (n->number < head->number)
        {
            n->next = head;
            head = n;
        }
        else
        {
            for (node *ptr = head; ptr != NULL; ptr = ptr->next)
            {
                if (ptr->next == NULL)
                {
                    ptr->next = n;
                    break;
                }

                if (n->number < ptr->next->number)
                {
                    n->next = ptr->next;
                    ptr->next = n;
                    break;
                }
            }
        }

    }

    for (node *ptr = head; ptr != NULL; ptr = ptr->next)
    {
        printf("%i\n", ptr->number);
    }

    free_list(head);
    return 0;
}
