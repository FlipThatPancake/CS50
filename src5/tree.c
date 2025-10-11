// Implements a list of numbers as a binary search tree
// A BST keeps smaller numbers on the left, bigger numbers on the right

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Represents a node (one box that holds a number)
typedef struct node
{
    int number;
    struct node *left;   // Points to smaller numbers
    struct node *right;  // Points to bigger numbers
}
node;

// Function declarations
node *insert(node *root, int value);
bool search(node *root, int value);
void free_tree(node *root);
void print_tree(node *root);

int main(void)
{
    // Start with an empty tree
    node *tree = NULL;

    // Add numbers to the tree
    // The insert function automatically puts them in the right place
    tree = insert(tree, 2);
    tree = insert(tree, 1);
    tree = insert(tree, 3);
    tree = insert(tree, 5);
    tree = insert(tree, 4);

    // Check if insert failed
    if (tree == NULL)
    {
        printf("Error: Could not create tree\n");
        return 1;
    }

    // Print all numbers in order (smallest to largest)
    printf("Numbers in the tree:\n");
    print_tree(tree);

    // Search for a number
    int search_for = 4;
    if (search(tree, search_for))
    {
        printf("\nFound %i in the tree!\n", search_for);
    }
    else
    {
        printf("\n%i is not in the tree.\n", search_for);
    }

    // Clean up memory
    free_tree(tree);
    return 0;
}

// Inserts a new number into the tree in the correct position
node *insert(node *root, int value)
{
    // If we reach an empty spot, create a new node here
    if (root == NULL)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Error: Out of memory\n");
            return NULL;
        }
        n->number = value;
        n->left = NULL;
        n->right = NULL;
        return n;
    }

    // If the value is smaller, go left
    if (value < root->number)
    {
        root->left = insert(root->left, value);
    }
    // If the value is bigger, go right
    else if (value > root->number)
    {
        root->right = insert(root->right, value);
    }
    // If the value is equal, don't add it (no duplicates)

    return root;
}

// Searches for a number in the tree
// Returns true if found, false if not found
bool search(node *root, int value)
{
    // If we reach an empty spot, the number isn't here
    if (root == NULL)
    {
        return false;
    }

    // If we found it, return true
    if (value == root->number)
    {
        return true;
    }

    // If the value is smaller, search left
    if (value < root->number)
    {
        return search(root->left, value);
    }
    // Otherwise, search right
    else
    {
        return search(root->right, value);
    }
}

// Frees all memory used by the tree
// Uses "post-order" traversal: left, right, then root
void free_tree(node *root)
{
    if (root == NULL)
    {
        return;
    }
    free_tree(root->left);   // Free left side first
    free_tree(root->right);  // Then free right side
    free(root);              // Finally free this node
}

// Prints all numbers in sorted order (smallest to largest)
// Uses "in-order" traversal: left, root, right
void print_tree(node *root)
{
    if (root == NULL)
    {
        return;
    }
    print_tree(root->left);       // Print smaller numbers first
    printf("%i\n", root->number); // Print this number
    print_tree(root->right);      // Print bigger numbers last
}
