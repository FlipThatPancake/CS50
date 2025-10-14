// ============================================================================
// FILE: dictionary.c
// PURPOSE: Implements all the dictionary functions using a hash table
// This is where the "magic" happens - loading words, checking spelling, etc.
// ============================================================================

#include <ctype.h>      // For tolower() function to make words case-insensitive
#include <stdbool.h>    // For bool type (true/false)
#include <stdio.h>      // For FILE operations like fopen, fclose, fscanf
#include <stdlib.h>     // For malloc() and free() - dynamic memory allocation
#include <string.h>     // For strcpy() and strlen() - string manipulation
#include <strings.h>    // For strcasecmp() - case-insensitive string comparison

#include "dictionary.h" // Header file with function prototypes and LENGTH constant

// Represents a node in a hash table
// This is a linked list node that stores one word from the dictionary
typedef struct node
{
    char word[LENGTH + 1];  // Array to store the word (+1 for null terminator '\0')
    struct node *next;       // Pointer to the next node in the linked list (for collision handling)
} node;

// Choose number of buckets in hash table
// Using 26*26 = 676 buckets (one for each two-letter combination like "aa", "ab", etc.)
// More buckets = fewer collisions = faster lookups!
const unsigned int N = 676;

// Hash table - array of pointers to linked lists
// Each index points to the first node of a linked list (or NULL if empty)
node *table[N];

// Global variable to keep track of how many words we've loaded
// We increment this in load() and use it in size()
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Step 1: Hash the word to get an index (which bucket to look in)
    unsigned int index = hash(word);
    
    // Step 2: Create a cursor pointer to traverse the linked list at table[index]
    // Start at the head of the list
    node *cursor = table[index];
    
    // Step 3: Traverse the linked list looking for the word
    while (cursor != NULL)  // Keep going until we reach the end of the list
    {
        // Compare the word we're looking for with the current node's word
        // strcasecmp = case-insensitive comparison (so "The" matches "the")
        // Returns 0 if strings are equal
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;  // Found it! Word is spelled correctly
        }
        
        // Move to the next node in the linked list
        cursor = cursor->next;
    }
    
    // We've checked the entire linked list and didn't find the word
    return false;  // Word is misspelled (not in dictionary)
}

// Hashes word to a number
// Takes a word and returns an index (0 to N-1) to determine which bucket it goes in
unsigned int hash(const char *word)
{
    // Using a simple hash function based on first two letters
    // Convert to uppercase and calculate index
    
    // Get first character, convert to uppercase, get value 0-25
    unsigned int hash_value = toupper(word[0]) - 'A';
    
    // If there's a second character, factor it in too
    // This gives us 26*26 = 676 possible hash values
    if (word[1] != '\0')  // Check if there's a second character
    {
        hash_value = hash_value * 26 + (toupper(word[1]) - 'A');
    }
    
    // Make sure we don't go out of bounds (mod by N to keep it in range)
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Step 1: Open the dictionary file
    FILE *file = fopen(dictionary, "r");  // "r" means read mode
    if (file == NULL)  // Check if file opening failed
    {
        return false;  // Can't load dictionary if we can't open the file
    }
    
    // Step 2: Initialize the hash table to NULL
    // This makes sure all buckets start empty
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;  // Each bucket starts with no nodes
    }
    
    // Step 3: Read words from dictionary file one at a time
    char word[LENGTH + 1];  // Temporary buffer to hold each word we read
    
    // fscanf reads formatted input from file
    // "%s" means read a string (stops at whitespace)
    // Returns EOF (End Of File) when done reading
    while (fscanf(file, "%s", word) != EOF)
    {
        // Step 4: Create a new node for this word
        node *new_node = malloc(sizeof(node));  // Allocate memory for a new node
        if (new_node == NULL)  // Check if malloc failed (out of memory)
        {
            fclose(file);  // Clean up: close the file
            unload();      // Clean up: free any memory we've allocated so far
            return false;  // Loading failed
        }
        
        // Step 5: Copy the word into the new node
        strcpy(new_node->word, word);  // Copy word into node's word array
        
        // Step 6: Hash the word to find which bucket it belongs in
        unsigned int index = hash(word);
        
        // Step 7: Insert node at the beginning of the linked list (easiest/fastest)
        // This is called "prepending" - adding to the front
        new_node->next = table[index];  // New node points to current first node
        table[index] = new_node;        // New node becomes the new first node
        
        // Step 8: Increment our word counter
        word_count++;
    }
    
    // Step 9: Close the file (important to free up resources!)
    fclose(file);
    
    // Successfully loaded all words!
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Simply return the count we've been tracking
    // We increment word_count every time we load a word in load()
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // We need to free ALL memory we allocated with malloc()
    // This means freeing every node in every linked list
    
    // Step 1: Loop through each bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        // Step 2: For each bucket, traverse its linked list and free each node
        node *cursor = table[i];  // Start at the head of this bucket's list
        
        while (cursor != NULL)  // Keep going until we reach the end
        {
            // Important: Save the next pointer BEFORE we free the current node!
            // If we free first, we lose access to cursor->next!
            node *temp = cursor;      // Remember current node
            cursor = cursor->next;    // Move to next node
            free(temp);               // Now free the old node
        }
        
        // After freeing all nodes in this bucket, set it to NULL
        table[i] = NULL;
    }
    
    // Reset word count since dictionary is now empty
    word_count = 0;
    
    // Successfully unloaded everything!
    return true;
}
